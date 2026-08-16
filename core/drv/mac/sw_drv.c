/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "sw_drv.h"
#ifdef SWITCH_SERIES_TIGER
#include "sw_yt921x.h"
#endif

yt_switch_drv_t *gpSwDrvList[] =
{
#ifdef SWITCH_SERIES_TIGER
    [YT_SW_MODEL_9215] = &yt9215_drv,
    [YT_SW_MODEL_9218] = &yt9218_drv,
#endif
    [YT_SW_MODEL_END] = NULL,
};
