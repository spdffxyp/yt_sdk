/*
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#ifndef __CLI_ABSTR_H__
#define __CLI_ABSTR_H__

#include "yt_types.h"
#include "cli_types.h"
#if defined(CMD_SMPL_ACCESS_SDK_IOCTL)
#include "cli_intercomm.h"
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

struct cli_abstr_ops_s
{
    yt_ret_t (*cli_reg_read_regaddr)(yt_unit_t unit, uint32_t, uint32_t *);
    yt_ret_t (*cli_reg_write_regaddr_value)(yt_unit_t unit, uint32_t, uint32_t);
    yt_ret_t (*cli_phy_read_port_regmode_regaddr)(yt_unit_t unit, yt_port_t port, cli_phy_type_t phy_type, cli_phy_reg_type_t regmode, uint32_t regAddr, uint16_t *regData);
    yt_ret_t (*cli_phy_write_port_regmode_regaddr_value)(yt_unit_t unit, yt_port_t port, cli_phy_type_t phy_type, cli_phy_reg_type_t regmode, uint32_t regAddr, uint16_t regData);
    yt_ret_t (*cli_port_set_port_extifmode)(yt_unit_t unit, cli_port_extifmode_params_t);
    yt_ret_t (*cli_port_get_port_extifmode)(yt_unit_t unit, cli_get_port_extifmode_params_t *);
    yt_ret_t (*cli_port_get_port_linkstatus)(yt_unit_t unit, cli_get_port_linkstatus_params_t *);
    yt_ret_t (*cli_port_set_port_auto_neg_state)(yt_unit_t unit, cli_set_port_autoneg_params_t);
    yt_ret_t (*cli_port_get_port_auto_neg_state)(yt_unit_t unit, cli_get_port_autoneg_params_t *);
    yt_ret_t (*cli_port_set_port_state)(yt_unit_t unit, cli_set_port_state_params_t);
    yt_ret_t (*cli_port_get_port_state)(yt_unit_t unit, cli_get_port_state_params_t *);
    yt_ret_t (*cli_port_set_port_duplex_speed)(yt_unit_t unit, cli_set_port_speed_duplex_t);
    yt_ret_t (*cli_port_set_port_fc_state)(yt_unit_t unit, cli_set_fc_state_params_t);
    yt_ret_t (*cli_vlan_set_port_pvid)(yt_unit_t unit, cli_vlan_set_pvid_params_t);
    yt_ret_t (*cli_vlan_set_vid_memberports_untagports)(yt_unit_t unit, cli_vlan_set_vid_params_t);
    yt_ret_t (*cli_mib_set_state)(yt_unit_t unit, yt_enable_t);
    yt_ret_t (*cli_mib_get_state)(yt_unit_t unit, yt_enable_t *);
    yt_ret_t (*cli_mib_get_port)(yt_unit_t unit, cli_mib_get_params_t *);
    yt_ret_t (*cli_mib_get_clear)(yt_unit_t unit, cli_mib_clear_params_t);
    yt_ret_t (*cli_fdb_set_learn_state)(yt_unit_t unit, cli_fdb_learn_params_t);
    yt_ret_t (*cli_yt_switch_init)();
    yt_ret_t (*cli_exit)(yt_unit_t unit);
};
typedef struct cli_abstr_ops_s  cli_abstr_ops_t;

enum cli_abstr_id_e
{
    CLI_SMPL,
    CLI_IOCTL,
};
typedef enum cli_abstr_id_e cli_abstr_id_t;

struct cli_abstr_s
{
    cli_abstr_id_t id;
    const cli_abstr_ops_t *ops;
};
typedef struct cli_abstr_s cli_abstr_t;

struct cli_abstr_info_s
{
    const cli_abstr_t *abstr;
#if defined(CMD_SMPL_ACCESS_SDK_IOCTL)
    cli_ioctl_t *info;
#endif
};
typedef struct cli_abstr_info_s cli_abstr_info_t;

extern cli_abstr_info_t gAbstrInfo;
#define CLI_ABSTR_OPS gAbstrInfo.abstr->ops

void cli_abstr_exit(void);
int32_t cli_abstr_init(void);

#endif // _CLI_ABSTR_H_