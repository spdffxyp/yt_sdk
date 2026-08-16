/*
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "yt_error.h"
#include "yt_types.h"
#include "cli_abstr.h"
#include "cli_intercomm.h"
#include "securec.h"

yt_ret_t
ioctl_reg_read_regaddr(yt_unit_t unit, uint32_t regAddr, uint32_t *regData)
{
    cli_debug_param_t params;
    memset_s(&params, sizeof(params), 0, sizeof(params));

    params.regAddr = regAddr;
    params.regData = regData;
    return cli_build_data_send_recv(unit, &params, sizeof(params), REG_READ_REGADDR_SET);
}

yt_ret_t
ioctl_reg_write_regaddr_value(yt_unit_t unit, uint32_t regAddr, uint32_t regData)
{
    cli_debug_param_t params;
    memset_s(&params, sizeof(params), 0, sizeof(params));

    params.regAddr = regAddr;
    params.regData = &regData;
    return cli_build_data_send_recv(unit, &params, sizeof(params), REG_WRITE_REGADDR_VALUE_SET);
}

yt_ret_t
ioctl_port_set_port_extifmode(yt_unit_t unit, cli_port_extifmode_params_t extifmode)
{
    return cli_build_data_send_recv(unit, &extifmode, sizeof(cli_port_extifmode_params_t), PORT_SET_PORT_EXTIFMODE);
}

yt_ret_t
ioctl_port_get_port_extifmode(yt_unit_t unit, cli_get_port_extifmode_params_t *extifmode)
{
    return cli_build_data_send_recv(unit, extifmode, sizeof(cli_get_port_extifmode_params_t), PORT_GET_PORT_EXTIFMODE);
}

yt_ret_t
ioctl_port_get_port_linkstatus(yt_unit_t unit, cli_get_port_linkstatus_params_t *linkstatus)
{
    return cli_build_data_send_recv(unit, linkstatus, sizeof(cli_get_port_linkstatus_params_t), PORT_GET_PORT_LINKSTATUS);
}

yt_ret_t
ioctl_port_set_port_auto_neg_state(yt_unit_t unit, cli_set_port_autoneg_params_t autoneg)
{
     return cli_build_data_send_recv(unit, &autoneg, sizeof(cli_set_port_autoneg_params_t), PORT_SET_PORT_AUTO_NEG_STATE);
}

yt_ret_t
ioctl_port_get_port_auto_neg_state(yt_unit_t unit, cli_get_port_autoneg_params_t *autoneg)
{
    return cli_build_data_send_recv(unit, autoneg, sizeof(cli_get_port_autoneg_params_t), PORT_GET_PORT_AUTO_NEG_STATE);
}

yt_ret_t
ioctl_port_set_port_state(yt_unit_t unit, cli_set_port_state_params_t portstate)
{
    return cli_build_data_send_recv(unit, &portstate, sizeof(cli_set_port_state_params_t), PORT_SET_PORT_STATE);
}

yt_ret_t
ioctl_port_get_port_state(yt_unit_t unit, cli_get_port_state_params_t *portstate)
{
    return cli_build_data_send_recv(unit, portstate, sizeof(cli_get_port_state_params_t), PORT_GET_PORT_STATE);
}

yt_ret_t
ioctl_port_set_port_duplex_speed(yt_unit_t unit, cli_set_port_speed_duplex_t forcectrl)
{
    return cli_build_data_send_recv(unit, &forcectrl, sizeof(cli_set_port_speed_duplex_t), PORT_SET_PORT_DUPLEX_SPEED);
}

yt_ret_t
ioctl_port_set_port_fc_state(yt_unit_t unit, cli_set_fc_state_params_t port_an_ability)
{
    return cli_build_data_send_recv(unit, &port_an_ability, sizeof(cli_set_fc_state_params_t), PORT_SET_PORT_FC_STATE);
}

yt_ret_t
ioctl_vlan_set_port_pvid(yt_unit_t unit, cli_vlan_set_pvid_params_t pvid)
{
    return cli_build_data_send_recv(unit, &pvid, sizeof(cli_vlan_set_pvid_params_t), VLAN_SET_PORT_PVID);
}

yt_ret_t
ioctl_vlan_set_vid_memberports_untagports(yt_unit_t unit, cli_vlan_set_vid_params_t pvid)
{
    return cli_build_data_send_recv(unit, &pvid, sizeof(cli_vlan_set_vid_params_t), VLAN_SET_VID_MEMBERPORTS_UNTAGPORTS);
}

yt_ret_t
ioctl_mib_set_state(yt_unit_t unit, yt_enable_t state)
{
    return cli_build_data_send_recv(unit, &state, sizeof(yt_enable_t), MIB_SET_STATE);
}

yt_ret_t
ioctl_mib_get_state(yt_unit_t unit, yt_enable_t *state)
{
    return cli_build_data_send_recv(unit, state, sizeof(yt_enable_t), MIB_GET_STATE);
}

yt_ret_t
ioctl_mib_get_port(yt_unit_t unit, cli_mib_get_params_t *mib_get)
{
    return cli_build_data_send_recv(unit, mib_get, sizeof(cli_mib_get_params_t), MIB_GET_PORT);
}

yt_ret_t
ioctl_mib_get_clear(yt_unit_t unit, cli_mib_clear_params_t mib_clear)
{
    return cli_build_data_send_recv(unit, &mib_clear, sizeof(cli_mib_clear_params_t), MIB_GET_CLEAR);
}

yt_ret_t
ioctl_fdb_set_learn_state(yt_unit_t unit, cli_fdb_learn_params_t fdb_learn)
{
    return cli_build_data_send_recv(unit, &fdb_learn, sizeof(cli_fdb_learn_params_t), FDB_LEARN_STATE);
}

yt_ret_t
ioctl_yt_switch_init(void)
{
    yt_unit_t unit = 0;
    return cli_build_data_send_recv(unit, (void *)NULL, 0, SWITCH_INIT);
}

yt_ret_t
ioctl_yt_switch_exit(yt_unit_t unit)
{
    return cli_build_data_send_recv(unit, (void *)NULL, 0, SWITCH_EXIT);
}

yt_ret_t
ioctl_phy_read_port_regmode_regaddr(yt_unit_t unit, yt_port_t port, cli_phy_type_t phy_type, cli_phy_reg_type_t regmode, uint32_t regAddr, uint16_t *regData)
{
    cli_debug_param_t params;
    memset_s(&params, sizeof(params), 0, sizeof(params));

    params.port = port;
    params.regmode = regmode;
    params.regAddr = regAddr;
    params.regDataHalf = regData;
    params.phytype = phy_type;
    return cli_build_data_send_recv(unit, &params, sizeof(params), PHY_REG_REGADDR_READ);
}

yt_ret_t
ioctl_phy_write_port_regmode_regaddr_value(yt_unit_t unit, yt_port_t port, cli_phy_type_t phy_type, cli_phy_reg_type_t regmode, uint32_t regAddr, uint16_t regData)
{
    cli_debug_param_t params;
    memset_s(&params, sizeof(params), 0, sizeof(params));

    params.port = port;
    params.regmode = regmode;
    params.phytype = phy_type;
    params.regAddr = regAddr;
    params.regDataHalf = &regData;
    return cli_build_data_send_recv(unit, &params, sizeof(params), PHY_REG_REGADDR_WRITE);
}

const cli_abstr_ops_t ioctlOps =
{
#ifdef SYS_INCLUDED
    .cli_reg_read_regaddr = ioctl_reg_read_regaddr,
    .cli_reg_write_regaddr_value = ioctl_reg_write_regaddr_value,
#endif
#ifdef PORT_INCLUDED
    .cli_phy_read_port_regmode_regaddr = ioctl_phy_read_port_regmode_regaddr,
    .cli_phy_write_port_regmode_regaddr_value = ioctl_phy_write_port_regmode_regaddr_value,
    .cli_port_set_port_extifmode = ioctl_port_set_port_extifmode,
    .cli_port_get_port_extifmode = ioctl_port_get_port_extifmode,
    .cli_port_get_port_linkstatus = ioctl_port_get_port_linkstatus,
    .cli_port_set_port_auto_neg_state = ioctl_port_set_port_auto_neg_state,
    .cli_port_get_port_auto_neg_state = ioctl_port_get_port_auto_neg_state,
    .cli_port_set_port_state = ioctl_port_set_port_state,
    .cli_port_get_port_state = ioctl_port_get_port_state,
    .cli_port_set_port_duplex_speed = ioctl_port_set_port_duplex_speed,
    .cli_port_set_port_fc_state = ioctl_port_set_port_fc_state,
#endif
#ifdef VLAN_INCLUDED
    .cli_vlan_set_port_pvid = ioctl_vlan_set_port_pvid,
    .cli_vlan_set_vid_memberports_untagports = ioctl_vlan_set_vid_memberports_untagports,
#endif
#ifdef STAT_INCLUDED
    .cli_mib_set_state = ioctl_mib_set_state,
    .cli_mib_get_state = ioctl_mib_get_state,
    .cli_mib_get_port = ioctl_mib_get_port,
    .cli_mib_get_clear = ioctl_mib_get_clear,
#endif
#ifdef L2_INCLUDED
    .cli_fdb_set_learn_state = ioctl_fdb_set_learn_state,
#endif
    .cli_yt_switch_init = ioctl_yt_switch_init,
    .cli_exit = ioctl_yt_switch_exit,
};