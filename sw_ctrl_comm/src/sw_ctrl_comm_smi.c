/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include <linux/types.h>
#include <linux/mutex.h>

#define REG_ADDR_BIT1_ADDR      0
#define REG_ADDR_BIT1_DATA      1
#define REG_ADDR_BIT0_WRITE     0
#define REG_ADDR_BIT0_READ      1
/* modify according to switch Phyaddr */
#define SWITCH_ADDR             (0x1D)
/* modify according to switch SwitchID */
#define SWITCH_ID               (0x0)

static struct mutex smi_lock;

/* porting the follow macro or function according to the platform */
static uint32_t switch_ctrl_mdio_write(uint8_t phyAddr, uint8_t regAddr, uint16_t regVal)
{

    return 0;
}

static uint32_t switch_ctrl_mdio_read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pRegVal)
{

    return 0;
}
/* porting the above macro or function according to the platform */

uint32_t switch_ctrl_smi_write(uint8_t unit, uint32_t addr, uint32_t data)
{
    uint8_t phyAddr;
    uint8_t switchId;
    uint8_t regAddr;
    uint16_t regVal;
    uint32_t ret = 0;

    mutex_lock(&smi_lock);

    phyAddr = SWITCH_ADDR;
    switchId = SWITCH_ID;
    regAddr = (switchId << 2) | (REG_ADDR_BIT1_ADDR << 1) | (REG_ADDR_BIT0_WRITE);
    /* Set addr[31:16] */
    regVal = (addr >> 16) & 0xffff;
    ret = switch_ctrl_mdio_write(phyAddr, regAddr, regVal);
    if (ret)
    {
        goto out;
    }

    /* Set addr[15:0] */
    regVal = addr & 0xffff;
    ret = switch_ctrl_mdio_write(phyAddr, regAddr, regVal);
    if (ret)
    {
        goto out;
    }

    /* Write data [31:16] out */
    regAddr = (switchId << 2) | (REG_ADDR_BIT1_DATA << 1) | (REG_ADDR_BIT0_WRITE);
    regVal = (data >> 16) & 0xffff;
    ret = switch_ctrl_mdio_write(phyAddr, regAddr, regVal);
    if (ret)
    {
        goto out;
    }

    /* Write data [15:0] out */
    regVal = data & 0xffff;
    ret = switch_ctrl_mdio_write(phyAddr, regAddr, regVal);

out:
    mutex_unlock(&smi_lock);

    return ret;
}

uint32_t switch_ctrl_smi_read(uint8_t unit, uint32_t addr, uint32_t *data)
{
    uint8_t phyAddr;
    uint8_t switchId;
    uint8_t regAddr;
    uint16_t regVal;
    uint32_t tmpData;
    uint32_t ret = 0;

    mutex_lock(&smi_lock);

    phyAddr = SWITCH_ADDR;
    switchId = SWITCH_ID;

    regAddr = (switchId << 2) | (REG_ADDR_BIT1_ADDR << 1) | (REG_ADDR_BIT0_READ);
    /* Set addr[31:16] */
    regVal = (addr >> 16) & 0xffff;
    ret = switch_ctrl_mdio_write(phyAddr, regAddr, regVal);
    if (ret)
    {
        goto out;
    }

    /* Set addr[15:0] */
    regVal = addr & 0xffff;
    ret = switch_ctrl_mdio_write(phyAddr, regAddr, regVal);
    if (ret)
    {
        goto out;
    }

    regAddr  = (switchId << 2) | (REG_ADDR_BIT1_DATA << 1) | (REG_ADDR_BIT0_READ);
    /* Read data [31:16] */
    regVal = 0x0;
    ret = switch_ctrl_mdio_read(phyAddr, regAddr, &regVal);
    if (ret)
    {
        goto out;
    }
    tmpData = (uint32_t)(regVal << 16);

    /* Read data [15:0] */
    regVal = 0x0;
    ret = switch_ctrl_mdio_read(phyAddr, regAddr, &regVal);
    if (!ret)
    {
        tmpData |= regVal;
        *data = tmpData;
    }

out:
    mutex_unlock(&smi_lock);

    return ret;
}

void switch_ctrl_smi_init(void)
{
    mutex_init(&smi_lock);
}