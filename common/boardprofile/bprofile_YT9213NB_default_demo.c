/******************************************************************************
*                                                                             *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
*******************************************************************************
*  File Name     : bprofile_yt9213NB_default_demo.c
*  Version       : Initial Draft
*  Created       :
*  Last Modified :
*  Description   : board profile for yt9213nb board,port(2 utp)
*
******************************************************************************/

/**************************************************
 *      Include header files                       *
 **************************************************/
#include "cal_bprofile.h"
#include "yt_error.h"
/**************************************************
 *      Constants or macros Declaration            *
 **************************************************/

/**************************************************
 *      Global variables                           *
 **************************************************/

/**************************************************
 *      Functions Declaration                      *
 **************************************************/
yt_ret_t cal_profile_yt9213nb_init(yt_hwProfile_info_t *hwprofile_info);

yt_swDescp_t yt9213nb_swDescp;

const board_profile_identify_t yt9213nbProfileIdentifier = {BOARD_ID_YT9213NB, "yt9213NB Default Demo"};

/* port descp */
const yt_portDescp_t yt9213nbPortDescp[] =
{
    /*macid	attribute		phy_index	phy_addr	serdes_index	ethtype			medium				smi */
    {1,		PORT_ATTR_ETH,	0,			1,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {3,		PORT_ATTR_ETH,	0,			3,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {9,		PORT_ATTR_NONE,	INVALID_ID,			0,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
#ifdef INTER_MCU
    /* internel cpu port */
    {10,    PORT_ATTR_INT_CPU,			    INVALID_ID,	INVALID_ID,	PORT_ATTR_ETH,	ETH_TYPE_GE,	PORT_MEDI_COPPER,	INVALID_ID},
#endif
    {INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,YT_SMI_NONE},
};

/* phy.descp */
const yt_phyDescp_t yt9213nbPhyDescp[] =
{
	/*phy_id chip_model				start_mac_id	phy_max*/
	{0,         YT_PHY_MODEL_INT861X,	0,				8},
	{INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID},
};
    
/* LED description */
const yt_sled_remapInfo_t yt9213nbRemapInfo[] = 
{
    {6, 1}, {4, 0}, {5, 1}, {3, 0}, {2, 0}, {1, 0}, {0, 0},
    {6, 0}, {5, 0}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1},
    {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}
};

yt_sled_param_t yt9213nbSLEDParam = {
    LED_SERIAL_ACTIVE_MODE_LOW,
    SLED_DATANUM_YT9215_21, 
    0,
    yt9213nbRemapInfo
};

const yt_ledDescp_t yt9213nbLEDDescp = {LED_MODE_PARALLEL, &yt9213nbSLEDParam};


/* hardware profile */
const yt_hwProfile_t yt9213nb_default_demo =
{
    .pIdentifier = &yt9213nbProfileIdentifier,
    .profile_init = cal_profile_yt9213nb_init,
};

yt_ret_t cal_profile_yt9213nb_init(yt_hwProfile_info_t *hwprofile_info)
{
    uint8_t i;

    hwprofile_info->pIdentifier = &yt9213nbProfileIdentifier;

    /* switch info */
    hwprofile_info->switch_count = 1;

    yt9213nb_swDescp.chip_id = YT_SW_ID_9215;
    yt9213nb_swDescp.chip_model = YT_SW_MODEL_9215;

    i = 0;
    while(yt9213nbPortDescp[i].mac_id != INVALID_ID)
    {
        yt9213nb_swDescp.pPortDescp[i] = &yt9213nbPortDescp[i];
        i++;
    }
    yt9213nb_swDescp.port_num = i;

    i = 0;
    while(yt9213nbPhyDescp[i].phy_index != INVALID_ID)
    {
        yt9213nb_swDescp.pPhyDescp[i] = &yt9213nbPhyDescp[i];
        i++;
    }

    yt9213nbSLEDParam.remapInfoNum = sizeof(yt9213nbRemapInfo)/sizeof(yt_sled_remapInfo_t);
    yt9213nb_swDescp.pLEDDescp = &yt9213nbLEDDescp;

    hwprofile_info->pSwDescp[0] = &yt9213nb_swDescp;

    return CMM_ERR_OK;
}
