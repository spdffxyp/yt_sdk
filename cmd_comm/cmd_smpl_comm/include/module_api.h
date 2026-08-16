/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __MODULE_API_H__
#define __MODULE_API_H__
#include "securec.h"
#include "cli_types.h"
#include "ioctl_share.h"

int32_t reg_read_regaddr(cli_ioctl_data_t *data);
int32_t reg_write_regaddr_value(cli_ioctl_data_t *data);
int32_t phy_reg_read_regaddr(cli_ioctl_data_t *data);
int32_t phy_reg_write_regaddr(cli_ioctl_data_t *data);
int32_t port_set_port_extifmode(cli_ioctl_data_t *data);
int32_t port_get_port_extifmode(cli_ioctl_data_t *data);
int32_t port_get_port_linkstatus(cli_ioctl_data_t *data); 
int32_t port_set_port_auto_neg_state(cli_ioctl_data_t *data);
int32_t port_get_port_auto_neg_state(cli_ioctl_data_t *data);
int32_t port_set_port_state(cli_ioctl_data_t *data);
int32_t port_get_port_state(cli_ioctl_data_t *data);
int32_t port_set_port_duplex_speed(cli_ioctl_data_t *data);
int32_t port_set_port_fc_state(cli_ioctl_data_t *data);
int32_t vlan_set_port_pvid(cli_ioctl_data_t *data);
int32_t vlan_set_vid_memberports_untagports(cli_ioctl_data_t *data);
int32_t mib_set_state(cli_ioctl_data_t *data);
int32_t mib_get_state(cli_ioctl_data_t *data);
int32_t mib_get_port(cli_ioctl_data_t *data);
int32_t mib_get_clear(cli_ioctl_data_t *data);
int32_t fdb_set_learn_state(cli_ioctl_data_t *data);
int32_t intercomm_switch_init(cli_ioctl_data_t *data);
int32_t intercomm_switch_exit(cli_ioctl_data_t *data);

#endif // __MODULE_API_H__