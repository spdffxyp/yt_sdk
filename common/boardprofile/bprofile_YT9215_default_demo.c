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
extern uint32_t yt_smi_cl22_write_unit0(uint8_t phyAddr, uint8_t regAddr, uint16_t regVal);
extern uint32_t yt_smi_cl22_read_unit0(uint8_t phyAddr, uint8_t regAddr, uint16_t *pRegVal);
extern uint32_t yt_smi_cl22_write_unit1(uint8_t phyAddr, uint8_t regAddr, uint16_t regVal);
extern uint32_t yt_smi_cl22_read_unit1(uint8_t phyAddr, uint8_t regAddr, uint16_t *pRegVal);
yt_ret_t cal_profile_yt9215_init(yt_hwProfile_info_t *hwprofile_info);

// 声明两个独立的交换机实例变量
yt_swDescp_t yt9215_swDescp_unit0;
yt_swDescp_t yt9215_swDescp_unit1;

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
    {8,		PORT_ATTR_EXT_RGMII, INVALID_ID, 8,     INVALID_ID,     ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
    {9,		PORT_ATTR_EXT_RGMII, INVALID_ID, 9,     INVALID_ID,     ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
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

const yt_ledDescp_t yt9215LEDDescp = {LED_MODE_PARALLEL, NULL};


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

    // 声明双芯片架构，交换机数量设为 2
    hwprofile_info->switch_count = 2;

    // 配置第 1 颗交换机 (Unit 0, stmmac-1 -> eth0) 
    yt9215_swDescp_unit0.chip_id = YT_SW_ID_9215;
    yt9215_swDescp_unit0.chip_model = YT_SW_MODEL_9215;

    i = 0;
    while(yt9215PortDescp[i].mac_id != INVALID_ID)
    {
        yt9215_swDescp_unit0.pPortDescp[i] = &yt9215PortDescp[i];
        i++;
    }
    yt9215_swDescp_unit0.port_num = i;

    i = 0;
    while(yt9215PhyDescp[i].phy_index != INVALID_ID)
    {
        yt9215_swDescp_unit0.pPhyDescp[i] = &yt9215PhyDescp[i];
        i++;
    }

    yt9215SLEDParam.remapInfoNum = sizeof(yt9215RemapInfo)/sizeof(yt_sled_remapInfo_t);
    // yt9215_swDescp_unit0.pLEDDescp = &yt9215LEDDescp;
    
    // 绑定 Unit 0 物理读写指针 (指向 stmmac-1 物理总线)
    yt9215_swDescp_unit0.sw_access.swreg_acc_method = SWCHIP_ACC_SMI;
    yt9215_swDescp_unit0.sw_access.controller.smi_controller.smi_write = yt_smi_cl22_write_unit0;
    yt9215_swDescp_unit0.sw_access.controller.smi_controller.smi_read = yt_smi_cl22_read_unit0;
    yt9215_swDescp_unit0.sw_access.controller.smi_controller.phyAddr = 0x1D; // PHYADDR = 0x1D (29)
    yt9215_swDescp_unit0.sw_access.controller.smi_controller.switchId = 0x0;  // SWITCHID = 0x0

    hwprofile_info->pSwDescp[0] = &yt9215_swDescp_unit0;


    // 配置第 2 颗交换机 (Unit 1, stmmac-0 -> eth1)
    yt9215_swDescp_unit1.chip_id = YT_SW_ID_9215;
    yt9215_swDescp_unit1.chip_model = YT_SW_MODEL_9215;

    i = 0;
    while(yt9215PortDescp[i].mac_id != INVALID_ID)
    {
        yt9215_swDescp_unit1.pPortDescp[i] = &yt9215PortDescp[i];
        i++;
    }
    yt9215_swDescp_unit1.port_num = i;

    i = 0;
    while(yt9215PhyDescp[i].phy_index != INVALID_ID)
    {
        yt9215_swDescp_unit1.pPhyDescp[i] = &yt9215PhyDescp[i];
        i++;
    }

    yt9215SLEDParam.remapInfoNum = sizeof(yt9215RemapInfo)/sizeof(yt_sled_remapInfo_t);
    // yt9215_swDescp_unit1.pLEDDescp = &yt9215LEDDescp;
    
    // 绑定 Unit 1 物理读写指针 (指向 stmmac-0 物理总线)
    yt9215_swDescp_unit1.sw_access.swreg_acc_method = SWCHIP_ACC_SMI;
    yt9215_swDescp_unit1.sw_access.controller.smi_controller.smi_write = yt_smi_cl22_write_unit1;
    yt9215_swDescp_unit1.sw_access.controller.smi_controller.smi_read = yt_smi_cl22_read_unit1;
    yt9215_swDescp_unit1.sw_access.controller.smi_controller.phyAddr = 0x1D; // PHYADDR = 0x1D (29)
    yt9215_swDescp_unit1.sw_access.controller.smi_controller.switchId = 0x0;  // SWITCHID = 0x0

    hwprofile_info->pSwDescp[1] = &yt9215_swDescp_unit1;

    return CMM_ERR_OK;
}
