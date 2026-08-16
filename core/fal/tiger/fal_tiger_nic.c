/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"
#include "fal_tiger_nic.h"
#include "fal_tiger_port.h"
#include "fal_tiger_l2.h"
#include "fal_tiger_vlan.h"
#include "fal_tiger_l2.h"
#ifdef INTER_MCU
#include "eth_export.h"
#endif

yt_ret_t  fal_tiger_nic_init(yt_unit_t unit)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2_learn_per_port_ctrl;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, FAL_INTERNAL_CPU_MACID, sizeof(l2_learn_per_port_ctrl), &l2_learn_per_port_ctrl), ret);
    /* disable internal cpu port learn fdb*/
    HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_DISABLEf, &l2_learn_per_port_ctrl, YT_ENABLE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_PER_PORT_CTRLNm, FAL_INTERNAL_CPU_MACID, sizeof(l2_learn_per_port_ctrl), &l2_learn_per_port_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_cpuport_mode_set(yt_unit_t unit, yt_cpuport_mode_t mode)
{
    cpu_copy_dst_ctrl_t tmp_entry;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&tmp_entry, 0, sizeof(cpu_copy_dst_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, CPU_COPY_DST_CTRLm, 0, sizeof(cpu_copy_dst_ctrl_t), &tmp_entry), ret);
    if (mode == CPUPORT_MODE_INTERNAL)
    {
        HAL_FIELD_SET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_INT_CPUf, &tmp_entry, YT_ENABLE);
        HAL_FIELD_SET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_EXT_CPUf, &tmp_entry, YT_DISABLE);
    }
    else if (mode == CPUPORT_MODE_EXTERNAL)
    {
        HAL_FIELD_SET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_INT_CPUf, &tmp_entry, YT_DISABLE);
        HAL_FIELD_SET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_EXT_CPUf, &tmp_entry, YT_ENABLE);
    }
    else
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, CPU_COPY_DST_CTRLm, 0, sizeof(cpu_copy_dst_ctrl_t), &tmp_entry), ret);  

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_cpuport_mode_get(yt_unit_t unit, yt_cpuport_mode_t *pmode)
{
    cpu_copy_dst_ctrl_t tmp_entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t to_ext_cpu;

    osal_memset(&tmp_entry, 0, sizeof(cpu_copy_dst_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, CPU_COPY_DST_CTRLm, 0, sizeof(cpu_copy_dst_ctrl_t), &tmp_entry), ret);
    HAL_FIELD_GET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_EXT_CPUf, &tmp_entry, &to_ext_cpu);
    *pmode = to_ext_cpu ? CPUPORT_MODE_EXTERNAL : CPUPORT_MODE_INTERNAL;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cpuport_en_set(yt_unit_t unit,  yt_enable_t enable)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t ext_cpuport;
    uint32_t macid;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_SET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EXT_CPU_PORT_CTRLm, 0, sizeof(ext_cpu_port_ctrl_t), &entry), ret);

    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_NUMf, &entry, &macid);
    CAL_MAC_TO_YTP(unit, macid, ext_cpuport);
    /* enable the backpress on cpu port */
#ifdef PORT_INCLUDED
    fal_tiger_port_backpress_enable_set(unit, ext_cpuport, (enable?YT_DISABLE:YT_ENABLE));
#endif
    /* l2 learn on port */
#ifdef L2_INCLUDED
    fal_tiger_l2_port_learn_en_set(unit, ext_cpuport, (enable?YT_DISABLE:YT_ENABLE));
#endif
    /* udpate yt ext cpu port bit in portmask
    CMM_PLIST_UPDATE(unit, ext_cpuport, (enable?FALSE:TRUE)); */
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cpuport_en_get(yt_unit_t unit,  yt_enable_t *pEnable)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_ENf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cpuport_port_set(yt_unit_t unit,  yt_port_t port)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    uint32_t enable;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_SET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_NUMf, &entry, macid);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EXT_CPU_PORT_CTRLm, 0, sizeof(ext_cpu_port_ctrl_t), &entry), ret);

    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_ENf, &entry, &enable);
    if(enable == YT_ENABLE)
    {
        /* disable the backpress on cpu port */
#ifdef PORT_INCLUDED
        fal_tiger_port_backpress_enable_set(unit, port, YT_DISABLE);
#endif
        /* l2 learn on port */
#ifdef L2_INCLUDED
        fal_tiger_l2_port_learn_en_set(unit, port, YT_DISABLE);
#endif
        /* udpate yt ext cpu port bit in portmask
        CMM_PLIST_UPDATE(unit, port, FALSE); */
     }
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cpuport_port_get(yt_unit_t unit,  yt_port_t *pport)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t macid;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_NUMf, &entry, &macid);
    CAL_MAC_TO_YTP(unit, macid, (*pport));

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_cpuport_tagtpid_set(yt_unit_t unit, uint16_t tpid)
{
    cpu_tag_tpid_t cpu_tag_tpid;
    cmm_err_t ret = CMM_ERR_OK;

    HAL_FIELD_SET(CPU_TAG_TPIDm, CPU_TAG_TPID_TPIDf, &cpu_tag_tpid, tpid);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, CPU_TAG_TPIDm, 0, sizeof(cpu_tag_tpid_t), &cpu_tag_tpid), ret);
   
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_cpuport_tagtpid_get(yt_unit_t unit, uint16_t *ptpid)
{
    cpu_tag_tpid_t cpu_tag_tpid;
    uint32_t tpid;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, CPU_TAG_TPIDm, 0, sizeof(cpu_tag_tpid_t), &cpu_tag_tpid), ret);
    HAL_FIELD_GET(CPU_TAG_TPIDm, CPU_TAG_TPID_TPIDf, &cpu_tag_tpid, &tpid);
    *ptpid = tpid;
   
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cputag_en_set(yt_unit_t unit,  yt_enable_t enable)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_SET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_TAG_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EXT_CPU_PORT_CTRLm, 0, sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cputag_en_get(yt_unit_t unit,  yt_enable_t *pEnable)
{
    ext_cpu_port_ctrl_t entry;
    uint32_t enable;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_TAG_ENf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_nic_rx_register(yt_unit_t unit, yt_nic_rx_cb_f rx_cb)
{
#ifdef INTER_MCU
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(eth_rx_register(rx_cb), ret);
#else
	CMM_UNUSED_PARAM(unit);
	CMM_UNUSED_PARAM(rx_cb);
#endif
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_nic_rx_handler(yt_unit_t unit)
{
#ifdef INTER_MCU
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(eth_rx_handler(), ret);
#else
	CMM_UNUSED_PARAM(unit);
#endif
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_nic_tx(yt_unit_t unit, yt_tx_pkt_t *p_tx_pkt, uint8_t mem_free_flag)
{
#ifdef INTER_MCU
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t macmask;
    yt_port_mask_t port_member;
    yt_port_mask_t untag_port_member;
    uint16_t lookup_index = 0;
    yt_l2_fdb_info_t fdb_info;
    yt_mac_addr_t mac_addr;
    uint8_t index;
    
    if (NIC_TX_BY_PORTLIST == p_tx_pkt->tx_pkt_info.tx_flag)
    {
        CMM_PARAM_CHK((0 == p_tx_pkt->tx_pkt_info.port_mask.portbits[0]), CMM_ERR_PORTLIST);
        CAL_YTPLIST_TO_MLIST(unit, p_tx_pkt->tx_pkt_info.port_mask, macmask);        
    }
    else if (NIC_TX_BY_VLAN == p_tx_pkt->tx_pkt_info.tx_flag)
    {
        for (index = 0; index < MAC_ADDR_LEN; index++)
        {
            mac_addr.addr[index] = p_tx_pkt->p_pkt_buff[index];
        }
        
        ret = fal_tiger_l2_fdb_get(unit, p_tx_pkt->tx_pkt_info.vlan_id, mac_addr, &fdb_info, &lookup_index);
        if (CMM_ERR_ENTRY_NOT_FOUND == ret)
        {
            CMM_ERR_CHK(fal_tiger_vlan_port_get(unit, p_tx_pkt->tx_pkt_info.vlan_id, &port_member, &untag_port_member), ret);
            CMM_PARAM_CHK((0 == port_member.portbits[0]), CMM_ERR_PORTLIST);
            
            CAL_YTPLIST_TO_MLIST(unit, port_member, macmask);
        }
        else
        {
            macmask.portbits[0] = fdb_info.DST_PORT_MASK;
        }                    
    }
    else
    {
        macmask.portbits[0] = 0;
    }

    CMM_ERR_CHK(eth_tx_by_portlist(p_tx_pkt->p_pkt_buff, p_tx_pkt->pkt_len, macmask.portbits[0], mem_free_flag), ret);
#else
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(p_tx_pkt);
    CMM_UNUSED_PARAM(mem_free_flag);
#endif
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_nic_eth_buf_cnt_len_set(yt_unit_t unit, uint16_t buf_cnt, uint16_t tx_buf_cnt, uint16_t buf_len)
{
#ifdef INTER_MCU
    cmm_err_t ret = CMM_ERR_OK;
    CMM_ERR_CHK(eth_buf_cnt_len_set(buf_cnt, tx_buf_cnt, buf_len), ret);
#else
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(buf_cnt);
    CMM_UNUSED_PARAM(tx_buf_cnt);
    CMM_UNUSED_PARAM(buf_len);
#endif

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_nic_eth_debug_flag_set(yt_unit_t unit, uint8_t debug_flag)
{
    CMM_UNUSED_PARAM(unit);
#ifdef INTER_MCU
    cmm_err_t ret = CMM_ERR_OK;
    CMM_ERR_CHK(eth_debug_flag_set(debug_flag), ret);
#else
    CMM_UNUSED_PARAM(debug_flag);
#endif

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_nic_eth_drv_init(yt_unit_t unit)
{
#ifdef INTER_MCU
    cmm_err_t ret = CMM_ERR_OK;
    CMM_ERR_CHK(eth_init(), ret);
#else
    CMM_UNUSED_PARAM(unit);
#endif

    return CMM_ERR_OK;
}


