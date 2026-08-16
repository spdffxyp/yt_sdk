/******************************************************************************
*                                                                             *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
*******************************************************************************
*  File Name     : bprofile_yt9215_default_demo.c
*  Version       : Initial Draft
*  Created       :
*  Last Modified :
*  Description   : board profile for yt9215 board,port(5 utp)
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
yt_ret_t cal_profile_yt9215_init(yt_hwProfile_info_t *hwprofile_info);

yt_swDescp_t yt9215_swDescp;

const board_profile_identify_t yt9215ProfileIdentifier = {BOARD_ID_YT9215, "yt9215 Default Demo"};

/* port descp */
const yt_portDescp_t yt9215PortDescp[] =
{
    /*macid	attribute		phy_index	phy_addr	serdes_index	ethtype			medium				smi */
    {0,		PORT_ATTR_ETH,	0,			0,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {1,		PORT_ATTR_ETH,	0,			1,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {2,		PORT_ATTR_ETH,	0,			2,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {3,		PORT_ATTR_ETH,	0,			3,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {4,		PORT_ATTR_ETH,	0,			4,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {8,		PORT_ATTR_NONE,	INVALID_ID,	8,			0,		        ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
    {9,		PORT_ATTR_NONE,	INVALID_ID,	9,			0,		        ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
#ifdef INTER_MCU
    /* internel cpu port */
    {10,    PORT_ATTR_INT_CPU,			    INVALID_ID,	INVALID_ID,	PORT_ATTR_ETH,	ETH_TYPE_GE,	PORT_MEDI_COPPER,	INVALID_ID},
#endif
    {INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,YT_SMI_NONE},
};

/* phy.descp */
const yt_phyDescp_t yt9215PhyDescp[] =
{
	/*phy_id chip_model				start_mac_id	phy_max*/
	{0,         YT_PHY_MODEL_INT861X,	0,				5},
	{INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID},
};
    
/* LED description */
const yt_sled_remapInfo_t yt9215RemapInfo[] = 
{
    {6, 1}, {4, 0}, {5, 1}, {3, 0}, {2, 0}, {1, 0}, {0, 0},
    {6, 0}, {5, 0}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1},
    {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}
};

yt_sled_param_t yt9215SLEDParam = {
    LED_SERIAL_ACTIVE_MODE_LOW,
    SLED_DATANUM_YT9215_21, 
    0,
    yt9215RemapInfo
};

const yt_ledDescp_t yt9215LEDDescp = {LED_MODE_SERIAL, &yt9215SLEDParam};


/* hardware profile */
const yt_hwProfile_t yt9215_default_demo =
{
    .pIdentifier = &yt9215ProfileIdentifier,
    .profile_init = cal_profile_yt9215_init,
};

yt_ret_t cal_profile_yt9215_init(yt_hwProfile_info_t *hwprofile_info)
{
    uint8_t	i;

    hwprofile_info->pIdentifier = &yt9215ProfileIdentifier;

    /* switch info */
    hwprofile_info->switch_count = 1;

    yt9215_swDescp.chip_id = YT_SW_ID_9215;
    yt9215_swDescp.chip_model = YT_SW_MODEL_9215;

    i = 0;
    while(yt9215PortDescp[i].mac_id != INVALID_ID)
    {
        yt9215_swDescp.pPortDescp[i] = &yt9215PortDescp[i];
        i++;
    }
    yt9215_swDescp.port_num = i;

    i = 0;
    while(yt9215PhyDescp[i].phy_index != INVALID_ID)
    {
        yt9215_swDescp.pPhyDescp[i] = &yt9215PhyDescp[i];
        i++;
    }

    yt9215SLEDParam.remapInfoNum = sizeof(yt9215RemapInfo)/sizeof(yt_sled_remapInfo_t);
    yt9215_swDescp.pLEDDescp = &yt9215LEDDescp;
    
    hwprofile_info->pSwDescp[0] = &yt9215_swDescp;

    return CMM_ERR_OK;
}
