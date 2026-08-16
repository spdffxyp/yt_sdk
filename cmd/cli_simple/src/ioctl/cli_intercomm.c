/*
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "cli_intercomm.h"
#include "cli_types.h"
#include "cli_abstr.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "osal_utility.h"
#include "securec.h"

cli_ioctl_t ioctlComm =
{
    .create = ioctl_create,
    .send_recv = ioctl_send_recv,
    .destroy = ioctl_destroy,
};

int32_t ioctl_create(void *arg)
{
    cli_ioctl_t *info = (cli_ioctl_t *)arg;
    int32_t ret;
    ret = osal_open("/dev/cmdDev", O_RDWR);
    if (ret < 0)
    {
        osal_printf("%s %d error: %s(%d)\n", __func__, __LINE__, strerror(errno), errno);
        return CLI_EXE_FAIL;
    }
    info->fd = ret;
    return CLI_EXE_SUCCESS;
}

int32_t ioctl_send_recv(void *arg, cli_ioctl_data_t *data)
{
    cli_ioctl_t *info = (cli_ioctl_t *)arg;
    int32_t ret;
    ret = ioctl(info->fd, CMDIOCTL, data);
    if (ret < 0)
    {
        osal_printf("%s %d error: %s(%d)\n", __func__, __LINE__, strerror(errno), errno);
        return CLI_EXE_FAIL;
    }

    return CLI_EXE_SUCCESS;
}

void ioctl_destroy(void *arg)
{
    cli_ioctl_t *info = (cli_ioctl_t *)arg;

    if (info->fd)
    {
        if (osal_close(info->fd) < 0)
        {
	    osal_printf("%s %d error: %s(%d)\n", __func__, __LINE__, strerror(errno), errno);
        }
    }

}

yt_ret_t
cli_build_data_send_recv(yt_unit_t unit, void * params, uint32_t len, uint8 cmd)
{
    cli_ioctl_t *info = gAbstrInfo.info;
    cli_ioctl_data_t data;
    yt_ret_t ret;

    memset_s(&data, sizeof(data), 0, sizeof(data));

    data.hdr.cmd = cmd;
    data.hdr.unit = unit;
    /* data.tlv[0].tlvhdr.type only valid in generic netlink
    data.tlv[0].tlvhdr.type = CMD_ATTR_DOT1X; */
    data.tlv[0].tlvhdr.len = len;
    data.tlv[0].val = params;

    ret = info->send_recv(info, &data);
    if (ret)
    {
        osal_printf("%s %d send_recv error ret %d\n", __func__, __LINE__, ret);
        return CMM_ERR_FAIL;
    }

    return data.hdr.ret;
}