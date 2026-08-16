/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_lag.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_lag.h"
#include "fal_dispatch.h"


/**
 * @internal      yt_lag_hash_sel_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     hash_mask           -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_lag_hash_sel_set(yt_unit_t unit, uint16_t hash_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((0 == hash_mask || 0x0FF < hash_mask), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->lag_hash_sel_set(unit, (uint8_t)hash_mask);
}

/**
 * @internal      yt_lag_hash_sel_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    p_hash_mask         -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_lag_hash_sel_get(yt_unit_t unit, uint16_t *p_hash_mask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == p_hash_mask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->lag_hash_sel_get(unit, (uint8_t *)p_hash_mask);
}

/**
 * @internal      yt_lag_group_port_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -x
 * @param[in]     member_portmask     -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_lag_group_port_set(yt_unit_t unit, uint8_t groupId, yt_port_mask_t member_portmask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit,member_portmask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->lag_group_port_set(unit, groupId, member_portmask);
}

/**
 * @internal      yt_lag_group_port_get
 * @endinternal
 *
 * @brief         get lag group member portmask
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     groupId             -lag group index
 * @param[out]    p_member_portmask   -link aggregation group portmask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t yt_lag_group_port_get(yt_unit_t unit, uint8_t groupId, yt_port_mask_t *p_member_portmask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(YT_DISPATCH(unit)->is_inited)), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == p_member_portmask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->lag_group_port_get(unit, groupId, p_member_portmask);
}

