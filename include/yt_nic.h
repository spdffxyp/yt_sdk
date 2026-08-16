/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_nic.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_NIC_H
#define __YT_NIC_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_cmm.h"

#define NIC_ETH_BUF_CNT_MIN 56
#define NIC_ETH_TXBUF_CNT_MIN 6
#define NIC_ETH_BUF_LEN_MIN 256

typedef enum yt_cpuport_mode_e
{
    CPUPORT_MODE_INTERNAL,
    CPUPORT_MODE_EXTERNAL,
} yt_cpuport_mode_t;
    
typedef enum yt_nic_tx_flag_e
{
    NIC_TX_BY_DEFAULT = 0,
    NIC_TX_BY_PORTLIST,
    NIC_TX_BY_VLAN,
} yt_nic_tx_flag_t;    
    
typedef struct yt_tx_pkt_info_s
{
    yt_port_mask_t port_mask;
    yt_vlan_t vlan_id;
    yt_nic_tx_flag_t tx_flag;
} yt_tx_pkt_info_t;

typedef struct yt_tx_pkt_s
{
    uint8_t *p_pkt_buff;
    uint32_t pkt_len; 
    yt_tx_pkt_info_t tx_pkt_info;
} yt_tx_pkt_t;


/**
 * @internal      yt_nic_rx_register
 * @endinternal
 *
 * @brief         register rx callback function
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     rx_cb               -rx callback function
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input parameter error
 */
extern yt_ret_t yt_nic_rx_register(yt_unit_t unit, yt_nic_rx_cb_f rx_cb);


/**
 * @internal      yt_nic_rx_handler
 * @endinternal
 *
 * @brief         rx handler API used in user's main task
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_nic_rx_handler(yt_unit_t unit);


/**
 * @internal      yt_nic_tx
 * @endinternal
 *
 * @brief         tx the packet in given portlist
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     p_tx_pkt            -pointer to tx packet struct
 * @param[in]     mem_free_flag       -flag to indicate whether to free packet buffer after tx the packet
 *                                     if FALSE, Application should free the packet buffer by itself;
 *                                     if TRUE, the API will free the packet buffer aumatically after tx successfully;
 *                                     Note: in TRUE case, Applictaion still should free the packet buffer by itself if
 *                                           the called API return failure.
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT       -input parameter error
 */
extern yt_ret_t yt_nic_tx(yt_unit_t unit, yt_tx_pkt_t *p_tx_pkt, uint8_t mem_free_flag);


/**
 * @internal      yt_nic_eth_buf_cnt_len_set
 * @endinternal
 *
 * @brief         set eth buffer count and length, this API should be called before yt_nic_eth_drv_init.
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     buf_cnt             -buffer count, the value should be greater than NIC_ETH_BUF_CNT_MIN
 * @param[in]     tx_buf_cnt        -reserved buffer count for tx, the value should be greater than NIC_ETH_TXBUF_CNT_MIN
 * @param[in]     buf_len             -buffer length, the value should be greater than NIC_ETH_BUF_LEN_MIN
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_nic_eth_buf_cnt_len_set(yt_unit_t unit, uint16_t buf_cnt, uint16_t tx_buf_cnt, uint16_t buf_len);


/**
 * @internal      yt_nic_eth_debug_flag_set
 * @endinternal
 *
 * @brief         set eth debug flag
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     debug_flag          -debug flag, TRUE or FALSE
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_nic_eth_debug_flag_set(yt_unit_t unit, uint8_t debug_flag);


/**
 * @internal      yt_nic_eth_drv_init
 * @endinternal
 *
 * @brief         init eth driver
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_eth_drv_init(yt_unit_t unit);


/**
 * @internal      yt_nic_init
 * @endinternal
 *
 * @brief         init nic module
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_init(yt_unit_t unit);


/**
 * @internal      yt_nic_cpuport_mode_set
 * @endinternal
 *
 * @brief         Select internal or external cpu port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     mode                -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input parameter error
 */
extern yt_ret_t  yt_nic_cpuport_mode_set(yt_unit_t unit, yt_cpuport_mode_t mode);


/**
 * @internal      yt_nic_cpuport_mode_get
 * @endinternal
 *
 * @brief         Get cpu port mode setting
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pMode               -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input parameter error
 */
extern yt_ret_t  yt_nic_cpuport_mode_get(yt_unit_t unit, yt_cpuport_mode_t *pMode);


/**
 * @internal      yt_nic_ext_cpuport_en_set
 * @endinternal
 *
 * @brief         enable or disable extend cpu port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cpuport_en_set(yt_unit_t unit,  yt_enable_t enable);


/**
 * @internal      yt_nic_ext_cpuport_en_get
 * @endinternal
 *
 * @brief         get the state of extend cpu port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cpuport_en_get(yt_unit_t unit,  yt_enable_t *pEnable);


/**
 * @internal      yt_nic_ext_cpuport_port_set
 * @endinternal
 *
 * @brief         set the extend cpu port num
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cpuport_port_set(yt_unit_t unit,  yt_port_t port);


/**
 * @internal      yt_nic_ext_cpuport_port_get
 * @endinternal
 *
 * @brief         get the extend cpu port num
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pPort               -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cpuport_port_get(yt_unit_t unit,  yt_port_t *pPort);


/**
 * @internal      yt_nic_cpuport_tagtpid_set
 * @endinternal
 *
 * @brief         set vlan tag tpid for cpu port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -tag tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_cpuport_tagtpid_set(yt_unit_t unit, uint16_t tpid);


/**
 * @internal      yt_nic_cpuport_tagtpid_get
 * @endinternal
 *
 * @brief         get vlan tag tpid for cpu port
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pTpid               -tag tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_cpuport_tagtpid_get(yt_unit_t unit, uint16_t *pTpid);

/**
 * @internal      yt_nic_ext_cputag_en_set
 * @endinternal
 *
 * @brief         enable or disable extend cpu tag
 * @note          APPLICABLE DEVICES  -Tiger 
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cputag_en_set(yt_unit_t unit,  yt_enable_t enable);

/**
 * @internal      yt_nic_ext_cputag_en_get
 * @endinternal
 *
 * @brief         get the state of extend cpu tag
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cputag_en_get(yt_unit_t unit,  yt_enable_t *pEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
