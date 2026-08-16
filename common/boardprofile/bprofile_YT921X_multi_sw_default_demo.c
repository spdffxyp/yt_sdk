/******************************************************************************
*                                                                             *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
*******************************************************************************
*  File Name     : bprofile_YT921X_multi_sw_default_demo.c
*  Version       : Initial Draft
*  Created       :
*  Last Modified :
*  Description   : board profile for yt9215 + yt9218 + yt9214
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
yt_ret_t cal_profile_yt921x_multi_sw_init(yt_hwProfile_info_t *hwprofile_info);

yt_swDescp_t yt921xSwDesc[YT_MAX_UNIT];

const board_profile_identify_t yt921xProfileIdentifier = {BOARD_ID_YT921X, "YT921X Multiple Switch Demo"};

/* port descp */
const yt_portDescp_t yt9215xPortDescp[] =
{
    /*macid	    attribute		phy_index	phy_addr	serdes_index	ethtype			medium				smi */
    {0,		    PORT_ATTR_ETH,	0,			0,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {1,		    PORT_ATTR_ETH,	0,			1,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {2,		    PORT_ATTR_ETH,	0,			2,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {3,		    PORT_ATTR_ETH,	0,			3,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {4,		    PORT_ATTR_ETH,	0,			4,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {8,		    PORT_ATTR_NONE,	INVALID_ID,	8,			0,		        ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
    {9,		    PORT_ATTR_NONE,	INVALID_ID,	9,			0,		        ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
    {INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,YT_SMI_NONE},
};

const yt_portDescp_t yt9218xPortDescp[] =
{
    /*macid	attribute		phy_index	phy_addr	serdes_index	ethtype			medium				smi */
    {0,		PORT_ATTR_ETH,	0,			0,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {1,		PORT_ATTR_ETH,	0,			1,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {2,		PORT_ATTR_ETH,	0,			2,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {3,		PORT_ATTR_ETH,	0,			3,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {4,		PORT_ATTR_ETH,	0,			4,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {5,		PORT_ATTR_ETH,	0,			5,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {6,		PORT_ATTR_ETH,	0,			6,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {7,		PORT_ATTR_ETH,	0,			7,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {8,		PORT_ATTR_NONE,	INVALID_ID,	8,			0,		        ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
    {9,		PORT_ATTR_NONE,	INVALID_ID,	9,			0,		        ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
    {INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,YT_SMI_NONE},
};

const yt_portDescp_t yt9214xPortDescp[] =
{
    /*macid	    attribute		    phy_index	    phy_addr	serdes_index	ethtype			medium				smi */
    {1,		    PORT_ATTR_ETH,	    0,			    1,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {3,		    PORT_ATTR_ETH,	    0,			    3,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {8,		    PORT_ATTR_NONE,	    INVALID_ID,		0,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
    {9,		    PORT_ATTR_NONE,	    INVALID_ID,		0,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
    {INVALID_ID,INVALID_ID,         INVALID_ID,     INVALID_ID, INVALID_ID,INVALID_ID,INVALID_ID,YT_SMI_NONE},
};

/* phy descp */
const yt_phyDescp_t yt9215xPhyDescp[] =
{
	/*phy_id    chip_model				start_mac_id	phy_max*/
	{0,         YT_PHY_MODEL_INT861X,	0,				5},
	{INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID},
};

const yt_phyDescp_t yt9218xPhyDescp[] =
{
	/*phy_id    chip_model				start_mac_id	phy_max*/
	{0,         YT_PHY_MODEL_INT861X,	0,				8},
	{INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID},
};

const yt_phyDescp_t yt9214xPhyDescp[] =
{
	/*phy_id    chip_model				start_mac_id	phy_max*/
	{0,         YT_PHY_MODEL_INT861X,	0,				8},
	{INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID},
};

/* LED description */
const yt_sled_remapInfo_t yt9215xRemapInfo[] = 
{
    {6, 1}, {4, 0}, {5, 1}, {3, 0}, {2, 0}, {1, 0}, {0, 0},
    {6, 0}, {5, 0}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1},
    {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}
};

yt_sled_param_t yt9215xLEDParam = {
    LED_SERIAL_ACTIVE_MODE_LOW,
    SLED_DATANUM_YT9215_21, 
    0,
    yt9215xRemapInfo
};

const yt_ledDescp_t yt9215xLEDDescp = {LED_MODE_SERIAL, &yt9215xLEDParam};

const yt_sled_remapInfo_t yt9218xRemapInfo[] =
{
    {9, 0}, {8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0},
    {9, 1}, {8, 1}, {7, 1}, {6, 1}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1},
    {9, 2}, {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}
};

yt_sled_param_t yt9218xLEDParam = {
    LED_SERIAL_ACTIVE_MODE_LOW,
    SLED_DATANUM_YT9218_30, 
    0,
    yt9218xRemapInfo
};

const yt_ledDescp_t yt9218xLEDDescp = {LED_MODE_SERIAL, &yt9218xLEDParam};

const yt_sled_remapInfo_t yt9214xRemapInfo[] =
{
    {6, 1}, {4, 0}, {5, 1}, {3, 0}, {2, 0}, {1, 0}, {0, 0},
    {6, 0}, {5, 0}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1},
    {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}
};

yt_sled_param_t yt9214xLEDParam = {
    LED_SERIAL_ACTIVE_MODE_LOW,
    SLED_DATANUM_YT9215_21, 
    0,
    yt9214xRemapInfo
};

const yt_ledDescp_t yt9214xLEDDescp = {LED_MODE_PARALLEL, &yt9214xLEDParam};

/* hardware profile */
const yt_hwProfile_t yt921x_multi_sw_default_demo =
{
    .pIdentifier = &yt921xProfileIdentifier,
    .profile_init = cal_profile_yt921x_multi_sw_init,
};

yt_ret_t cal_profile_yt921x_multi_sw_init(yt_hwProfile_info_t *hwprofile_info)
{
    uint8_t	i;
    yt_unit_t unit;

    hwprofile_info->pIdentifier = &yt921xProfileIdentifier;
    /* modified according to the actual situation */
    hwprofile_info->switch_count = BOARD_MAX_UNIT_NUM;

    /* default unit0: 9215x, modified according to the actual situation */
    unit = 0;
    yt921xSwDesc[unit].chip_id = YT_SW_ID_9215;
    yt921xSwDesc[unit].chip_model = YT_SW_MODEL_9215;

    i = 0;
    while(yt9215xPortDescp[i].mac_id != INVALID_ID)
    {
        yt921xSwDesc[unit].pPortDescp[i] = &yt9215xPortDescp[i];
        i++;
    }
    yt921xSwDesc[unit].port_num = i;

    i = 0;
    while(yt9215xPhyDescp[i].phy_index != INVALID_ID)
    {
        yt921xSwDesc[unit].pPhyDescp[i] = &yt9215xPhyDescp[i];
        i++;
    }

    yt9215xLEDParam.remapInfoNum = sizeof(yt9215xRemapInfo)/sizeof(yt_sled_remapInfo_t);
    yt921xSwDesc[unit].pLEDDescp = &yt9215xLEDDescp;


    /* default unit1: 9218x, modified according to the actual situation */
    unit = 1;
    yt921xSwDesc[unit].chip_id = YT_SW_ID_9218;
    yt921xSwDesc[unit].chip_model = YT_SW_MODEL_9218;

    i = 0;
    while(yt9218xPortDescp[i].mac_id != INVALID_ID)
    {
        yt921xSwDesc[unit].pPortDescp[i] = &yt9218xPortDescp[i];
        i++;
    }
    yt921xSwDesc[unit].port_num = i;

    i = 0;
    while(yt9218xPhyDescp[i].phy_index != INVALID_ID)
    {
        yt921xSwDesc[unit].pPhyDescp[i] = &yt9218xPhyDescp[i];
        i++;
    }

    yt9218xLEDParam.remapInfoNum = sizeof(yt9218xRemapInfo)/sizeof(yt_sled_remapInfo_t);
    yt921xSwDesc[unit].pLEDDescp = &yt9218xLEDDescp;

    /* default unit2: 9214x, modified according to the actual situation */
    unit = 2;
    yt921xSwDesc[unit].chip_id = YT_SW_ID_9215;
    yt921xSwDesc[unit].chip_model = YT_SW_MODEL_9215;

    i = 0;
    while(yt9214xPortDescp[i].mac_id != INVALID_ID)
    {
        yt921xSwDesc[unit].pPortDescp[i] = &yt9214xPortDescp[i];
        i++;
    }
    yt921xSwDesc[unit].port_num = i;

    i = 0;
    while(yt9214xPhyDescp[i].phy_index != INVALID_ID)
    {
        yt921xSwDesc[unit].pPhyDescp[i] = &yt9214xPhyDescp[i];
        i++;
    }

    yt9214xLEDParam.remapInfoNum = sizeof(yt9214xRemapInfo)/sizeof(yt_sled_remapInfo_t);
    yt921xSwDesc[unit].pLEDDescp = &yt9214xLEDDescp;


    for (unit = 0; unit < YT_MAX_UNIT; unit++)
    {
        hwprofile_info->pSwDescp[unit] = &yt921xSwDesc[unit];
    }

    return CMM_ERR_OK;
}
