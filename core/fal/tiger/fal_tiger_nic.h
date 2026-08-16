/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_nic.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_NIC_H__
#define __FAL_TIGER_NIC_H__


#include "fal_cmm.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @internal      fal_tiger_nic_init
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_init(yt_unit_t unit);


/**
 * @internal      fal_tiger_nic_cpuport_mode_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     mode                -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_cpuport_mode_set(yt_unit_t unit, yt_cpuport_mode_t mode);


/**
 * @internal      fal_tiger_nic_cpuport_mode_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pmode               -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_cpuport_mode_get(yt_unit_t unit, yt_cpuport_mode_t *pmode);


/**
 * @internal      fal_tiger_nic_ext_cpuport_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cpuport_en_set(yt_unit_t unit,  yt_enable_t enable);


/**
 * @internal      fal_tiger_nic_ext_cpuport_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cpuport_en_get(yt_unit_t unit,  yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_nic_ext_cpuport_port_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cpuport_port_set(yt_unit_t unit,  yt_port_t port);


/**
 * @internal      fal_tiger_nic_ext_cpuport_port_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pport               -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cpuport_port_get(yt_unit_t unit,  yt_port_t *pport);


/**
 * @internal      fal_tiger_nic_cpuport_tagtpid_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_cpuport_tagtpid_set(yt_unit_t unit, uint16_t tpid);


/**
 * @internal      fal_tiger_nic_cpuport_tagtpid_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    ptpid               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_cpuport_tagtpid_get(yt_unit_t unit, uint16_t *ptpid);


/**
 * @internal      fal_tiger_nic_ext_cputag_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cputag_en_set(yt_unit_t unit,  yt_enable_t enable);


/**
 * @internal      fal_tiger_nic_ext_cputag_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cputag_en_get(yt_unit_t unit,  yt_enable_t *pEnable);

extern yt_ret_t fal_tiger_nic_rx_register(yt_unit_t unit, yt_nic_rx_cb_f rx_cb);

extern yt_ret_t fal_tiger_nic_rx_handler(yt_unit_t unit);

extern yt_ret_t fal_tiger_nic_tx(yt_unit_t unit, yt_tx_pkt_t *p_tx_pkt, uint8_t mem_free_flag);

extern yt_ret_t fal_tiger_nic_eth_buf_cnt_len_set(yt_unit_t unit, uint16_t buf_cnt, uint16_t tx_buf_cnt, uint16_t buf_len);

extern yt_ret_t fal_tiger_nic_eth_debug_flag_set(yt_unit_t unit, uint8_t debug_flag);

extern yt_ret_t fal_tiger_nic_eth_drv_init(yt_unit_t unit);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
