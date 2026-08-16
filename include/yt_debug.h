/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_debug.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_DEBUG_H__
#define __YT_DEBUG_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "yt_types.h"
#include "yt_cmm.h"
#include "yt_port.h"

typedef enum yt_utp_template_testmode_e
{
    YT_UTP_TEMPLATE_TMODE_10M_10MSINE = 0, /*10MHz sine wave*/
    YT_UTP_TEMPLATE_TMODE_10M_PRANDOM, /*prseudo random*/
    YT_UTP_TEMPLATE_TMODE_10M_LINKPULSE, /*normal link pulse only*/
    YT_UTP_TEMPLATE_TMODE_10M_5MSINE, /*5MHz sine wave*/
    YT_UTP_TEMPLATE_TMODE_10M_NORMAL, /*normal mode*/
    YT_UTP_TEMPLATE_TMODE_100M_MDI = 5,
    YT_UTP_TEMPLATE_TMODE_100M_MDIX,
    YT_UTP_TEMPLATE_TMODE_1000M_T1, /*waveform test*/
    YT_UTP_TEMPLATE_TMODE_1000M_T2, /*jitter test on master*/
    YT_UTP_TEMPLATE_TMODE_1000M_T3, /*jitter test on slave*/
    YT_UTP_TEMPLATE_TMODE_1000M_T4 = 10, /*distortion test*/
    YT_UTP_TEMPLATE_TMODE_SDS2500M,
    YT_UTP_TEMPLATE_TMODE_SDS1000M
}yt_utp_template_testmode_t;

typedef enum yt_phy_loopback_mode_e
{
    YT_PHY_LOOPBACK_MODE_INTERNAL,
    YT_PHY_LOOPBACK_MODE_EXTERNAL,
    YT_PHY_LOOPBACK_MODE_REMOTE,
    YT_PHY_LOOPBACK_MODE_DISABLE
}yt_phy_loopback_mode_t;

typedef struct yt_utp_snr_s
{
    uint16_t snr[4];
}yt_utp_snr_t;

/**
 * @internal      yt_debug_phyTemplate_test_set
 * @endinternal
 *
 * @brief         set utp template test mode.
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -different test mode,refer yt_utp_template_testmode_t for details.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_debug_phyTemplate_test_set(yt_unit_t unit, yt_port_t port, yt_utp_template_testmode_t mode);


/**
 * @internal      yt_debug_phyLoopback_test_set
 * @endinternal
 *
 * @brief         set phy loopback mode.
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     mode                -internal loopback or external loopback or remote loopback.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_debug_phyLoopback_test_set(yt_unit_t unit, yt_port_t port, yt_phy_loopback_mode_t mode);

/**
 * @internal      yt_debug_utp_snr_get
 * @endinternal
 *
 * @brief         get phy utp snr value
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pSnrVal             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_debug_utp_snr_get(yt_unit_t unit, yt_port_t port, yt_utp_snr_t *pSnrVal);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
