/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "module_api.h"
#include "yt_vlan.h"
#include "yt_sys.h"
#include "yt_l2.h"
#include "yt_init.h"
#include "yt_exit.h"
#include "fal_dispatch.h"
#include "cal_cmm.h"
#include "phy_drv.h"
#include "hal_ctrl.h"

#ifdef SYS_INCLUDED
int32_t
reg_read_regaddr(cli_ioctl_data_t *data)
{
    cli_debug_param_t *params;
    cli_ioctl_tlv_t *tlv;

    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u \n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    data->hdr.ret = yt_sys_register_value_get(data->hdr.unit, params->regAddr, params->regData);

    return CMM_ERR_OK;
}

int32_t
reg_write_regaddr_value(cli_ioctl_data_t *data)
{
    cli_debug_param_t *params;
    cli_ioctl_tlv_t *tlv;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    data->hdr.ret = yt_sys_register_value_set(data->hdr.unit, params->regAddr, *(params->regData));

    return CMM_ERR_OK;
}
#endif

#ifdef PORT_INCLUDED
int32_t
phy_reg_read_regaddr(cli_ioctl_data_t *data)
{
    cli_debug_param_t *params;
    yt_port_t phy_addr = 0;
    cli_ioctl_tlv_t *tlv;

    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;

    CMM_PARAM_CHK((YT_UNIT_NUM <= data->hdr.unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(data->hdr.unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(data->hdr.unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(data->hdr.unit, params->port))), CMM_ERR_PORT);

    if (MII == params->regmode)
    {
        if (INT_PHY == params->phytype)
        {
            data->hdr.ret = yt_port_phy_reg_get(data->hdr.unit, params->port, params->regAddr, params->regDataHalf, PHY_INTERNAL);
        }
        else if (EXT_PHY == params->phytype)
        {
             data->hdr.ret = yt_port_phy_reg_get(data->hdr.unit, params->port, params->regAddr, params->regDataHalf, PHY_EXTERNAL);
        }
    }
    else if (EXT == params->regmode)
    {
        phy_addr = CAL_YTP_TO_PHYADDR(data->hdr.unit, params->port);
        if (phy_addr == INVALID_ID || !HALSWDRV(data->hdr.unit))
        {
            return CMM_ERR_FAIL;
        }

        if (INT_PHY == params->phytype)
        {
             data->hdr.ret = yt_port_phy_reg_set(data->hdr.unit, params->port, 0x1e, params->regAddr, PHY_INTERNAL);
             data->hdr.ret = yt_port_phy_reg_get(data->hdr.unit, params->port, 0x1f, params->regDataHalf, PHY_INTERNAL);
        }
        else if (EXT_PHY == params->phytype)
        {
             data->hdr.ret = yt_port_phy_reg_set(data->hdr.unit, params->port, 0x1e, params->regAddr, PHY_EXTERNAL);
             data->hdr.ret = yt_port_phy_reg_get(data->hdr.unit, params->port, 0x1f, params->regDataHalf, PHY_EXTERNAL);
        }
    }
    else
    {
        printk("err regmode. \n");
    }
    return CMM_ERR_OK;
}

int32_t
phy_reg_write_regaddr(cli_ioctl_data_t *data)
{
    cli_debug_param_t *params;
    cli_ioctl_tlv_t *tlv;
    yt_port_t phy_addr = 0;

    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
	
    CMM_PARAM_CHK((YT_UNIT_NUM <= data->hdr.unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(data->hdr.unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(data->hdr.unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(data->hdr.unit, params->port))), CMM_ERR_PORT);

    if (MII == params->regmode)
    {
        if (INT_PHY == params->phytype)
        {
            data->hdr.ret = yt_port_phy_reg_set(data->hdr.unit, params->port, params->regAddr, *(params->regDataHalf), PHY_INTERNAL);
        }
        else if (EXT_PHY == params->phytype)
        {
            data->hdr.ret = yt_port_phy_reg_set(data->hdr.unit, params->port, params->regAddr, *(params->regDataHalf), PHY_EXTERNAL);
        }

    }
    else if (EXT == params->regmode)
    {
        phy_addr = CAL_YTP_TO_PHYADDR(data->hdr.unit, params->port);
        if (phy_addr == INVALID_ID || !HALSWDRV(data->hdr.unit))
        {
            return CMM_ERR_FAIL;
        }

        if (INT_PHY == params->phytype)
        {
            data->hdr.ret = yt_port_phy_reg_set(data->hdr.unit, params->port, 0x1e, params->regAddr, PHY_INTERNAL);
            data->hdr.ret = yt_port_phy_reg_set(data->hdr.unit, params->port, 0x1f, *(params->regDataHalf), PHY_INTERNAL);
        }
        else if (EXT_PHY == params->phytype)
        {
            data->hdr.ret = yt_port_phy_reg_set(data->hdr.unit, params->port, 0x1e, params->regAddr, PHY_EXTERNAL);
            data->hdr.ret = yt_port_phy_reg_set(data->hdr.unit, params->port, 0x1f, *(params->regDataHalf), PHY_EXTERNAL);
        }

    }
    return CMM_ERR_OK;
}

int32_t
port_set_port_extifmode(cli_ioctl_data_t *data)
{
    cli_port_extifmode_params_t *params;
    cli_ioctl_tlv_t *tlv;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_port_extif_mode_set(data->hdr.unit, params->port, params->extif_mode))
    {
        printk("set port extif mode failed. \n");
        return CMM_ERR_FAIL;
    }    
    //done

    return CMM_ERR_OK;
}

int32_t
port_get_port_extifmode(cli_ioctl_data_t *data)
{
    cli_get_port_extifmode_params_t *params;
    cli_ioctl_tlv_t *tlv;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_port_extif_mode_get(data->hdr.unit, params->port, &params->extif_mode))
    {
        printk("get port extif mode failed. \n");
        return CMM_ERR_FAIL;
    }
    //done

    return CMM_ERR_OK;
}

int32_t
port_get_port_linkstatus(cli_ioctl_data_t *data)
{
    cli_get_port_linkstatus_params_t *params;
    cli_ioctl_tlv_t *tlv;

    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_port_link_status_all_get(data->hdr.unit, params->port, &params->linkstatus))
    {
        printk("get link status failed. \n");
        return CMM_ERR_FAIL;
    }
    //done

    return CMM_ERR_OK;
}

int32_t
port_set_port_auto_neg_state(cli_ioctl_data_t *data)
{
    cli_set_port_autoneg_params_t *params;
    cli_ioctl_tlv_t *tlv;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u \n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_port_phyAutoNeg_enable_set(data->hdr.unit, params->port, params->state))
    {
        printk("set macAutoNeg failed. \n");
        return CMM_ERR_FAIL;
    }    
    //done

    return CMM_ERR_OK;
}

int32_t
port_get_port_auto_neg_state(cli_ioctl_data_t *data)
{
    cli_get_port_autoneg_params_t *params;
    cli_ioctl_tlv_t *tlv;

    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u \n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_port_phyAutoNeg_enable_get(data->hdr.unit, params->port, &params->state))
    {
        printk("get phyAutoNeg status failed. \n");
        return CMM_ERR_FAIL;
    }
    //done

    return CMM_ERR_OK;
}

int32_t
port_set_port_state(cli_ioctl_data_t *data)
{
    cli_set_port_state_params_t *params;
    cli_ioctl_tlv_t *tlv;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }
    params = tlv->val;
    //call back
    if (yt_port_enable_set(data->hdr.unit, params->port, params->state))
    {
        printk("set port status failed. \n");
        return CMM_ERR_FAIL;
    }    
    //done
    return CMM_ERR_OK;
}

int32_t
port_get_port_state(cli_ioctl_data_t *data)
{
    cli_get_port_state_params_t *params;
    cli_ioctl_tlv_t *tlv;
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_port_enable_get(data->hdr.unit, params->port, &params->state))
    {
        printk("get port status failed. \n");
        return CMM_ERR_FAIL;
    }
    //done

    return CMM_ERR_OK;
}

int32_t
port_set_port_duplex_speed(cli_ioctl_data_t *data)
{
    cli_set_port_speed_duplex_t *params;
    cli_ioctl_tlv_t *tlv;
    yt_enable_t state;
    yt_port_an_ability_t port_an_ability;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    memset_s (&port_an_ability, sizeof(yt_port_an_ability_t), 0, sizeof(yt_port_an_ability_t));
    if (yt_port_phyAutoNeg_enable_get(data->hdr.unit, params->port, &state))
    {
        printk("get phyAutoNeg failed.\n");
        return CMM_ERR_FAIL;
    }
    if (YT_ENABLE == state)
    {
        if (yt_port_phyAutoNeg_ability_get(data->hdr.unit, params->port, &port_an_ability))
        {
            printk("get phyAutoNeg_ability failed.\n");
            return CMM_ERR_FAIL;
        }
        port_an_ability.full_1000_en = FALSE;
        port_an_ability.full_100_en = FALSE;
        port_an_ability.full_10_en = FALSE;
        port_an_ability.half_100_en = FALSE;
        port_an_ability.half_10_en = FALSE;
        switch(params->speed_duplex)
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
                printk("not support speed and duplex. \n");
                break;
        }
        if (yt_port_phyAutoNeg_ability_set(data->hdr.unit, params->port, port_an_ability))
        {
            printk(" speed and duplex set failed. \n");
            return CMM_ERR_FAIL;
        }
    }
    else
    {
        if (yt_port_phy_force_set(data->hdr.unit, params->port, params->speed_duplex))
        {
            printk(" speed and duplex set failed. \n");
            return CMM_ERR_FAIL;
        }
    }
    //done

    return CMM_ERR_OK;
}

int32_t
port_set_port_fc_state(cli_ioctl_data_t *data)
{
    cli_set_fc_state_params_t *params;
    yt_port_an_ability_t pability;
    cli_ioctl_tlv_t *tlv;

    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    memset_s (&pability, sizeof(yt_port_an_ability_t), 0, sizeof(yt_port_an_ability_t));
    if (yt_port_phyAutoNeg_ability_get(data->hdr.unit, params->port, &pability))
    {
        printk("get port ability failed.\n");
        return CMM_ERR_FAIL;
    }
    pability.asyFC_en = params->port_ability.asyFC_en;
    pability.fc_en = params->port_ability.fc_en;
    if (yt_port_phyAutoNeg_ability_set(data->hdr.unit, params->port, pability))
    {
        printk("set port ability failed. \n");
        return CMM_ERR_FAIL;
    }
    //done

    return CMM_ERR_OK;
}
#endif

#ifdef VLAN_INCLUDED
int32_t
vlan_set_port_pvid(cli_ioctl_data_t *data)
{
    cli_vlan_set_pvid_params_t *params;
    cli_ioctl_tlv_t *tlv;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_vlan_port_igrPvid_set(data->hdr.unit, VLAN_TYPE_CVLAN, params->port, params->vid) ||yt_vlan_port_igrPvid_set(data->hdr.unit, VLAN_TYPE_SVLAN, params->port, params->vid))
    {
        printk("the pvid set failed. \n");
        return CMM_ERR_FAIL;
    }    
    //done

    return CMM_ERR_OK;
}

int32_t
vlan_set_vid_memberports_untagports(cli_ioctl_data_t *data)
{
    cli_vlan_set_vid_params_t *params;
    cli_ioctl_tlv_t *tlv;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u \n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_vlan_port_set(data->hdr.unit, params->pvid , params->memberports , params->untagports))
    {
        printk("set vlan port is failed. \n");
        return CMM_ERR_FAIL;
    }    
    //done

    return CMM_ERR_OK;
}
#endif

#ifdef STAT_INCLUDED
int32_t
mib_set_state(cli_ioctl_data_t *data)
{
    yt_enable_t *params;
    cli_ioctl_tlv_t *tlv;

    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_stat_mib_enable_set(data->hdr.unit, *params))
    {
        printk("mib state set fail. \n");
        return CMM_ERR_FAIL;
    }   
    //done
    return CMM_ERR_OK;
}

int32_t
mib_get_state(cli_ioctl_data_t *data)
{
    yt_enable_t *params;
    cli_ioctl_tlv_t *tlv;

    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_stat_mib_enable_get(data->hdr.unit, params))
    {
        printk("mib state get fail. \n");
        return CMM_ERR_FAIL;
    }

    return CMM_ERR_OK;
}

int32_t
mib_get_port(cli_ioctl_data_t *data)
{
    cli_mib_get_params_t *params;
    cli_ioctl_tlv_t *tlv;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_stat_mib_port_get(data->hdr.unit, params->port, &params->mib_port_cnt))
    {
        printk("get port mib failed. \n");
        return CMM_ERR_FAIL;
    } 
    //done

    return CMM_ERR_OK;
}

int32_t
mib_get_clear(cli_ioctl_data_t *data)
{
    cli_mib_clear_params_t *params;
    cli_ioctl_tlv_t *tlv;

    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (params->mib_clear_mode == ALL)
    {
        if (yt_stat_mib_clear_all(data->hdr.unit))
        {
            printk("clear all mib failed. \n");
            return CMM_ERR_FAIL;
        }
    }
    else if (params->mib_clear_mode == PORT)
    {
        if (yt_stat_mib_clear(data->hdr.unit, params->port))
        {
            printk("the port para is invalid. \n");
            return CMM_ERR_FAIL;
        }
    }
    //done

    return CMM_ERR_OK;
}
#endif

#ifdef L2_INCLUDED
int32_t
fdb_set_learn_state(cli_ioctl_data_t *data)
{
    cli_fdb_learn_params_t *params;
    cli_ioctl_tlv_t *tlv;
    
    tlv = &data->tlv[0];
    if (tlv->tlvhdr.len != sizeof(*params) || !tlv->val)
    {
        pr_err("%s %d data error, cmd %u\n",
        __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }

    params = tlv->val;
    //call back
    if (yt_l2_port_learn_en_set(data->hdr.unit, params->port, params->state))
    {
        printk("fdb_set_learn_state failed. \n");
        return CMM_ERR_FAIL;
    } 
    //done
    return CMM_ERR_OK;
}
#endif

int32_t
intercomm_switch_init(cli_ioctl_data_t *data)
{
    //call back
    data->hdr.ret = yt_init();
    //done
    return CMM_ERR_OK;
}

int32_t
intercomm_switch_exit(cli_ioctl_data_t *data)
{
    //call back
    data->hdr.ret = yt_exit(data->hdr.unit);
    //done
    return CMM_ERR_OK;
}

