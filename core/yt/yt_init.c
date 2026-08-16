/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/*
 * Include Files
 */
#include <linux/phy.h>
#include <linux/init.h>
#include <linux/module.h>
#include "yt_error.h"
#include "yt_types.h"
#include "yt_init.h"
#include "yt_vlan.h"
#include "cal_mgm.h"
#include "fal_dispatch.h"
#include "hal_cmm.h"
#include "drv.h"
#include "fal_init.h"

extern void smi_mib_proc_test(void);

yt_ret_t  yt_basic_init(void)
{
    cmm_err_t ret = CMM_ERR_OK;
    CMM_ERR_CHK(cal_mgm_init(), ret);
    CMM_ERR_CHK(hal_init(), ret);
    CMM_ERR_CHK(fal_dispatch_init(), ret);
    CMM_ERR_CHK(yt_drv_init(), ret);
    CMM_ERR_CHK(fal_init(), ret);

    return CMM_ERR_OK;
}

yt_ret_t  yt_device_init(yt_unit_t unit, uint8_t *dev)
{
    cmm_err_t ret = CMM_ERR_OK;
    CMM_UNUSED_PARAM(unit);
    CMM_ERR_CHK(drv_init(dev),ret);
    return CMM_ERR_OK;
}

yt_ret_t  yt_device_close(yt_unit_t unit)
{
    cmm_err_t ret = CMM_ERR_OK;
    CMM_UNUSED_PARAM(unit);
    CMM_ERR_CHK(drv_close(),ret);
    return CMM_ERR_OK;
}

yt_ret_t yt_modules_init(void)
{
    yt_unit_t unit;
    cmm_err_t ret = CMM_ERR_OK;
    
    for(unit = 0; unit < YT_UNIT_NUM; unit++)
    {
#ifdef L2_INCLUDED
        CMM_ERR_CHK(yt_l2_init(unit),ret);
#endif
#ifdef PORT_INCLUDED    
        CMM_ERR_CHK(yt_port_init(unit),ret);
#endif
#ifdef VLAN_INCLUDED
        CMM_ERR_CHK(yt_vlan_init(unit),ret);
#endif
#ifdef RATE_INCLUDED
        CMM_ERR_CHK(yt_rate_init(unit),ret);
#endif
#ifdef STORM_CTRL_INCLUDED
        CMM_ERR_CHK(yt_storm_ctrl_init(unit),ret);
#endif
#ifdef ACL_INCLUDED
        CMM_ERR_CHK(yt_acl_init(unit),ret);
#endif
#ifdef DOT1X_INCLUDED
        CMM_ERR_CHK(yt_dot1x_init(unit),ret);
#endif
#ifdef STAT_INCLUDED
        CMM_ERR_CHK(yt_stat_mib_init(unit),ret);
#endif
#ifdef NIC_INCLUDED
        CMM_ERR_CHK(yt_nic_init(unit),ret);
#endif
#ifdef DOS_INCLUDED
        CMM_ERR_CHK(yt_dos_init(unit),ret);
#endif
    }
    
    return CMM_ERR_OK;
}

yt_ret_t  yt_init(void)
{
    cmm_err_t ret = CMM_ERR_OK;

    YT_DEBUG_LEVEL(YT_DEBUG_NONE);

    CMM_ERR_CHK(yt_basic_init(),ret);

    CMM_ERR_CHK(yt_modules_init(),ret);

    return CMM_ERR_OK;
}

yt_ret_t  yt_drv_init(void)
{
    yt_unit_t unit;
    cmm_err_t ret = CMM_ERR_OK;

    for(unit = 0; unit < YT_UNIT_NUM; unit++)
    {
#ifdef ACC_UART
        CMM_ERR_CHK(yt_device_init(unit, "ttyS2"),ret);
#else
        CMM_ERR_CHK(yt_device_init(unit, NULL),ret);
#endif
    }

    return CMM_ERR_OK;
}


// 通道 0 专属：写入 stmmac-1 (eth0 交换机)
uint32_t yt_smi_cl22_write_unit0(uint8_t phyAddr, uint8_t regAddr, uint16_t regVal)
{
    struct mii_bus *bus = mdio_find_bus("stmmac-1");
    if (bus) {
        bus->write(bus, (int)phyAddr, (int)regAddr, regVal);
        return 0;
    }
    return -ENODEV;
}
EXPORT_SYMBOL(yt_smi_cl22_write_unit0);

// 通道 0 专属：读取 stmmac-1 (eth0 交换机)
uint32_t yt_smi_cl22_read_unit0(uint8_t phyAddr, uint8_t regAddr, uint16_t *pRegVal)
{
    struct mii_bus *bus = mdio_find_bus("stmmac-1");
    if (bus) {
        int ret = bus->read(bus, (int)phyAddr, (int)regAddr);
        if (ret >= 0 && ret != 0xffff) {
            *pRegVal = (uint16_t)ret;
            return 0;
        }
    }
    return -ENODEV;
}
EXPORT_SYMBOL(yt_smi_cl22_read_unit0);

// 通道 1 专属：写入 stmmac-0 (eth1 交换机)
uint32_t yt_smi_cl22_write_unit1(uint8_t phyAddr, uint8_t regAddr, uint16_t regVal)
{
    struct mii_bus *bus = mdio_find_bus("stmmac-0");
    if (bus) {
        bus->write(bus, (int)phyAddr, (int)regAddr, regVal);
        return 0;
    }
    return -ENODEV;
}
EXPORT_SYMBOL(yt_smi_cl22_write_unit1);

// 通道 1 专属：读取 stmmac-0 (eth1 交换机)
uint32_t yt_smi_cl22_read_unit1(uint8_t phyAddr, uint8_t regAddr, uint16_t *pRegVal)
{
    struct mii_bus *bus = mdio_find_bus("stmmac-0");
    if (bus) {
        int ret = bus->read(bus, (int)phyAddr, (int)regAddr);
        if (ret >= 0 && ret != 0xffff) {
            *pRegVal = (uint16_t)ret;
            return 0;
        }
    }
    return -ENODEV;
}
EXPORT_SYMBOL(yt_smi_cl22_read_unit1);

/**
 * @brief  配置指定 Unit 和 Port 的 LED 行为为常亮+数据闪烁模式
 * @return yt_ret_t CMM_ERR_OK 代表成功，其他代表失败
 */
yt_ret_t set_ports_led_active_blink(void)
{
    yt_ret_t ret = CMM_ERR_OK;
    yt_unit_t unit;
    yt_port_t port;
    yt_led_act_cfg_t led_cfg;

    // 1. 初始化 LED 动作配置结构体
    // 如果希望“仅在有数据时闪烁，平时完全不亮（无 Link 状态指示）”，可将 spdxxx_on_en 保持为 0
    memset(&led_cfg, 0, sizeof(yt_led_act_cfg_t));
    
    // 配置 Link 指示：10M/100M/1000M 链路建立时常亮
    led_cfg.spd10m_on_en   = 1;
    led_cfg.spd100m_on_en  = 1;
    led_cfg.spd1000m_on_en = 1;
    
    // 配置 Active 闪烁指示：有 RX 或 TX 数据流时闪烁
    led_cfg.rxact_blink_en = 1;
    led_cfg.txact_blink_en = 1;

    // 2. 循环遍历 2 个 Unit (Unit 0 和 Unit 1)
    for (unit = 0; unit < 2; unit++)
    {
        // 全局使能该 Unit 的 LED 控制器（如尚未使能）
        ret = yt_led_enable(unit);
        if (ret != CMM_ERR_OK)
        {
            printk(KERN_INFO "[Motorcomm] Failed to enable LED for unit %d, error: %d\n", unit, ret);
            return ret;
        }

        // 3. 循环遍历 port 1 到 port 4
        for (port = 1; port <= 4; port++)
        {
            // 此处以配置各端口的 LED_ID_1 (LED1) 为例
            // led_cfg 采用按值传递方式
            ret = yt_led_action_set(unit, port, LED_ID_1, led_cfg);
            if (ret != CMM_ERR_OK)
            {
                printk(KERN_INFO "[Motorcomm] Failed to set LED action for unit %d, port %d, error: %d\n", unit, port, ret);
                return ret;
            }
        }
    }

    printk(KERN_INFO "[Motorcomm] Successfully configured LED active blink mode for Unit 0~1, Port 1~4.\n");
    return CMM_ERR_OK;
}


/**
 * @brief 配置指定 Unit 的 Port 1 ~ Port 4 为有数据闪烁的 Parallel LED 模式
 * 
 * @param unit 芯片物理 Unit ID (例如 0 或 1)
 * @return yt_ret_t 返回 CMM_ERR_OK 表示成功，其他表示失败
 */
yt_ret_t config_led_parallel_activity_blink(yt_unit_t unit)
{
    yt_ret_t ret = CMM_ERR_OK;

    // 1. 全局使能该 Unit 的 LED 控制器
    ret = yt_led_enable(unit);
    if (ret != CMM_ERR_OK) {
        return ret;
    }

    // 2. 将 LED 模式切换为 Parallel（并口）模式
    ret = yt_led_mode_set(unit, (yt_led_mode_t)LED_MODE_PARALLEL);
    if (ret != CMM_ERR_OK) {
        return ret;
    }

    // 3. 配置并口输出的端口掩码 (Port 1 ~ Port 4 对应的 Bit 1 ~ Bit 4 置 1)
    // 修正：先清零结构体，再对 portbits 数组的第 0 个字进行赋值
    yt_port_mask_t port_mask;
    memset(&port_mask, 0, sizeof(yt_port_mask_t));
    port_mask.portbits[0] = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);

    ret = yt_led_parallel_output_set(unit, port_mask);
    if (ret != CMM_ERR_OK) {
        return ret;
    }

    // 4. 定义并初始化 LED 指示行为配置结构体
    yt_led_act_cfg_t led_cfg;
    memset(&led_cfg, 0, sizeof(yt_led_act_cfg_t)); // 务必清零以防随机脏数据

    // 设定指示规则：任何速率 Link 建立时常亮
    led_cfg.spd10m_on_en   = 1;
    led_cfg.spd100m_on_en  = 1;
    led_cfg.spd1000m_on_en = 1;

    // 设定指示规则：有数据收发（RX/TX）时闪烁
    led_cfg.rxact_blink_en = 1;
    led_cfg.txact_blink_en = 1;

    // 5. 循环配置 Port 1 到 Port 4 的 LED 行为（本例以 LED1 为例）
    for (yt_port_t port = 1; port <= 4; port++) {
        ret = yt_led_action_set(unit, port, LED_ID_1, led_cfg);
        if (ret != CMM_ERR_OK) {
            return ret; // 如果某个端口配置失败，则中断返回
        }
    }

    return CMM_ERR_OK;
}

/**
 * @brief 业务调用入口：同时配置两个 Unit 的 LED
 */
void app_led_init(void)
{
    yt_ret_t ret;
    yt_unit_t unit;

    // 配置Unit
    for (unit = 0; unit < 2; unit++)
    {
        ret = config_led_parallel_activity_blink(unit);
        if (ret != CMM_ERR_OK) {
            printk(KERN_INFO "[Motorcomm] Failed to enable LED for unit %d, error: %d\n", unit, ret);
        }
    }
}

static int __init yt_init_module(void)
{
    uint8_t u;
    int ret;
    
    printk(KERN_INFO "[Motorcomm] Auto-initializing Dual Switch SDK (Late level)...\n");
    ret = yt_init();
    printk(KERN_INFO "[Motorcomm] yt_init completed with return code: %d\n", ret);

    // 注册/proc/smi /proc/mib接口
    // smi_mib_proc_test();
    // printk(KERN_INFO "[Motorcomm] smi_mib_proc_test called.");

    struct yt_port_force_ctrl_s unit_port_ctrl;
    unit_port_ctrl.speed_dup = PORT_SPEED_DUP_1000FULL;
    unit_port_ctrl.rx_fc_en = 1;
    unit_port_ctrl.tx_fc_en = 1;
    
    // 1. 自动使能两颗芯片的所有物理端口
    printk(KERN_INFO "[Motorcomm] Enabling forwarding on all Switch ports...\n");
    for (u = 0; u < 2; u++) {
        // 设置GMAC9 RGMII模式
        yt_port_enable_set(u, 6, 1);
        yt_port_extif_mode_set(u, 6, YT_EXTIF_MODE_RGMII);
        // 设置GMAC9 FORCE 1G Full
        yt_port_mac_force_set(u, 6, unit_port_ctrl);
    }

    // set_ports_led_active_blink();
    app_led_init();
    
    printk(KERN_INFO "[Motorcomm] All Switch ports successfully configured!\n");
    return 0;
}
late_initcall(yt_init_module);
