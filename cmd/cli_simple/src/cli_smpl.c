/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "cli_smpl.h"
#include "yt_vlan.h"
#include "yt_sys.h"
#include "yt_port.h"
#include "yt_init.h"
#include "cli_types.h"
#include "yt_l2.h"
#include "yt_exit.h"
#include "cli_parser.h"
#include "fal_dispatch.h"
#include "cal_cmm.h"
#include "hal_ctrl.h"
#include "phy_drv.h"
#include "securec.h"

#ifdef SYS_INCLUDED
yt_ret_t
smpl_reg_read_regaddr(yt_unit_t unit, uint32_t regAddr, uint32_t *regData)
{
    if (yt_sys_register_value_get(unit, regAddr, regData))
    {
        return CMM_ERR_FAIL;
    }

    return CMM_ERR_OK;
}

yt_ret_t
smpl_reg_write_regaddr_value(yt_unit_t unit, uint32_t regAddr, uint32_t regData)
{
    if (yt_sys_register_value_set(unit, regAddr, regData))
    {
        return CMM_ERR_FAIL;
    } 

    return CMM_ERR_OK;
}
#endif

#ifdef PORT_INCLUDED
 yt_ret_t
smpl_phy_read_port_regmode_regaddr(yt_unit_t unit, yt_port_t port, cli_phy_type_t phy_type, cli_phy_reg_type_t regmode, uint32_t regAddr, uint16_t *regData)
{
    yt_ret_t ret = CMM_ERR_OK;
    yt_port_t phy_addr = 0;

    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    if (MII == regmode)
    {
        if (INT_PHY == phy_type)
        {
            ret = yt_port_phy_reg_get(unit, port, regAddr, regData, PHY_INTERNAL);
        }
        else if (EXT_PHY == phy_type)
        {
             ret = yt_port_phy_reg_get(unit, port, regAddr, regData, PHY_EXTERNAL);
        }
    }
    else if (EXT == regmode)
    {
        phy_addr = CAL_YTP_TO_PHYADDR(unit, port);
        if (phy_addr == INVALID_ID || !HALSWDRV(unit))
        {
            return CMM_ERR_FAIL;
        }

        if (INT_PHY == phy_type)
        {
             ret = yt_port_phy_reg_set(unit, port, 0x1e, regAddr, PHY_INTERNAL);
             ret = yt_port_phy_reg_get(unit, port, 0x1f, regData, PHY_INTERNAL);
        }
        else if (EXT_PHY == phy_type)
        {
             ret = yt_port_phy_reg_set(unit, port, 0x1e, regAddr, PHY_EXTERNAL);
             ret = yt_port_phy_reg_get(unit, port, 0x1f, regData, PHY_EXTERNAL);
        }
    }
    if (ret != CMM_ERR_OK)
    {
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t
smpl_phy_write_port_regmode_regaddr_value(yt_unit_t unit, yt_port_t port, cli_phy_type_t phy_type, cli_phy_reg_type_t regmode, uint32_t regAddr, uint16_t regData)
{
    yt_ret_t ret = CMM_ERR_OK;
    yt_port_t phy_addr = 0;

    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);

    if (MII == regmode)
    {
        if (INT_PHY == phy_type)
        {
            ret = yt_port_phy_reg_set(unit, port, regAddr, regData, PHY_INTERNAL);
        }
        else if (EXT_PHY == phy_type)
        {
            ret = yt_port_phy_reg_set(unit, port, regAddr, regData, PHY_EXTERNAL);
        }

    }
    else if (EXT == regmode)
    {
        phy_addr = CAL_YTP_TO_PHYADDR(unit, port);
        if (phy_addr == INVALID_ID || !HALSWDRV(unit))
        {
            return CMM_ERR_FAIL;
        }

        if (INT_PHY == phy_type)
        {
            ret = yt_port_phy_reg_set(unit, port, 0x1e, regAddr, PHY_INTERNAL);
            ret = yt_port_phy_reg_set(unit, port, 0x1f, regData, PHY_INTERNAL);
        }
        else if (EXT_PHY == phy_type)
        {
            ret = yt_port_phy_reg_set(unit, port, 0x1e, regAddr, PHY_EXTERNAL);
            ret = yt_port_phy_reg_set(unit, port, 0x1f, regData, PHY_EXTERNAL);
        }
    }

    if (ret != CMM_ERR_OK)
    {
        return CMM_ERR_FAIL;
    }

    return CMM_ERR_OK;
}

yt_ret_t
smpl_port_set_port_extifmode(yt_unit_t unit, cli_port_extifmode_params_t extifmode)
{
    if (yt_port_extif_mode_set(unit, extifmode.port, extifmode.extif_mode))
    {
        cli_print_out("set port extif mode failed. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t
smpl_port_get_port_extifmode(yt_unit_t unit, cli_get_port_extifmode_params_t *extifmode)
{
    if (yt_port_extif_mode_get(unit, extifmode->port, &extifmode->extif_mode))
    {
        cli_print_out("get port extif mode failed. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t
smpl_port_get_port_linkstatus(yt_unit_t unit, cli_get_port_linkstatus_params_t *linkstatus)
{
    if (yt_port_link_status_all_get(unit, linkstatus->port, &linkstatus->linkstatus))
    {
        cli_print_out("get link status failed. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t
smpl_port_set_port_auto_neg_state(yt_unit_t unit, cli_set_port_autoneg_params_t autoneg)
{
    if (yt_port_phyAutoNeg_enable_set(unit, autoneg.port, autoneg.state))
    {
        cli_print_out("set phyAutoNeg failed. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t
smpl_port_get_port_auto_neg_state(yt_unit_t unit, cli_get_port_autoneg_params_t *autoneg)
{
    if (yt_port_phyAutoNeg_enable_get(unit, autoneg->port, &autoneg->state))
    {
        cli_print_out("get phyAutoNeg status failed. \n");
        return CMM_ERR_FAIL;
    }
    
    return CMM_ERR_OK;
}

yt_ret_t
smpl_port_set_port_state(yt_unit_t unit, cli_set_port_state_params_t portstate)
{
    if (yt_port_enable_set(unit, portstate.port, portstate.state))
    {
        cli_print_out("set port status failed. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t
smpl_port_get_port_state(yt_unit_t unit, cli_get_port_state_params_t *portstate)
{
    if (yt_port_enable_get(unit, portstate->port, &portstate->state))
    {
        cli_print_out("get port status failed. \n");
        return CMM_ERR_FAIL;
    }

    return CMM_ERR_OK;
}

yt_ret_t
smpl_port_set_port_duplex_speed(yt_unit_t unit, cli_set_port_speed_duplex_t forcectrl)
{
    yt_enable_t state = YT_DISABLE;
    yt_port_an_ability_t port_an_ability;
    memset_s (&port_an_ability, sizeof(yt_port_an_ability_t), 0, sizeof(yt_port_an_ability_t));
    if (yt_port_phyAutoNeg_enable_get(unit, forcectrl.port, &state))
    {
        cli_print_out("get phyAutoNeg failed.\n");
        return CMM_ERR_FAIL;
    }
    if (YT_ENABLE == state)
    {
        if (yt_port_phyAutoNeg_ability_get(unit, forcectrl.port, &port_an_ability))
        {
            cli_print_out("get phyAutoNeg_ability failed.\n");
            return CMM_ERR_FAIL;
        }
        port_an_ability.full_1000_en = FALSE;
        port_an_ability.full_100_en = FALSE;
        port_an_ability.full_10_en = FALSE;
        port_an_ability.half_100_en = FALSE;
        port_an_ability.half_10_en = FALSE;
        switch(forcectrl.speed_duplex)
        {
            case PORT_SPEED_DUP_10HALF:
                port_an_ability.half_10_en = TRUE;
                break;
            case PORT_SPEED_DUP_10FULL:
                port_an_ability.full_10_en = TRUE;
                break;
            case PORT_SPEED_DUP_100HALF:
                port_an_ability.half_100_en = TRUE;
                break;
            case PORT_SPEED_DUP_100FULL:
                port_an_ability.full_100_en = TRUE;
                break;
            case PORT_SPEED_DUP_1000FULL:
                port_an_ability.full_1000_en = TRUE;
                break;
            default:
                cli_print_out("not support speed and duplex. \n");
                break;
        }
        if (yt_port_phyAutoNeg_ability_set(unit, forcectrl.port, port_an_ability))
        {
            cli_print_out(" speed and duplex set failed. \n");
            return CMM_ERR_FAIL;
        }
    }
    else
    {
        if (yt_port_phy_force_set(unit, forcectrl.port, forcectrl.speed_duplex))
        {
            cli_print_out(" speed and duplex set failed. \n");
            return CMM_ERR_FAIL;
        }
    }

    return CMM_ERR_OK;
}

yt_ret_t
smpl_port_set_port_fc_state(yt_unit_t unit, cli_set_fc_state_params_t port_an_ability)
{
    yt_port_an_ability_t pability;
    memset_s (&pability, sizeof(yt_port_an_ability_t), 0, sizeof(yt_port_an_ability_t));
    if (yt_port_phyAutoNeg_ability_get(unit, port_an_ability.port, &pability))
    {
        cli_print_out("get port ability failed.\n");
        return CMM_ERR_FAIL;
    }
    pability.asyFC_en = port_an_ability.port_ability.asyFC_en;
    pability.fc_en = port_an_ability.port_ability.fc_en;
    if (yt_port_phyAutoNeg_ability_set(unit, port_an_ability.port, pability))
    {
        cli_print_out("set port ability failed. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}
#endif

#ifdef VLAN_INCLUDED
yt_ret_t
smpl_vlan_set_port_pvid(yt_unit_t unit, cli_vlan_set_pvid_params_t pvid)
{
    if (yt_vlan_port_igrPvid_set(unit, VLAN_TYPE_CVLAN, pvid.port, pvid.vid) ||yt_vlan_port_igrPvid_set(unit, VLAN_TYPE_SVLAN, pvid.port, pvid.vid))
    {
        cli_print_out("the pvid set failed. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t
smpl_vlan_set_vid_memberports_untagports(yt_unit_t unit, cli_vlan_set_vid_params_t pvid)
{
    if (yt_vlan_port_set(unit, pvid.pvid, pvid.memberports, pvid.untagports))
    {
        cli_print_out("set vlan port is failed. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}
#endif

#ifdef STAT_INCLUDED
yt_ret_t
smpl_mib_set_state(yt_unit_t unit, yt_enable_t state)
{
    if (yt_stat_mib_enable_set(unit, state))
    {
        cli_print_out("mib state set fail. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t
smpl_mib_get_state(yt_unit_t unit, yt_enable_t *state)
{
    if (yt_stat_mib_enable_get(unit, state))
    {
        cli_print_out("mib state get fail. \n");
        return CMM_ERR_FAIL;
    }
    return CMM_ERR_OK;
}

yt_ret_t
smpl_mib_get_port(yt_unit_t unit, cli_mib_get_params_t *mib_get)
{
    if (yt_stat_mib_port_get(unit, mib_get->port, &mib_get->mib_port_cnt))
    {
        cli_print_out("get port mib failed. \n");
        return CMM_ERR_FAIL;
    }
    
    return CMM_ERR_OK;
}

yt_ret_t
smpl_mib_get_clear(yt_unit_t unit, cli_mib_clear_params_t mib_clear)
{
    if (mib_clear.mib_clear_mode == ALL)
    {
        if (yt_stat_mib_clear_all(unit))
        {
            cli_print_out("clear all mib failed. \n");
            return CMM_ERR_FAIL;
        }
    }
    else if (mib_clear.mib_clear_mode == PORT)
    {
        if (yt_stat_mib_clear(unit, mib_clear.port))
        {
            cli_print_out("the port para is invalid. \n");
            return CMM_ERR_FAIL;
        }
    }
    return CMM_ERR_OK;
}
#endif

#ifdef L2_INCLUDED
yt_ret_t
smpl_fdb_set_learn_state(yt_unit_t unit, cli_fdb_learn_params_t fdb_learn)
{
    if (yt_l2_port_learn_en_set(unit, fdb_learn.port, fdb_learn.state))
    {
        cli_print_out("fdb learn failed. \n");
        return CMM_ERR_FAIL;
    }
    
    return CMM_ERR_OK;
}
#endif

yt_ret_t
smpl_yt_switch_init()
{
    yt_ret_t ret;
    ret = yt_init();
    return ret;
}

yt_ret_t
smpl_yt_switch_exit(yt_unit_t unit)
{
    yt_ret_t ret;
    ret = yt_exit(unit);
    return ret;
}

const cli_abstr_ops_t smplOps =
{
#ifdef SYS_INCLUDED
    .cli_reg_read_regaddr = smpl_reg_read_regaddr,
    .cli_reg_write_regaddr_value = smpl_reg_write_regaddr_value,
#endif
#ifdef PORT_INCLUDED
    .cli_phy_read_port_regmode_regaddr = smpl_phy_read_port_regmode_regaddr,
    .cli_phy_write_port_regmode_regaddr_value = smpl_phy_write_port_regmode_regaddr_value,
    .cli_port_set_port_extifmode = smpl_port_set_port_extifmode,
    .cli_port_get_port_extifmode = smpl_port_get_port_extifmode,
    .cli_port_get_port_linkstatus = smpl_port_get_port_linkstatus,
    .cli_port_set_port_auto_neg_state = smpl_port_set_port_auto_neg_state,
    .cli_port_get_port_auto_neg_state = smpl_port_get_port_auto_neg_state,
    .cli_port_set_port_state = smpl_port_set_port_state,
    .cli_port_get_port_state = smpl_port_get_port_state,
    .cli_port_set_port_duplex_speed = smpl_port_set_port_duplex_speed,
    .cli_port_set_port_fc_state = smpl_port_set_port_fc_state,
#endif
#ifdef VLAN_INCLUDED
    .cli_vlan_set_port_pvid = smpl_vlan_set_port_pvid,
    .cli_vlan_set_vid_memberports_untagports = smpl_vlan_set_vid_memberports_untagports,
#endif
#ifdef STAT_INCLUDED
    .cli_mib_set_state = smpl_mib_set_state,
    .cli_mib_get_state = smpl_mib_get_state,
    .cli_mib_get_port = smpl_mib_get_port,
    .cli_mib_get_clear = smpl_mib_get_clear,
#endif
#ifdef L2_INCLUDED
    .cli_fdb_set_learn_state = smpl_fdb_set_learn_state,
#endif
    .cli_yt_switch_init = smpl_yt_switch_init,
    .cli_exit = smpl_yt_switch_exit,
};
