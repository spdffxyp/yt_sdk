/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_nic.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_nic.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_nic_init
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->nic_init(unit);
}

/**
 * @internal      yt_nic_cpuport_mode_set
 * @endinternal
 *
 * @brief         Select internal or external cpu port
 * @param[in]     unit                -unit id
 * @param[in]     mode                -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_cpuport_mode_set(yt_unit_t unit, yt_cpuport_mode_t mode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->nic_cpuport_mode_set(unit, mode);
}

/**
 * @internal      yt_nic_cpuport_mode_get
 * @endinternal
 *
 * @brief         get cpu port mode setting
 * @param[in]     unit                -unit id
 * @param[out]    pmode               -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_cpuport_mode_get(yt_unit_t unit, yt_cpuport_mode_t *pMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->nic_cpuport_mode_get(unit, pMode);
}

/**
 * @internal      yt_nic_ext_cpuport_en_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_ext_cpuport_en_set(yt_unit_t unit,  yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->nic_ext_cpuport_en_set(unit, enable);
}

/**
 * @internal      yt_nic_ext_cpuport_en_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_ext_cpuport_en_get(yt_unit_t unit,  yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->nic_ext_cpuport_en_get(unit, pEnable);
}

/**
 * @internal      yt_nic_ext_cpuport_port_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_ext_cpuport_port_set(yt_unit_t unit,  yt_port_t port)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);

    return YT_DISPATCH(unit)->nic_ext_cpuport_port_set(unit, port);
}

/**
 * @internal      yt_nic_ext_cpuport_port_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pPort               -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_ext_cpuport_port_get(yt_unit_t unit,  yt_port_t *pPort)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pPort), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->nic_ext_cpuport_port_get(unit, pPort);
}

/**
 * @internal      yt_nic_cpuport_tagtpid_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -TPID
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_cpuport_tagtpid_set(yt_unit_t unit, uint16_t tpid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->nic_cpuport_tagtpid_set(unit, tpid);
}

/**
 * @internal      yt_nic_cpuport_tagtpid_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit
 * @param[out]    ptpid               -TPID
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_cpuport_tagtpid_get(yt_unit_t unit, uint16_t *pTpid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pTpid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->nic_cpuport_tagtpid_get(unit, pTpid);
}

/**
 * @internal      yt_nic_ext_cputag_en_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_ext_cputag_en_set(yt_unit_t unit,  yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->nic_ext_cputag_en_set(unit, enable);
}

/**
 * @internal      yt_nic_ext_cputag_en_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t  yt_nic_ext_cputag_en_get(yt_unit_t unit,  yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->nic_ext_cputag_en_get(unit, pEnable);
}

yt_ret_t yt_nic_rx_register(yt_unit_t unit, yt_nic_rx_cb_f rx_cb)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == rx_cb), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->nic_rx_register(unit, rx_cb);
}

yt_ret_t yt_nic_rx_handler(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->nic_rx_handler(unit);
}

yt_ret_t yt_nic_tx(yt_unit_t unit, yt_tx_pkt_t *p_tx_pkt, uint8_t mem_free_flag)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == p_tx_pkt), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((TRUE != mem_free_flag) && (FALSE != mem_free_flag), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->nic_tx(unit, p_tx_pkt, mem_free_flag);
}

yt_ret_t  yt_nic_eth_drv_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->nic_eth_drv_init(unit);
}

yt_ret_t yt_nic_eth_buf_cnt_len_set(yt_unit_t unit, uint16_t buf_cnt, uint16_t tx_buf_cnt, uint16_t buf_len)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((buf_cnt < NIC_ETH_BUF_CNT_MIN), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((tx_buf_cnt < NIC_ETH_TXBUF_CNT_MIN), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((buf_cnt <= tx_buf_cnt), CMM_ERR_INPUT);
    CMM_PARAM_CHK((buf_len < NIC_ETH_BUF_LEN_MIN), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->nic_eth_buf_cnt_len_set(unit, buf_cnt, tx_buf_cnt, buf_len);
}

yt_ret_t yt_nic_eth_debug_flag_set(yt_unit_t unit, uint8_t debug_flag)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((TRUE != debug_flag) && (FALSE != debug_flag), CMM_ERR_INPUT);
    
    return YT_DISPATCH(unit)->nic_eth_debug_flag_set(unit, debug_flag);
}

