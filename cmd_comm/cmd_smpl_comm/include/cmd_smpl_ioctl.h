/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __CMD_SMPL_IOCTL_H__
#define __CMD_SMPL_IOCTL_H__

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include "ioctl_share.h"

struct cmd_dev_s
{
    struct cdev cdev;
    dev_t devno;
    struct device *dev;
    struct class *cls;
};
typedef struct cmd_dev_s cmd_dev_t;

struct intercomm_cmd_ops_s
{
    uint16_t cmd;
    int32_t (*ops)(cli_ioctl_data_t *data);
};
typedef struct intercomm_cmd_ops_s intercomm_cmd_ops_t;

int32_t __init cmd_cdev_init(void);
void __exit cmd_cdev_exit(void);
const intercomm_cmd_ops_t *intercomm_find_ops(uint16_t cmd);

#endif // __CMD_SMPL_IOCTL_H__