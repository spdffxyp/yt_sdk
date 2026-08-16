#if 1
#include <linux/version.h>
#include <linux/proc_fs.h>

extern int yt_smi_cl22_write_unit0(u8 phyAddr, u8 regAddr, u16 regVal); 
extern int yt_smi_cl22_read_unit0(u8 phyAddr, u8 regAddr, u16 *pRegVal);

/* SMI format */ 
#define REG_ADDR_BIT1_ADDR 0
#define REG_ADDR_BIT1_DATA 1
#define REG_ADDR_BIT0_WRITE 0
#define REG_ADDR_BIT0_READ 1
#define PHYADDR 0x1D /*base on Hardware Switch Phyaddr*/ 
#define SWITCHID 0x0 /*base on Hardware Switch SwitchID*/ 

static struct mutex smi_reg_mutex; 

static u32 yt_smi_switch_write(u32 reg_addr, u32 reg_value)
{ 
    u8 phyAddr; 
    u8 switchId; 
    u8 regAddr; 
    u16 regVal; 
    
    mutex_lock(&smi_reg_mutex); 
    phyAddr = PHYADDR; 
    switchId = SWITCHID; 
    regAddr = (switchId<<2)|(REG_ADDR_BIT1_ADDR<<1)|(REG_ADDR_BIT0_WRITE);
    
    /* Set reg_addr[31:16] */ 
    regVal = (reg_addr >> 16)&0xffff; 
    yt_smi_cl22_write_unit0(phyAddr, regAddr, regVal);
    
    /* Set reg_addr[15:0] */ 
    regVal = reg_addr&0xffff; 
    yt_smi_cl22_write_unit0(phyAddr, regAddr, regVal);
    
    /* Write Data [31:16] out */ 
    regAddr = (switchId<<2)|(REG_ADDR_BIT1_DATA<<1)|(REG_ADDR_BIT0_WRITE); 
    regVal = (reg_value >> 16)&0xffff; 
    yt_smi_cl22_write_unit0(phyAddr, regAddr, regVal);
    
    /* Write Data [15:0] out */ 
    regVal = reg_value&0xffff; 
    yt_smi_cl22_write_unit0(phyAddr, regAddr, regVal); 
    
    mutex_unlock(&smi_reg_mutex); 
    return 0; 
}

static u32 yt_smi_switch_read(u32 reg_addr, u32 *reg_value)
{ 
    u32 rData; 
    u8 phyAddr; 
    u8 switchId; 
    u8 regAddr; 
    u16 regVal; 
    
    mutex_lock(&smi_reg_mutex); 
    phyAddr = PHYADDR; 
    switchId = SWITCHID; 
    regAddr = (switchId<<2)|(REG_ADDR_BIT1_ADDR<<1)|(REG_ADDR_BIT0_READ);
    
    /* Set reg_addr[31:16] */ 
    regVal = (reg_addr >> 16)&0xffff; 
    yt_smi_cl22_write_unit0(phyAddr, regAddr, regVal);/*change to platform smi write*/
    
    /* Set reg_addr[15:0] */ 
    regVal = reg_addr&0xffff; 
    yt_smi_cl22_write_unit0(phyAddr, regAddr, regVal);
    
    regAddr = (switchId<<2)|(REG_ADDR_BIT1_DATA<<1)|(REG_ADDR_BIT0_READ);
    
    /* Read Data [31:16] */ 
    regVal = 0x0; 
    yt_smi_cl22_read_unit0(phyAddr, regAddr, &regVal);/*change to platform smi read*/ 
    rData = (uint32_t)(regVal<<16);
    
    /* Read Data [15:0] */ 
    regVal = 0x0; 
    yt_smi_cl22_read_unit0(phyAddr, regAddr, &regVal); 
    rData |= regVal; 
    *reg_value = rData; 
    mutex_unlock(&smi_reg_mutex); 
    return 0; 
}

static void yt_smi_switch_rmw(u32 reg, u32 mask, u32 set)
{ 
    u32 val = 0; 
    yt_smi_switch_read(reg, &val); 
    val &= ~mask;
    val |= set;
    yt_smi_switch_write(reg, val); 
}

static ssize_t smi_write_proc(struct file *filp, const char *buffer, size_t count, loff_t *offp)
{
    char *str, *cmd, *value; 
    char tmpbuf[128] = {0}; 
    uint32_t regAddr = 0; 
    uint32_t regData = 0; 
    uint32_t rData = 0;
    
    if(count >= sizeof(tmpbuf)) 
        goto error;
        
    if(!buffer || copy_from_user(tmpbuf, buffer, count) != 0)
        return 0;
        
    if (count > 0)
    {
        str = tmpbuf; 
        cmd = strsep(&str, "\t \n");
        
        if (!cmd)
        { 
            goto error; 
        }
        
        if (strcmp(cmd, "write") == 0)
        { 
            value = strsep(&str, "\t \n");
            if (!value)
            { 
                goto error; 
            }
            regAddr = simple_strtoul(value, &value, 16); 
            
            value = strsep(&str, "\t \n");
            if (!value)
            { 
                goto error; 
            }
            regData = simple_strtoul(value, &value, 16); 
            printk("write regAddr = 0x%x regData = 0x%x\n", regAddr, regData);
            yt_smi_switch_write(regAddr, regData); 
        }
        else if (strcmp(cmd, "read") == 0)
        { 
            value = strsep(&str, "\t \n");
            if (!value)
            { 
                goto error; 
            }
            regAddr = simple_strtoul(value, &value, 16); 
            printk("read regAddr = 0x%x ", regAddr);
            yt_smi_switch_read(regAddr, &rData);
            printk("regData = 0x%x\n", rData); 
        }
        else 
        { 
            goto error; 
        }
    }
    return count; 

error: 
    printk("usage: \n");
    printk("  read regaddr: for example, echo read 0xd0004 > /proc/smi\n");
    printk("  write regaddr regdata: for example; echo write 0xd0004 0x680 > /proc/smi\n"); 
    return -EFAULT; 
}

static struct proc_dir_entry *smi_proc; 

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops smi_proc_fops = {
    .proc_write = smi_write_proc,
};
#else
static const struct file_operations smi_proc_fops = {
    .write = smi_write_proc,
};
#endif

struct stat_mib_counter { 
    unsigned int size; 
    unsigned int offset; 
    const char *name; 
}; 

static const struct stat_mib_counter stat_mib[] = {
    { 1, 0x00, "RxBcast"}, 
    { 1, 0x04, "RxPause"}, 
    { 1, 0x08, "RxMcast"}, 
    { 1, 0x0C, "RxCrcErr"}, 
    { 1, 0x10, "RxAlignErr"}, 
    { 1, 0x14, "RxRunt"}, 
    { 1, 0x18, "RxFragment"}, 
    { 1, 0x1C, "RxSz64"}, 
    { 1, 0x20, "RxSz65To127"}, 
    { 1, 0x24, "RxSz128To255"}, 
    { 1, 0x28, "RxSz256To511"}, 
    { 1, 0x2C, "RxSz512To1023"}, 
    { 1, 0x30, "RxSz1024To1518"}, 
    { 1, 0x34, "RxJumbo"}, 
    /*{ 1, 0x38, "RxMaxByte"},*/ 
    { 2, 0x3C, "RxOkByte"},
    { 2, 0x44, "RxNoOkByte"}, 
    { 1, 0x4C, "RxOverFlow"}, 
    /*{ 1, 0x50, "QMFilter"},*/ 
    { 1, 0x54, "TxBcast"}, 
    { 1, 0x58, "TxPause"}, 
    { 1, 0x5C, "TxMcast"}, 
    /*{ 1, 0x60, "TxUnderRun"},*/ 
    { 1, 0x64, "TxSz64"}, 
    { 1, 0x68, "TxSz65To127"}, 
    { 1, 0x6C, "TxSz128To255"}, 
    { 1, 0x70, "TxSz256To511"}, 
    { 1, 0x74, "TxSz512To1023"}, 
    { 1, 0x78, "TxSz1024To1518"}, 
    { 1, 0x7C, "TxJumbo"}, 
    { 1, 0x80, "TxOverSize"}, 
    { 2, 0x84, "TxOkByte"}, 
    { 1, 0x8C, "TxCollision"}, 
    /*{ 1, 0x90, "TxAbortCollision"},*/
    /*{ 1, 0x94, "TxMultiCollision"},*/
    /*{ 1, 0x98, "TxSingleCollision"},*/
    /*{ 1, 0x9C, "TxExcDefer"},*/
    /*{ 1, 0xA0, "TxDefer"},*/ 
    { 1, 0xA4, "TxLateCollision"}, 
    /*{ 1, 0xA8, "RxOamCounter"},*/
    /*{ 1, 0xAC, "TxOamCounter"},*/ 
}; 

#define YT9215_PORT_MIB_BASE(n) (0xc0100 + (n) * 0x100) 

static u32 stat_mib_port_get(u8 unit, u32 port)
{
    int i = 0; 
    u32 lowData = 0; 
    u32 highData = 0; 
    u64 resultData = 0;
    int mibCount; 
    u64 count = 0; 
    mibCount = ARRAY_SIZE(stat_mib);
    
    printk("%-20s %20d\n", "port", port); 
    for (i = 0; i < mibCount; i++)
    {
        count = 0; 
        yt_smi_switch_read(YT9215_PORT_MIB_BASE(port) + stat_mib[i].offset, &lowData); 
        count = lowData;
        
        if (stat_mib[i].size == 2)
        { 
            yt_smi_switch_read(YT9215_PORT_MIB_BASE(port) + stat_mib[i].offset + 4, &highData); 
            resultData = highData; 
            count |= resultData << 32; 
        }
        
        if (stat_mib[i].size == 1) 
            printk("%-20s %20u\n", stat_mib[i].name, (u32)count); 
        else
            printk("%-20s %20llu\n", stat_mib[i].name, count); 
    }
    return 0; 
}

static ssize_t mib_write_proc(struct file *filp, const char *buffer, size_t count, loff_t *offp)
{
    char *str, *cmd, *value; 
    char tmpbuf[128] = {0}; 
    uint32_t port = 0; 
    // uint32_t ret = 0; 
    uint8_t unit = 0;
    
    if(count >= sizeof(tmpbuf)) 
        goto error;
        
    if(!buffer || copy_from_user(tmpbuf, buffer, count) != 0)
        return 0;
        
    if (count > 0)
    {
        str = tmpbuf; 
        cmd = strsep(&str, "\t \n");
        if (!cmd)
        { 
            goto error; 
        }
        if (strcmp(cmd, "mib") == 0)
        {
            cmd = strsep(&str, "\t \n");
            if (!cmd)
            { 
                goto error; 
            }
            if (strcmp(cmd, "enable") == 0)
            { 
                yt_smi_switch_rmw(0x80004, 1<<1, 1<<1); 
            }
            else if (strcmp(cmd, "disable") == 0)
            { 
                yt_smi_switch_rmw(0x80004, 1<<1, 0<<1); 
            }
            else if (strcmp(cmd, "clear") == 0)
            { 
                u32 ctrl_data = 0; 
                yt_smi_switch_read(0xc0004, &ctrl_data); 
                yt_smi_switch_write(0xc0004, 0<<0);
                yt_smi_switch_write(0xc0004, 1<<30); 
            }
            else if (strcmp(cmd, "get") == 0)
            { 
                value = strsep(&str, "\t \n");
                if (!value)
                { 
                    goto error; 
                }
                port = simple_strtoul(value, &value, 10);
                if (port <= 9)
                {
                    stat_mib_port_get(unit, port); 
                }
            }
            else 
            { 
                goto error; 
            }
        }
        else 
        { 
            goto error; 
        }
    }
    return count; 

error: 
    printk("usage: \n"); 
    printk("  mib enable  : for example, echo mib enable > /proc/mib\n"); 
    printk("  mib disable : for example, echo mib disable > /proc/mib\n"); 
    printk("  mib clear   : for example, echo mib clear > /proc/mib\n"); 
    printk("  mib get port : for example, echo mib get 8 > /proc/mib\n"); 
    printk("  get mib 8/9 for extern RGMII counter \n"); 
    return -EFAULT; 
}

static struct proc_dir_entry *mib_proc; 

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops mib_proc_fops = {
    .proc_write = mib_write_proc,
};
#else
static const struct file_operations mib_proc_fops = {
    .write = mib_write_proc,
};
#endif

void smi_mib_proc_test(void)
{ 
    mutex_init(&smi_reg_mutex); 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
    smi_proc = proc_create("smi", 0666, NULL, &smi_proc_fops); 
    mib_proc = proc_create("mib", 0666, NULL, &mib_proc_fops); 
#else
    smi_proc = proc_create("smi", 0666, NULL, &smi_proc_fops); 
    mib_proc = proc_create("mib", 0666, NULL, &mib_proc_fops); 
#endif
}
#endif
