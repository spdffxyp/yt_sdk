/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************/

#ifndef _CLI_TYPES_H_
#define _CLI_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "yt_types.h"
#include "osal_print.h"
#include "yt_stat.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#define cmd_strtou32(x, y, z)           osal_strtou32((char*)x, (char**)y, z)
#define DECIMAL_STRLEN_MIN 1
#define DECIMAL_STRLEN_MAX 10

struct cli_port_extifmode_params_s
{
    yt_port_t port;
    yt_extif_mode_t extif_mode;
};
typedef struct cli_port_extifmode_params_s  cli_port_extifmode_params_t;

struct cli_get_port_extifmode_params_s
{
    yt_port_t port;
    yt_extif_mode_t extif_mode;
};
typedef struct cli_get_port_extifmode_params_s  cli_get_port_extifmode_params_t;

struct cli_get_port_linkstatus_params_s
{
    yt_port_t port;
    yt_port_linkStatus_all_t linkstatus;
};
typedef struct cli_get_port_linkstatus_params_s  cli_get_port_linkstatus_params_t;

struct cli_set_port_autoneg_params_s
{
    yt_port_t port;
    yt_enable_t state;
};
typedef struct cli_set_port_autoneg_params_s  cli_set_port_autoneg_params_t;

struct cli_get_port_autoneg_params_s
{
    yt_port_t port;
    yt_enable_t state;
};
typedef struct cli_get_port_autoneg_params_s  cli_get_port_autoneg_params_t;

struct cli_set_port_state_params_s
{
    yt_port_t port;
    yt_enable_t state;
};
typedef struct cli_set_port_state_params_s  cli_set_port_state_params_t;

struct cli_get_port_state_params_s
{
    yt_port_t port;
    yt_enable_t state;
};
typedef struct cli_get_port_state_params_s  cli_get_port_state_params_t;

struct cli_set_port_speed_duplex_s
{
    yt_port_t port;
    yt_port_speed_duplex_t speed_duplex;
};
typedef struct cli_set_port_speed_duplex_s  cli_set_port_speed_duplex_t;

struct cli_set_fc_state_params_s
{
    yt_port_t port;
    yt_port_an_ability_t port_ability;
};
typedef struct cli_set_fc_state_params_s cli_set_fc_state_params_t;

struct cli_vlan_set_pvid_params_s
{
    yt_port_t port;
    yt_vlan_t vid;
};
typedef struct cli_vlan_set_pvid_params_s  cli_vlan_set_pvid_params_t;

struct cli_vlan_set_vid_params_s
{
    yt_vlan_t pvid;
    yt_port_mask_t memberports;
    yt_port_mask_t untagports;
};
typedef struct cli_vlan_set_vid_params_s  cli_vlan_set_vid_params_t;

struct cli_mib_get_params_s
{
    yt_port_t port;
    yt_stat_mib_port_cnt_t mib_port_cnt;
};
typedef struct cli_mib_get_params_s  cli_mib_get_params_t;

enum cli_exe_mode_e
{
    PORT,
    ALL,
};
typedef enum cli_exe_mode_e  cli_exe_mode_t;

struct cli_mib_clear_params_s
{
    yt_port_t port;
    cli_exe_mode_t mib_clear_mode;
};
typedef struct cli_mib_clear_params_s  cli_mib_clear_params_t;

struct cli_fdb_learn_params_s
{
    yt_port_t port;
    yt_enable_t state;
};
typedef struct cli_fdb_learn_params_s  cli_fdb_learn_params_t;

enum cli_exe_result_e
{
    CLI_EXE_SUCCESS,
    CLI_EXE_FAIL,
    CLI_EXE_PARA_INVALID,
    CLI_EXE_QUIT,
    CLI_EXE_UNKNOW_COMMAND,
    CLI_EXE_NOT_SUPPORT,
    CLI_ABSTR_INIT_FAIL,
};
typedef enum cli_exe_result_e cli_exe_result_t;

enum cli_phy_reg_type_e
{
    MII,
    EXT,
};
typedef enum cli_phy_reg_type_e cli_phy_reg_type_t;

enum cli_phy_type_e
{
    INT_PHY,
    EXT_PHY,
};
typedef enum cli_phy_type_e cli_phy_type_t;


#ifdef __cplusplus
}
#endif

#endif // _CLI_TYPES_H_
