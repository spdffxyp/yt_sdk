/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include "../include/sw_ctrl_comm_smi.h"
#include "../include/share/sw_ctrl_comm_share.h"

typedef struct cmd_dev_s {
    struct cdev cdev;
    dev_t devno;
    struct device *dev;
    struct class *cls;
}sw_ctrl_dev_t;

static sw_ctrl_dev_t gSwCtrlDev;

static int32_t switch_ctrl_process(sw_ctrl_comm_data_t *data)
{
    uint32_t ret;

    if (data->flags & SW_CTRL_FLAG_WRITE)
    {
        ret = switch_ctrl_smi_write(data->unit, data->regAddr, data->regVal);
    }
    else if (data->flags & SW_CTRL_FLAG_READ)
    {
        ret = switch_ctrl_smi_read(data->unit, data->regAddr, &data->regVal);
    }
    else
    {
        pr_err("%s parameter error unit %u regAddr 0x%08x regVal 0x%08x flags 0x%x\n",
                __func__, data->unit, data->regAddr, data->regVal, data->flags);
        return -EINVAL;
    }

    if (ret)
    {
        pr_err("%s smi operation error ret %u unit %u regAddr 0x%08x regVal 0x%08x flags 0x%x\n",
                __func__, ret, data->unit, data->regAddr, data->regVal, data->flags);
        return -EIO;
    }

    return 0;
}

static long switch_ctrl_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    sw_ctrl_comm_data_t data;
    sw_ctrl_comm_data_t __user *dataUser = (sw_ctrl_comm_data_t __user *)arg;
    int32_t ret = 0;

    memset(&data, 0, sizeof(data));
    ret = copy_from_user((void *)&data, (void *)dataUser, sizeof(sw_ctrl_comm_data_t));
    if (ret)
    {
        pr_err("%s %d copy data from user error\n", __func__, __LINE__);
		return -EFAULT;
    }

    switch (cmd)
    {
        case SWACCESSIOCTL:
            ret = switch_ctrl_process(&data);
            if (ret)
            {
                return ret;
            }
            ret = copy_to_user((void *)dataUser, (void *)&data, sizeof(sw_ctrl_comm_data_t));
            break;
        default:
            ret = -EOPNOTSUPP;
            break;
    }

    return ret;
}

static const struct file_operations fileOps = {
    .owner		= THIS_MODULE,
    .unlocked_ioctl = switch_ctrl_ioctl,
};

int32_t __init switch_ctrl_cdev_init(void)
{
    int32_t ret;

    memset(&gSwCtrlDev, 0, sizeof(gSwCtrlDev));
    ret = alloc_chrdev_region(&gSwCtrlDev.devno, 0, 1, "swCtrlDev");
    if (ret)
    {
        pr_err("%s %d alloc chrdev fail ret %d\n", __func__, __LINE__, ret);
        goto alloc_cdev_err;
    }

    cdev_init(&gSwCtrlDev.cdev, &fileOps);
    gSwCtrlDev.cdev.owner = THIS_MODULE;
    ret = cdev_add(&gSwCtrlDev.cdev, gSwCtrlDev.devno, 1);
    if (ret)
    {
        pr_err("%s %d add cdev fail ret %d\n", __func__, __LINE__, ret);
        goto add_cdev_err;
    }

    gSwCtrlDev.cls = class_create(THIS_MODULE, "swCtrlClass");
    if (IS_ERR(gSwCtrlDev.cls))
    {
        ret = PTR_ERR(gSwCtrlDev.cls);
        pr_err("%s %d create class fail ret %d\n", __func__, __LINE__, ret);
        goto class_err;
    }

    gSwCtrlDev.dev = device_create(gSwCtrlDev.cls, NULL, gSwCtrlDev.devno, NULL, "swCtrlDev");
    if (IS_ERR(gSwCtrlDev.dev))
    {
        ret = PTR_ERR(gSwCtrlDev.dev);
        pr_err("%s %d create device fail ret %d\n", __func__, __LINE__, ret);
        goto device_err;
    }

    switch_ctrl_smi_init();
    pr_info("%s %d switch control cdev init OK\n", __func__, __LINE__);

    return 0;

device_err:
    class_destroy(gSwCtrlDev.cls);
class_err:
    cdev_del(&gSwCtrlDev.cdev);
add_cdev_err:
    unregister_chrdev_region(gSwCtrlDev.devno, 1);
alloc_cdev_err:
    return ret;
}

void __exit switch_ctrl_cdev_exit(void)
{
    cdev_del(&gSwCtrlDev.cdev);
    unregister_chrdev_region(gSwCtrlDev.devno, 1);
    device_destroy(gSwCtrlDev.cls, gSwCtrlDev.devno);
    class_destroy(gSwCtrlDev.cls);
}

module_init(switch_ctrl_cdev_init);
module_exit(switch_ctrl_cdev_exit);

MODULE_LICENSE("GPL");