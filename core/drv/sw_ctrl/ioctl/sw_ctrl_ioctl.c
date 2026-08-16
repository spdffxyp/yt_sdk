/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "yt_error.h"
#include "yt_types.h"
#include "osal_print.h"
#include "osal_utility.h"
#include "sw_ctrl_ioctl.h"
#include "sw_ctrl_comm_share.h"

static int32_t fd;

int32_t sw_ctrl_ioctl_init(void)
{
    int32_t ret;

    ret = osal_open("/dev/swCtrlDev", O_RDWR);
    if (ret < 0)
    {
        osal_printf("%s %d error: %s(%d)\n", __func__, __LINE__, strerror(errno), errno);
        return CMM_ERR_FAIL;
    }

    fd = ret;

    return CMM_ERR_OK;
}

static int32_t sw_ctrl_ioctl_send_recv(sw_ctrl_comm_data_t *data)
{
    int32_t ret;

    ret = ioctl(fd, SWACCESSIOCTL, data);
    if (ret < 0)
    {
        osal_printf("%s %d error: %s(%d)\n", __func__, __LINE__, strerror(errno), errno);
        return CMM_ERR_FAIL;
    }

    return CMM_ERR_OK;
}

uint32_t sw_ctrl_ioctl_write(uint8_t unit, uint32_t regAddr, uint32_t regValue)
{
    sw_ctrl_comm_data_t data;
    int32_t ret;

    data.regAddr = regAddr;
    data.regVal = regValue;
    data.unit = unit;
    data.flags = SW_CTRL_FLAG_WRITE;
    ret = sw_ctrl_ioctl_send_recv(&data);
    if (ret)
    {
        osal_printf("%s write fail, ret %d addr 0x%08x value 0x%08x\n", __func__, ret, regAddr, regValue);
        return ret;
    }

    return CMM_ERR_OK;
}

uint32_t sw_ctrl_ioctl_read(uint8_t unit, uint32_t regAddr, uint32_t *pRegVale)
{
    sw_ctrl_comm_data_t data;
    int32_t ret;

    data.regAddr = regAddr;
    data.unit = unit;
    data.flags = SW_CTRL_FLAG_READ;
    ret = sw_ctrl_ioctl_send_recv(&data);
    if (ret)
    {
        osal_printf("%s read fail, ret %d addr 0x%08x\n", __func__, ret, regAddr);
        return ret;
    }

    *pRegVale = data.regVal;

    return CMM_ERR_OK;
}



void sw_ctrl_ioctl_exit(void)
{
    if (fd)
    {
        if (osal_close(fd) < 0)
        {
		    osal_printf("%s %d error: %s(%d)\n", __func__, __LINE__, strerror(errno), errno);
        }
    }

}