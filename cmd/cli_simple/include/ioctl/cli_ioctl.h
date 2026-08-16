/*
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __CLI_IOCTL_H__
#define __CLI_IOCTL_H__

#include "cli_types.h"
#include "yt_error.h"
#include "yt_types.h"
#include "cli_abstr.h"

extern const cli_abstr_ops_t ioctlOps;

yt_ret_t ioctl_reg_read_regaddr(yt_unit_t unit, uint32_t regAddr, uint32_t *regData);
yt_ret_t ioctl_reg_write_regaddr_value(yt_unit_t unit, uint32_t regAddr, uint32_t regData);
yt_ret_t ioctl_phy_read_port_regmode_regaddr(yt_unit_t unit, yt_port_t port, cli_phy_type_t phy_type, cli_phy_reg_type_t regmode, uint32_t regAddr, uint16_t *regData);
yt_ret_t ioctl_phy_write_port_regmode_regaddr_value(yt_unit_t unit, yt_port_t port, cli_phy_type_t phy_type, cli_phy_reg_type_t regmode, uint32_t regAddr, uint16_t regData);
yt_ret_t ioctl_port_set_port_extifmode(yt_unit_t unit, cli_port_extifmode_params_t extifmode);
yt_ret_t ioctl_port_get_port_extifmode(yt_unit_t unit, cli_get_port_extifmode_params_t *extifmode);
yt_ret_t ioctl_port_get_port_linkstatus(yt_unit_t unit, cli_get_port_linkstatus_params_t *linkstatus); 
yt_ret_t ioctl_port_set_port_auto_neg_state(yt_unit_t unit, cli_set_port_autoneg_params_t autoneg);
yt_ret_t ioctl_port_get_port_auto_neg_state(yt_unit_t unit, cli_get_port_autoneg_params_t *autoneg);
yt_ret_t ioctl_port_set_port_state(yt_unit_t unit, cli_set_port_state_params_t portstate);
yt_ret_t ioctl_port_get_port_state(yt_unit_t unit, cli_get_port_state_params_t *portstate);
yt_ret_t ioctl_port_set_port_duplex_speed(yt_unit_t unit, cli_set_port_speed_duplex_t forcectrl);
yt_ret_t ioctl_port_set_port_fc_state(yt_unit_t unit, cli_set_fc_state_params_t port_an_ability);
yt_ret_t ioctl_vlan_set_port_pvid(yt_unit_t unit, cli_vlan_set_pvid_params_t pvid);
yt_ret_t ioctl_vlan_set_vid_memberports_untagports(yt_unit_t unit, cli_vlan_set_vid_params_t pvid);
yt_ret_t ioctl_mib_set_state(yt_unit_t unit, yt_enable_t state);
yt_ret_t ioctl_mib_get_state(yt_unit_t unit, yt_enable_t *state);
yt_ret_t ioctl_mib_get_port(yt_unit_t unit, cli_mib_get_params_t *mib_get);
yt_ret_t ioctl_mib_get_clear(yt_unit_t unit, cli_mib_clear_params_t mib_clear);
yt_ret_t ioctl_fdb_set_learn_state(yt_unit_t unit, cli_fdb_learn_params_t fdb_learn);
yt_ret_t ioctl_yt_switch_init(void);
yt_ret_t ioctl_yt_switch_exit(yt_unit_t unit);

#endif