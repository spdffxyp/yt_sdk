/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_debug.c
*
* @brief x
*
********************************************************************************
*/
/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_print.h"
#include "fal_tiger_debug.h"
#include "fal_tiger_port.h"
#include "hal_mem.h"
#include "hal_ctrl.h"

#if defined(DEBUG_INCLUDED) && defined(PORT_INCLUDED)
yt_ret_t fal_tiger_debug_phyTemplate_test_set(yt_unit_t unit, yt_port_t port, yt_utp_template_testmode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t phy_addr;
    yt_macid_t mac_id;
    uint32_t regData;

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, 0x80028, &regData), ret);

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if (INVALID_ID != CAL_YTP_TO_EXTPORT(unit, port))/*for sds test*/
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80028, (1<<(2+port-5)) | 0x3), ret);
        /*disable serdes interface*/
        regData &= 0x7c;
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80028, regData), ret);
        if(mode == YT_UTP_TEMPLATE_TMODE_SDS2500M)
        {
            fal_tiger_port_extif_mode_set(unit, port, YT_EXTIF_MODE_BX2500);
        }
        else if(mode == YT_UTP_TEMPLATE_TMODE_SDS1000M)
        {
            fal_tiger_port_extif_mode_set(unit, port, YT_EXTIF_MODE_FIB_1000);
        }
    }
    else /*internal phy port*/
    {
        /*disable serdes interface*/
        regData &= 0x7c;
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x80028, regData), ret);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, 0x8002c, (1<<(16+port)) | 0xff), ret);
    }

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            CMM_ERR_CHK(HALPHYDRV_FUNC(unit, mac_id)->phy_test_template(unit, phy_addr, mode), ret);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}
#endif

yt_ret_t fal_tiger_debug_phyLoopback_test_set(yt_unit_t unit, yt_port_t port, yt_phy_loopback_mode_t mode)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t phy_addr;
    yt_macid_t mac_id;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            CMM_ERR_CHK(HALPHYDRV_FUNC(unit, mac_id)->phy_loopback_set(unit, phy_addr, mode), ret);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t fal_tiger_debug_utp_snr_get(yt_unit_t unit, yt_port_t port, yt_utp_snr_t *pSnrValue)
{
    yt_ret_t ret = CMM_ERR_OK;
    yt_port_t phy_addr;
    yt_macid_t mac_id;
    yt_port_linkStatus_all_t allLinkStatus;

    mac_id = CAL_YTP_TO_MAC(unit, port);
    phy_addr = CAL_YTP_TO_PHYADDR(unit, port);
    osal_memset(&allLinkStatus, 0, sizeof(allLinkStatus));
    CMM_ERR_CHK(fal_tiger_port_link_status_all_get(unit, port, &allLinkStatus), ret);
    if (allLinkStatus.link_speed != PORT_SPEED_1000M
        && allLinkStatus.link_speed != PORT_SPEED_100M)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    if(phy_addr != INVALID_ID)
    {
        if(HALPHYDRV(unit, mac_id) != NULL)
        {
            HALPHYDRV_FUNC(unit, mac_id)->phy_utp_snr_get(unit, phy_addr, allLinkStatus.link_speed, pSnrValue);
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}
