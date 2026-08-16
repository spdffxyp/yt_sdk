/*================================================================
*                                                                             *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
================================================================*/

#ifndef _YT_UTIL_C
#define _YT_UTIL_C
#include "yt_util.h"
#include <linux/module.h>

MODULE_LICENSE("GPL");

#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x

int yt_util_log(int x)
{
    int res = 0;
    if (x <= 1) {
        return 0;
    }
    // 简易整数 log10 近似计算
    while (x >= 10) {
        res += 1;
        x /= 10;
    }
    return res;
}

#endif //_YT_UTIL_C
