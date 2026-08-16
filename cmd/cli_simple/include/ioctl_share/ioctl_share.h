/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __IOCTL_SHARE_H__
#define __IOCTL_SHARE_H__
#include "yt_types.h"
#include "yt_port.h"
#include  <fcntl.h>
#include <linux/input.h>

#define CLI_MAX_TLV         8
#define CMDIOCTL        _IOWR('y', 0x23, struct cli_ioctl_data_s)

struct cli_debug_param_s
{
    yt_port_t port;
    uint8_t regmode;
    uint8_t phytype;
    uint32_t regAddr;
    uint32_t *regData;
    uint16_t *regDataHalf;
    uint16_t countNum;
};
typedef struct cli_debug_param_s  cli_debug_param_t;

enum intercomm_cmd_id_e
{
    REG_READ_REGADDR_SET = 1,
    REG_WRITE_REGADDR_VALUE_SET,
    PHY_REG_REGADDR_READ,
    PHY_REG_REGADDR_WRITE,
    PORT_SET_PORT_EXTIFMODE,
    PORT_GET_PORT_EXTIFMODE,
    PORT_GET_PORT_LINKSTATUS,
    PORT_SET_PORT_AUTO_NEG_STATE,
    PORT_GET_PORT_AUTO_NEG_STATE,
    PORT_SET_PORT_STATE,
    PORT_GET_PORT_STATE,
    PORT_SET_PORT_DUPLEX_SPEED,
    PORT_SET_PORT_FC_STATE,
    VLAN_SET_PORT_PVID,
    VLAN_SET_VID_MEMBERPORTS_UNTAGPORTS,
    MIB_SET_STATE,
    MIB_GET_STATE,
    MIB_GET_PORT,
    MIB_GET_CLEAR,
    FDB_LEARN_STATE,
    SWITCH_INIT,
    SWITCH_EXIT,
};
typedef enum intercomm_cmd_id_e intercomm_cmd_id_t;

struct cli_ioctl_hdr_s
{
    uint16_t cmd;
    /* ret is the return value of YT API */
    yt_ret_t ret;
    uint32_t flags;
    uint8_t unit;
};
typedef struct cli_ioctl_hdr_s cli_ioctl_hdr_t;

struct cli_ioctl_tlv_hdr_s
{
    uint16_t type;
    /* val's len  */
    uint16_t len;
};
typedef struct cli_ioctl_tlv_hdr_s cli_ioctl_tlv_hdr_t;

struct cli_ioctl_tlv_s
{
    cli_ioctl_tlv_hdr_t tlvhdr;
    void *val;
};
typedef struct cli_ioctl_tlv_s cli_ioctl_tlv_t;

struct cli_ioctl_data_s
{
    cli_ioctl_hdr_t hdr;
    cli_ioctl_tlv_t tlv[CLI_MAX_TLV];
};
typedef struct cli_ioctl_data_s cli_ioctl_data_t;

#endif // __IOCTL_SHARE_H__