/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __CLI_INTERCOMM_H__
#define __CLI_INTERCOMM_H__
#include "ioctl_share.h"

struct cli_ioctl_s {
    int32_t fd;
    int32_t (*create)(void *arg);
    int32_t (*send_recv)(void *arg, cli_ioctl_data_t *data);
    void (*destroy)(void *arg);
};
typedef struct cli_ioctl_s cli_ioctl_t;


extern cli_ioctl_t ioctlComm;

int32_t ioctl_create(void *arg);
int32_t ioctl_send_recv(void *arg, cli_ioctl_data_t *data);
void ioctl_destroy(void *arg);

yt_ret_t cli_build_data_send_recv(yt_unit_t unit, void * params, uint32_t len, uint8 cmd);

#endif // __CLI_INTERCOMM_H__

