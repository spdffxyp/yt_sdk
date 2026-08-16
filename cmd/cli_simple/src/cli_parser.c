/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************/
#include "cli_parser.h"
#include "cli_types.h"
#include "osal_mem.h"
#include "osal_print.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cli_abstr.h"
#include "yt_exit.h"
#include <ctype.h>
#include "securec.h"

uint32_t
cli_input_commnad_read(char *buf, uint32_t buf_size)
{
    uint32_t nbytes = 0;
    nbytes = osal_read(0, buf, buf_size);
    return nbytes;
}

void
cli_prompt(void)
{
    osal_write(STDOUT_FILENO, CLI_PROMPT, sizeof(CLI_PROMPT));
}

char *
extifmode_enum_type_to_string(yt_extif_mode_t type)
{
    char *result;
    switch (type)
    {
        case YT_EXTIF_MODE_MII:
            result = "MII";
            break;
        case YT_EXTIF_MODE_REMII:
            result = "REMII";
            break;
        case YT_EXTIF_MODE_RMII_MAC:
            result = "RMII_MAC";
            break;
        case YT_EXTIF_MODE_RMII_PHY:
            result = "RMII_PHY";
            break;
        case YT_EXTIF_MODE_RGMII:
            result = "RGMII";
            break;
        case YT_EXTIF_MODE_XMII_DISABLE:
            result = "XMII_DISABLE";
            break;
        case YT_EXTIF_MODE_SG_MAC:
            result = "SG_MAC";
            break;
        case YT_EXTIF_MODE_SG_PHY:
            result = "SG_PHY";
            break;
        case YT_EXTIF_MODE_FIB_1000:
            result = "FIB_1000";
            break;
        case YT_EXTIF_MODE_FIB_100:
            result = "FIB_100";
            break;
        case YT_EXTIF_MODE_BX2500:
            result = "BX2500";
            break;
        case YT_EXTIF_MODE_SGFIB_AS:
            result = "SGFIB_AS";
            break;
        default:
            result = "Unsupported exitifmode";
            break;
    }
    return result;
}

char *
speed_status_enum_type_to_string(yt_port_link_status_t speed_status)
{
    char *speedstatus;
    switch (speed_status)
    {
        case PORT_LINK_DOWN:
            speedstatus = "down";
            break;
        case PORT_LINK_UP:
            speedstatus = "up";
            break;
        default:
            speedstatus = "Unsupported speed status";
            break;
    }
    return speedstatus;
}

char *
speed_enum_type_to_string(yt_port_speed_t speed)
{
    char *sp;
    switch (speed)
    {
        case PORT_SPEED_10M:
            sp = "10M";
            break;
        case PORT_SPEED_100M:
            sp = "100M";
            break;
        case PORT_SPEED_1000M:
            sp = "1G";
            break;
        case PORT_SPEED_2500M:
            sp = "2.5G";
            break;
        case PORT_SPEED_END:
        default:
            sp = "Unsupported speed";
            break;  
    }
    return sp;
}

char *
duplex_enum_type_to_string(yt_port_duplex_t duplex)
{
    char *duplex_result;
    switch (duplex)
    {
        case PORT_DUPLEX_HALF:
            duplex_result = "half";
            break;
        case PORT_DUPLEX_FULL:
            duplex_result = "full";
            break;
        default:
            duplex_result = "Unsupported duplex";
            break;
    }
    return duplex_result;
}

void
cli_usage(void)
{
    cli_print_out("Switch CLI Usage:\n");
    /* register read write directly */
    cli_print_out("switch init                                            --switch init\n");
    cli_print_out("quit                                                   --switch quit\n");
    cli_print_out("switch unit [UNITID]                                   --switch unit, UNITID: <0~3>\n");
#ifdef SYS_INCLUDED
    cli_print_out("reg read regaddr [REGADDR]                             --register read. REGADDR e.g. 0x80001\n");
    cli_print_out("reg write regaddr [REGADDR] value [VALUE]              --register write.REGADDR e.g. 0x80001, VALUE 0x1\n");
#endif
#ifdef PORT_INCLUDED
    cli_print_out("phy read port [PORT] phytype [TYPE] regmode [MODE] regaddr [REGADDR]  --phy register read.\n \
                                                        [PORT]:uint, min:0 max:depends on switch\n \
                                                        [TYPE]:intphy,extphy, [MODE]:mii,ext.\n");
    cli_print_out("phy write port [PORT] phytype [TYPE] regmode [MODE] regaddr [REGADDR] value VALUE  --phy register write. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n \
                                                        [TYPE]:intphy,extphy, [MODE]:mii,ext.\n");
    /* port config */
    cli_print_out("port set port [PORT] extifmode [MODE]                  --port mode set. \n \
                                                        [PORT]:uint, min:0 max:depends on switch, \n \
                                                        MODE <0~11> e.g. \n \
                                                        0:    YT_EXTIF_MODE_MII, \n \
                                                        1:    YT_EXTIF_MODE_REMII\n \
                                                        2:    YT_EXTIF_MODE_RMII_MAC\n \
                                                        3:    YT_EXTIF_MODE_RMII_PHY\n \
                                                        4:    YT_EXTIF_MODE_RGMII\n \
                                                        5.    YT_EXTIF_MODE_XMII_DISABLE\n \
                                                        6:    YT_EXTIF_MODE_SG_MAC\n \
                                                        7:    YT_EXTIF_MODE_SG_PHY\n \
                                                        8:    YT_EXTIF_MODE_FIB_1000\n \
                                                        9:    YT_EXTIF_MODE_FIB_100\n \
                                                        10:   YT_EXTIF_MODE_BX2500\n \
                                                        11:   YT_EXTIF_MODE_SGFIB_AS\n");
    cli_print_out("port get port [PORT] extifmode                         --port mode get. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n");
    cli_print_out("port get port [PORT] linkstatus                        --port linkstatus get. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n");
    cli_print_out("port set port [PORT] auto_neg state [STATE]            --port auto neg set. \n \
                                                        [PORT]:uint, min:0 max:depends on switch,\n \
                                                        [STATE]:enable,disable.\n");
    cli_print_out("port get port [PORT] auto_neg state                    --port auto neg get. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n");
    cli_print_out("port set port [PORT] state [STATE]                     --port state set. \n \
                                                        [PORT]:uint, min:0 max:depends on switch,\n \
                                                        [STATE]:enable,disable\n");
    cli_print_out("port get port [PORT] state                             --port state get. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n");
    cli_print_out("port set port [PORT] fc_en [FC_EN]                     --port fc state set. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n \
                                                        [FC_EN]:enable,disable \n");
    cli_print_out("port set port [PORT] speed_duplex [SPEED_DUPLEX]       --port speed set. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n \
                                                        [SPEED_DUPLEX]:\n \
                                                        10half:    PORT_SPEED_DUP_10HALF\n \
                                                        10full:    PORT_SPEED_DUP_10FULL\n \
                                                        100half:    PORT_SPEED_DUP_100HALF\n \
                                                        100full:    PORT_SPEED_DUP_100FULL\n \
                                                        1000full:    PORT_SPEED_DUP_1000FULL\n");
#endif
#ifdef VLAN_INCLUDED
    /* vlan config */
    cli_print_out("vlan set port [PORT] pvid [PVID]                       --vlan port pvid set. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n \
                                                        [PVID]:<1~4094>\n");                      
    cli_print_out("vlan set vid [VID] memberports [MASK] untagports [MASK]--vlan member/untag port set\n \
                                                        [VID]:<1~4094>, [MASK] e.g. 0x11 \n");
#endif
#ifdef STAT_INCLUDED
    /* mib config */
    cli_print_out("mib set state [STATE]                                  --mib state set.[STATE]:enable/disable\n");
    cli_print_out("mib get state                                          --mib state get\n");
    cli_print_out("mib get port [PORT]                                    --mib get by port index. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n");
    cli_print_out("mib clear port [PORT]                                  --mib clear by port index. \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n");
    cli_print_out("mib clear all                                          --mib clear by all \n");
#endif
#ifdef L2_INCLUDED
    /* fdb learn */
    cli_print_out("fdb learn port [PORT] state [STATE]                    --fdb learn state set.  \n \
                                                        [PORT]:uint, min:0 max:depends on switch.\n \
                                                        [STATE]:enable,disable\n");
#endif
}

cli_items_p
cli_split_input_buf(char *string)
{
    char *cp, *start, *token;
    char *tstart, *tend, *string2;
    uint32_t strlen;
    cli_items_p desc_items;

    if (NULL == string)
    {
        return NULL;
    }
    /* support "cmdline" parse*/
    string2 = osal_strdup(string);
    if ((tstart = osal_strchr(string2, '"')) != NULL)
    {
        tend = osal_strchr(tstart + 1, '"');
        if (tend != NULL)
        {
            *tstart = ' ';
            tstart ++;
            while (tstart < tend)
            {
                if (*tstart == ' ')
                {
                    *tstart = '-';
                }
                tstart ++;
            }
            *tend = ' ';
        }
    }

    cp = string2;

    /* skip the white space in cmdline*/
    while (osal_isspace((int) * cp) && *cp != '\0')
    {
        cp++;
    }

    /* Return if there is only white spaces */
    if (*cp == '\0')
    {
        osal_free(string2);
        string2 = NULL;
        return NULL;
    }

    /* maybe comment not a cmdline*/
    if (*cp == '!' || *cp == '#')
    {
        osal_free(string2);
        string2 = NULL;
        return NULL;
    }

    /* Prepare return items. */
    desc_items = cli_items_attr_init (CLI_ALLOCATED_MIN_SIZE);
    if (NULL == desc_items)
    {
        cli_print_out("malloc err !\n");
        osal_free(string2);
        string2 = NULL;
        return NULL;
    }

    /* Copy each command piece and set into items. */
    while (1)
    {
        start = cp;

        while (!(osal_isspace((int) * cp) || *cp == '\r' || *cp == '\n') && *cp != '\0')
        {
            cp++;
        }

        strlen = cp - start;
        token = osal_malloc(strlen + 1);
        if (NULL == token)
        {
            cli_print_out("malloc err !\n");
            cli_free_items(desc_items);
            osal_free(string2);
            string2 = NULL;
            return NULL;
        }

        memcpy_s(token, strlen + 1, start, strlen);
        *(token + strlen) = '\0';

        cli_items_set(desc_items, token);
        while ((osal_isspace((int) * cp) || *cp == '\n' || *cp == '\r') && *cp != '\0')
        {
            cp++;
        }

        if (*cp == '\0')
        {
            osal_free(string2);
            string2 = NULL;
            return desc_items;
        }
    }
}

cli_items_p
cli_items_attr_init (uint32_t size)
{
    cli_items_p items = osal_calloc (1, sizeof (struct cli_items_s));

    if (!items)
    {
        cli_print_out("malloc err!\n");
        return NULL;
    }

    /* allocate at least one slot */
    if (size == 0)
    {
        size = 1;
    }

    items->allocated = size;
    items->max = 0;
    items->index = osal_calloc (1, sizeof (void *) * size);

    if (!items->index)
    {
        osal_free(items);
        items = NULL;
        cli_print_out("malloc err!\n");
        return NULL;
    }

    return items;
}

uint32_t
cli_print_out (const char *format, ...)
{
    va_list args;

    va_start (args, format);
    vprintf (format, args);
    va_end (args);

    return 0;
}

uint32_t
cli_str_convert_uint(char* str, cli_exe_result_t *ret)
{
    uint32_t i;
    uint32_t len;
    uint32_t digit;
    uint32_t limit, remain;
    uint32_t max = 0xFFFFFFFF;
    uint32_t total = 0;

    /* Sanify check. */
    if (str == NULL || ret == NULL)
    {
        return CLI_EXE_PARA_INVALID;
    }

    /* First set return value as error. */
    *ret = CLI_EXE_PARA_INVALID;

    len = osal_strlen(str);

    /*add for suport parser hex format*/
    if (len >= 2 && !osal_memcmp(str, "0x", 2))
    {
        if (len == 2)
        {
            *ret = CLI_EXE_PARA_INVALID;
            return 0xFFFFFFFF;
        }
        else if (len > 10)
        {
            *ret = CLI_EXE_PARA_INVALID;
            return 0xFFFFFFFF;
        }

        for (i = 2; i < len; i++)
        {
            if ((*(str + i) <= '9' && *(str + i) >= '0')
                || (*(str + i) <= 'f' && *(str + i) >= 'a')
                || (*(str + i) <= 'F' && *(str + i) >= 'A'))
            {
                /*do nothing*/
            }
            else
            {
                *ret = CLI_EXE_PARA_INVALID;
                return 0xFFFFFFFF;
            }
        }

        total = cmd_strtou32(str, NULL, 16);
    }
    else
    {

        limit = max / 10;
        remain = max % 10;

        if (len < DECIMAL_STRLEN_MIN || len > DECIMAL_STRLEN_MAX)
        {
            *ret = CLI_EXE_PARA_INVALID;
            return 0xFFFFFFFF;
        }

        for (i = 0; i < len; i++)
        {
            if (*str < '0' || *str > '9')
            {
                *ret = -1;
                return 0xFFFFFFFF;
            }

            digit = *str++ - '0';

            if (total > limit || (total == limit && digit > remain))
            {
                *ret = CLI_EXE_PARA_INVALID;
                return 0xFFFFFFFF;
            }

            total = total * 10 + digit;
        }
    }

    *ret = CLI_EXE_SUCCESS;
    return total;
}

void
cli_str_convert_portmask(char* str, cli_exe_result_t* ret, yt_port_mask_t* portMask)
{
    uint32_t tmp = 0;
    uint32_t i = 0;

    tmp = cli_str_convert_uint(str, ret);
    for (i = 0; i < YT_PORTS_WORD_NUM; i++)
    {
        portMask->portbits[i] = tmp;
    }
    return;
}

cli_exe_result_t
cli_reg_read(cli_items_p vline)
{
    uint32_t regAddr = 0;
    uint32_t regData = 0;
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    regAddr = cli_str_convert_uint(vline->index[3], &ret);
    if (!CLI_ABSTR_OPS->cli_reg_read_regaddr)
    {
        cli_print_out("No callback function. \n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_reg_read_regaddr(unitId, regAddr, &regData);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_reg_read_regaddr failed.", ret);
        return CLI_EXE_FAIL;
    }
    cli_print_out("read %x : 0x%x\n", regAddr, regData);
    return ret;
}

cli_exe_result_t
cli_phy_reg_read(cli_items_p vline)
{
    uint32_t port = 0;
    uint32_t regAddr = 0;
    uint16_t regData = 0;
    cli_phy_reg_type_t regmode;
    cli_phy_type_t phytype;
    cli_exe_result_t ret = CLI_EXE_SUCCESS;

    port = cli_str_convert_uint(vline->index[3], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the port is invalid.\n");
        return CLI_EXE_PARA_INVALID;
    }

    if (!osal_strcmp(vline->index[5], "extphy"))
    {
        phytype = EXT_PHY;
    }
    else if (!osal_strcmp(vline->index[5], "intphy"))
    {
        phytype = INT_PHY;
    }
    else
    {
        cli_print_out("the regType is invalid.\n");
        return CLI_EXE_PARA_INVALID;
    }
    if (!osal_strcmp(vline->index[7], "mii"))
    {
        regmode = MII;
    }
    else if (!osal_strcmp(vline->index[7], "ext"))
    {
        regmode = EXT;
    }
    else
    {
        cli_print_out("the regmode is invalid.\n");
        return CLI_EXE_PARA_INVALID;
    }

    regAddr = cli_str_convert_uint(vline->index[9], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the regAddr is invalid.\n");
        return CLI_EXE_PARA_INVALID;
    }

    if (!CLI_ABSTR_OPS->cli_phy_read_port_regmode_regaddr)
    {
        cli_print_out("No callback function.\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_phy_read_port_regmode_regaddr(unitId, port, phytype, regmode, regAddr, &regData);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("phy_read_port_regmode_regaddr", ret);
        return CLI_EXE_FAIL;
    }

    if (MII == regmode)
    {
        cli_print_out("port  :  %d,    MII regAddr  :  0x%x,     read data  :  0x%x\n", port, regAddr, regData);
    }
    else if (EXT == regmode)
    {
        cli_print_out("port  :  %d,    EXT regAddr  :  0x%x,     read data  :  0x%x\n", port, regAddr, regData);
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_phy_reg_write(cli_items_p vline)
{
    uint32_t port = 0;
    uint32_t regAddr = 0;
    uint16_t regData = 0;
    cli_phy_reg_type_t regmode;
    cli_phy_type_t phytype;
    cli_exe_result_t ret = CLI_EXE_SUCCESS;

    port = cli_str_convert_uint(vline->index[3], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the port is invalid.\n");
        return CLI_EXE_PARA_INVALID;
    }

    if (!osal_strcmp(vline->index[5], "extphy"))
    {
        phytype = EXT_PHY;
    }
    else if (!osal_strcmp(vline->index[5], "intphy"))
    {
        phytype = INT_PHY;
    }
    else
    {
        cli_print_out("the phy type is invalid. \n");
        return CLI_EXE_PARA_INVALID;
    }
    if (!osal_strcmp(vline->index[7], "mii"))
    {
        regmode = MII;
    }
    else if (!osal_strcmp(vline->index[7], "ext"))
    {
        regmode = EXT;
    }
    else
    {
        cli_print_out("the regmode is invalid. \n");
        return CLI_EXE_PARA_INVALID;
    }

    regAddr = cli_str_convert_uint(vline->index[9], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the regAddr is invalid. \n");
        return CLI_EXE_PARA_INVALID;
    }

    regData = cli_str_convert_uint(vline->index[11], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the regData is invalid. \n");
        return CLI_EXE_PARA_INVALID;
    }

    if (!CLI_ABSTR_OPS->cli_phy_write_port_regmode_regaddr_value)
    {
        cli_print_out("No callback function.\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_phy_write_port_regmode_regaddr_value(unitId, port, phytype, regmode, regAddr, regData);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_phy_write_port_regmode_regaddr_value", ret);
        return CLI_EXE_FAIL;
    }
    return ret;
}

cli_exe_result_t
cli_reg_write(cli_items_p vline)
{
    uint32_t regData = 0;
    uint32_t regAddr = 0;
    cli_exe_result_t ret = CLI_EXE_SUCCESS;

    regAddr = cli_str_convert_uint(vline->index[3], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the regAddr is invalid \n");
        return CLI_EXE_FAIL;
    }

    regData = cli_str_convert_uint(vline->index[5], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the regData is invalid \n");
        return CLI_EXE_FAIL;
    }
    if (!CLI_ABSTR_OPS->cli_reg_write_regaddr_value)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_reg_write_regaddr_value(unitId, regAddr, regData);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_reg_write_regaddr_value", ret);
        return CLI_EXE_FAIL;
    }
    return ret;
}

cli_exe_result_t
cli_reg_execute(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;

    if (vline == NULL)
    {
        return CLI_EXE_FAIL;
    }
    if (vline->max < 4)
    {
        return CLI_EXE_UNKNOW_COMMAND;
    }
    if (vline->max == 4 && !osal_strcmp(vline->index[0], "reg")
        && !osal_strcmp(vline->index[1], "read")
        && !osal_strcmp(vline->index[2], "regaddr"))
    {
        ret = cli_reg_read(vline);
    }
    else if (vline->max == 6 && !osal_strcmp(vline->index[0], "reg"))
    {
        ret = cli_reg_write(vline);
    }
    else
    {
        ret = CLI_EXE_UNKNOW_COMMAND;
    }
    return ret;
}

cli_exe_result_t
cli_phy_reg_execute(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;

    if (vline == NULL)
    {
        return CLI_EXE_FAIL;
    }
    if (vline->max < 10 || vline->max > 12)
    {
        return CLI_EXE_UNKNOW_COMMAND;
    }
    if (vline->max == 10 && !osal_strcmp(vline->index[0], "phy")
        && !osal_strcmp(vline->index[1], "read")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "phytype")
        && !osal_strcmp(vline->index[6], "regmode")
        && !osal_strcmp(vline->index[8], "regaddr"))
    {
        ret = cli_phy_reg_read(vline);
    }
    else if (vline->max == 12 && !osal_strcmp(vline->index[0], "phy")
        && !osal_strcmp(vline->index[1], "write")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "phytype")
        && !osal_strcmp(vline->index[6], "regmode")
        && !osal_strcmp(vline->index[8], "regaddr")
        && !osal_strcmp(vline->index[10], "value"))
    {
        ret = cli_phy_reg_write(vline);
    }
    else
    {
        ret = CLI_EXE_UNKNOW_COMMAND;
    }
    return ret;
}


cli_exe_result_t
cli_get_port_extifmode(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_get_port_extifmode_params_t get_port_extifmode_params;

    get_port_extifmode_params.port = cli_str_convert_uint(vline->index[3], &ret);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the port para is invalid. \n");
        return CLI_EXE_FAIL;
    }
    if (!CLI_ABSTR_OPS->cli_port_get_port_extifmode)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }
    ret = CLI_ABSTR_OPS->cli_port_get_port_extifmode(unitId, &get_port_extifmode_params);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_get_port_extifmode", ret);
        return CLI_EXE_FAIL;
    }

    cli_print_out("port : %8d,   extifmode: %+8s \n", get_port_extifmode_params.port, extifmode_enum_type_to_string(get_port_extifmode_params.extif_mode));
    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_get_port_linkstatus(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_get_port_linkstatus_params_t port_linkstatus;

    memset_s(&port_linkstatus, sizeof(cli_get_port_linkstatus_params_t), 0, sizeof(cli_get_port_linkstatus_params_t));

    port_linkstatus.port = cli_str_convert_uint(vline->index[3], &ret);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the port para is invalid. \n");
        return CLI_EXE_FAIL;
    }
    if (!CLI_ABSTR_OPS->cli_port_get_port_linkstatus)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_port_get_port_linkstatus(unitId, &port_linkstatus);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED( "cli_port_get_port_linkstatus", ret);
        return ret;
    }
    cli_print_out("port : %8d \n", port_linkstatus.port);
    cli_print_out("link_status : %+8s \n", speed_status_enum_type_to_string(port_linkstatus.linkstatus.link_status));
    cli_print_out("link_speed : %+8s \n", speed_enum_type_to_string(port_linkstatus.linkstatus.link_speed));
    cli_print_out("link_duplex : %+8s \n", duplex_enum_type_to_string(port_linkstatus.linkstatus.link_duplex));
    cli_print_out("rx_fc_en : %8d \n", port_linkstatus.linkstatus.rx_fc_en);
    cli_print_out("tx_fc_en : %8d \n", port_linkstatus.linkstatus.tx_fc_en);

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_get_port_state(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    
    cli_get_port_state_params_t get_port_state;

    memset_s(&get_port_state, sizeof(cli_get_port_state_params_t), 0, sizeof(cli_get_port_state_params_t));

    get_port_state.port  = cli_str_convert_uint(vline->index[3], &ret);
    if (!CLI_ABSTR_OPS->cli_port_get_port_state)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_port_get_port_state(unitId, &get_port_state);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("port_get_port_state", ret);
        return ret;
    }
    cli_print_out("port : %8d,   port state: %8d \n", get_port_state.port, get_port_state.state);
    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_set_port_extifmode(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    uint32_t res;
    cli_port_extifmode_params_t port_extifmode;

    memset_s (&port_extifmode, sizeof(cli_port_extifmode_params_t), 0, sizeof(cli_port_extifmode_params_t));
    port_extifmode.port  = cli_str_convert_uint(vline->index[3], &ret);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the port para is invalid. \n");
        return CLI_EXE_FAIL;
    }
    res = cli_str_convert_uint(vline->index[5], &ret);
    port_extifmode.extif_mode = res;
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the mode para is invalid. \n");
        return CLI_EXE_FAIL;
    }
    if (!CLI_ABSTR_OPS->cli_port_set_port_extifmode)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_port_set_port_extifmode(unitId, port_extifmode);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_port_set_port_extifmode", ret);
        return CLI_EXE_FAIL;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_get_port_autoneg(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_get_port_autoneg_params_t get_port_autoneg;

    memset_s(&get_port_autoneg, sizeof(cli_get_port_autoneg_params_t), 0, sizeof(cli_get_port_autoneg_params_t));

    get_port_autoneg.port = cli_str_convert_uint(vline->index[3], &ret);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the port param is invalid \n");
        return ret;
    }
    if (!CLI_ABSTR_OPS->cli_port_get_port_auto_neg_state)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_port_get_port_auto_neg_state(unitId, &get_port_autoneg);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_port_get_port_auto_neg_state", ret);
        return ret;
    }
    cli_print_out("port : %8d, macAutoNeg : %8d\n", get_port_autoneg.port, get_port_autoneg.state);
    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_set_port_state(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_set_port_state_params_t set_port_state;

    memset_s (&set_port_state, sizeof(cli_set_port_state_params_t), 0, sizeof(cli_set_port_state_params_t));
    if (!osal_strcmp(vline->index[5], "enable"))
    {
        set_port_state.state = YT_ENABLE;
    }
    else if (!osal_strcmp(vline->index[5], "disable"))
    {
        set_port_state.state = YT_DISABLE;
    }
    else
    {
        return CLI_EXE_PARA_INVALID;
    }
    set_port_state.port  = cli_str_convert_uint(vline->index[3], &ret);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the port para is invalid. \n");
        return CLI_EXE_FAIL;
    }
    if (!CLI_ABSTR_OPS->cli_port_set_port_state)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_port_set_port_state(unitId, set_port_state);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_port_set_port_state", ret);
        return ret;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_set_port_autoneg(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_set_port_autoneg_params_t port_autoneg;

    memset_s (&port_autoneg, sizeof(cli_set_port_autoneg_params_t), 0, sizeof(cli_set_port_autoneg_params_t));
    if (!osal_strcmp(vline->index[6], "enable"))
    {
        port_autoneg.state = YT_ENABLE;
    }
    else if (!osal_strcmp(vline->index[6], "disable"))
    {
        port_autoneg.state = YT_DISABLE;
    }
    else
    {
        return CLI_EXE_PARA_INVALID;
    }
    port_autoneg.port = cli_str_convert_uint(vline->index[3], &ret);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the port para is invalid. \n");
        return CLI_EXE_FAIL;
    }
    if (!CLI_ABSTR_OPS->cli_port_set_port_auto_neg_state)
    {
       cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_port_set_port_auto_neg_state(unitId, port_autoneg);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_port_set_port_auto_neg_state", ret);
        return ret;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_set_port_fc_state(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_set_fc_state_params_t fc_state_params;

    memset_s (&fc_state_params, sizeof(cli_set_fc_state_params_t), 0, sizeof(cli_set_fc_state_params_t));
    if (!osal_strcmp(vline->index[5], "enable"))
    {
       fc_state_params.port_ability.asyFC_en = TRUE;
       fc_state_params.port_ability.fc_en = TRUE;
    }
    else if (!osal_strcmp(vline->index[5], "disable"))
    {
       fc_state_params.port_ability.asyFC_en = FALSE;
       fc_state_params.port_ability.fc_en = FALSE;
    }
    else
    {
        return CLI_EXE_PARA_INVALID;
    }

    if (!CLI_ABSTR_OPS->cli_port_set_port_fc_state)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_port_set_port_fc_state(unitId, fc_state_params);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_port_set_fc_state", ret);
        return ret;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_set_port_duplex_and_speed(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_set_port_speed_duplex_t port_force_ctrl;

    memset_s (&port_force_ctrl, sizeof(cli_set_port_speed_duplex_t), 0, sizeof(cli_set_port_speed_duplex_t));

    port_force_ctrl.port  = cli_str_convert_uint(vline->index[3], &ret);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the port para is invalid. \n");
        return CLI_EXE_FAIL;
    }
    if (!osal_strcmp(vline->index[5], "10half"))
    {
        port_force_ctrl.speed_duplex = PORT_SPEED_DUP_10HALF;
    }
    else if (!osal_strcmp(vline->index[5], "10full"))
    {
        port_force_ctrl.speed_duplex = PORT_SPEED_DUP_10FULL;
    }
    else if (!osal_strcmp(vline->index[5], "100half"))
    {
        port_force_ctrl.speed_duplex = PORT_SPEED_DUP_100HALF;
    }
    else if (!osal_strcmp(vline->index[5], "100full"))
    {
        port_force_ctrl.speed_duplex= PORT_SPEED_DUP_100FULL;
    }
    else if (!osal_strcmp(vline->index[5], "1000full"))
    {
        port_force_ctrl.speed_duplex = PORT_SPEED_DUP_1000FULL;
    }
    else
    {
        cli_print_out("not support speed duplex \n");
        return CLI_EXE_PARA_INVALID;
    }

    if (!CLI_ABSTR_OPS->cli_port_set_port_duplex_speed)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_port_set_port_duplex_speed(unitId, port_force_ctrl);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("port_set_port_duplex_speed", ret);
        return ret;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_port_execute(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    if (vline == NULL)
    {
        return CLI_EXE_FAIL;
    }
    if (vline->max < 5 ||vline->max > 8)
    {
        return CLI_EXE_UNKNOW_COMMAND;
    }
    if (vline->max == 5 && !osal_strcmp(vline->index[0], "port")
        && !osal_strcmp(vline->index[1], "get")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "extifmode"))
    {
        /*port get port [PORT] extifmode */
        ret = cli_get_port_extifmode(vline);
    }
    else if (vline->max == 5 && !osal_strcmp(vline->index[0], "port")
        && !osal_strcmp(vline->index[1], "get")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "linkstatus"))
    {
        /* port get port [PORT] linkstatus */
        ret = cli_get_port_linkstatus(vline);
    }
    else if (vline->max == 5 && !osal_strcmp(vline->index[0], "port")
        && !osal_strcmp(vline->index[1], "get")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "state"))
    {
        /* port get port [PORT] state  */
        ret = cli_get_port_state(vline);
    }
    else if (vline->max == 6 && !osal_strcmp(vline->index[0], "port")
        && !osal_strcmp(vline->index[1], "set")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "extifmode"))
    {
        /* port set port [PORT] extifmode [MODE] */
        ret = cli_set_port_extifmode(vline);
    }
    else if (vline->max == 6 && !osal_strcmp(vline->index[0], "port")
        && !osal_strcmp(vline->index[1], "get")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "auto_neg")
        && !osal_strcmp(vline->index[5], "state"))
    {
        /*port get port [PORT] auto_neg state*/
        ret = cli_get_port_autoneg(vline);
    }
    else if (vline->max == 6 && !osal_strcmp(vline->index[0], "port")
        && !osal_strcmp(vline->index[1], "set")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "state"))
    {
        /* port set port [PORT] state [STATE] */
        ret = cli_set_port_state(vline);
    }
    else if (vline->max == 6 && !osal_strcmp(vline->index[0], "port")
        && !osal_strcmp(vline->index[1], "set")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "fc_en"))
    {
        /* port set port [PORT] fc_en [FC_EN] */
        ret = cli_set_port_fc_state(vline);
    }
    else if (vline->max == 6 && !osal_strcmp(vline->index[0], "port")
    && !osal_strcmp(vline->index[1], "set")
    && !osal_strcmp(vline->index[2], "port")
    && !osal_strcmp(vline->index[4], "speed_duplex"))
    {
        /* port set port [PORT] speed [SPEED] */
        ret = cli_set_port_duplex_and_speed(vline);
    }
    else if (vline->max == 7 && !osal_strcmp(vline->index[0], "port")
        && !osal_strcmp(vline->index[1], "set")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "auto_neg")
        && !osal_strcmp(vline->index[5], "state"))
    {
        /* port set port [PORT] auto_neg state [STATE] */
        ret = cli_set_port_autoneg(vline);
    }
    else
    {
        return CLI_EXE_UNKNOW_COMMAND;
    }
    return ret;
}

cli_exe_result_t
cli_vlan_set_port_pvid(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_vlan_set_pvid_params_t pvid_params;

    memset_s (&pvid_params, sizeof(cli_vlan_set_pvid_params_t), 0, sizeof(cli_vlan_set_pvid_params_t));
    pvid_params.port = cli_str_convert_uint(vline->index[3], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the portID is invalid. \n");
        return ret;
    }
    pvid_params.vid = cli_str_convert_uint(vline->index[5], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the pvid is invalid. \n");
        return ret;
    }
    if (!CLI_ABSTR_OPS->cli_vlan_set_port_pvid)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_vlan_set_port_pvid(unitId, pvid_params);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("yt_vlan_port_igrPvid_set", ret);
        return ret;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_vlan_set_port_memberport_untag(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_vlan_set_vid_params_t vlan_vid_params;

    memset_s (&vlan_vid_params, sizeof(cli_vlan_set_vid_params_t), 0, sizeof(cli_vlan_set_vid_params_t));
    vlan_vid_params.pvid = cli_str_convert_uint(vline->index[3], &ret);
    if (ret != CLI_EXE_SUCCESS)
    {
        cli_print_out("the pvid is invalid. \n");
        return ret;
    }
   cli_str_convert_portmask(vline->index[5], &ret, &vlan_vid_params.memberports);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the member_ports is invalid. \n");
        return ret;
    }
    cli_str_convert_portmask(vline->index[7], &ret, &vlan_vid_params.untagports);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the untag_ports is invalid. \n");
        return ret;
    }
    if (!CLI_ABSTR_OPS->cli_vlan_set_vid_memberports_untagports)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }
    ret = CLI_ABSTR_OPS->cli_vlan_set_vid_memberports_untagports(unitId, vlan_vid_params);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("vlan_set_vid_memberports_untagports", ret);
        return ret;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_switch_init(void)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;

    if (!CLI_ABSTR_OPS->cli_yt_switch_init)
    {
       cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_yt_switch_init();
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("yt switch init failed.", ret);
        return ret;
    }
    cli_print_out("%s success !\n", __func__);
    return ret;
}

cli_exe_result_t
cli_switch_init_execute(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;

    if (NULL == vline)
    {
        return CLI_EXE_FAIL;
    }
    if (vline->max == 2 && !osal_strcmp(vline->index[0], "switch")
        &&!osal_strcmp(vline->index[1], "init"))
    {
        ret = cli_switch_init();
    }
    else if (vline->max ==3 && !osal_strcmp(vline->index[0], "switch")
        &&!osal_strcmp(vline->index[1], "unit"))
    {
        unitId = cli_str_convert_uint(vline->index[2], &ret);
        if (CLI_EXE_SUCCESS != ret)
        {
            cli_print_out("the port para is invalid. \n");
            return CLI_EXE_FAIL;
        }
    }
    else
    {
        return CLI_EXE_UNKNOW_COMMAND;
    }
    return ret;
}

cli_exe_result_t
cli_vlan_execute(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    if (NULL == vline)
    {
        return CLI_EXE_FAIL;
    }

    if (vline->max == 6 && !osal_strcmp(vline->index[0], "vlan")
        && !osal_strcmp(vline->index[1], "set")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "pvid"))
    {
        /*cli_print_out("vlan set port [PORT] pvid [PVID] */
        ret = cli_vlan_set_port_pvid(vline);
    }
    else if (vline->max == 8 && !osal_strcmp(vline->index[0], "vlan")
        && !osal_strcmp(vline->index[1], "set")
        && !osal_strcmp(vline->index[2], "vid")
        && !osal_strcmp(vline->index[4], "memberports")
        && !osal_strcmp(vline->index[6], "untagports"))
    {
        /*vlan set vid [VID] memberports [MASK] untagports [MASK]*/
        ret = cli_vlan_set_port_memberport_untag(vline);
    }
    else
    {
        return CLI_EXE_UNKNOW_COMMAND;
    }
    return ret;
}

cli_exe_result_t
cli_mib_get(void)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    yt_enable_t action = YT_DISABLE;

    if (!CLI_ABSTR_OPS->cli_mib_get_state)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }
    ret = CLI_ABSTR_OPS->cli_mib_get_state(unitId, &action);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("cli_mib_set_state Fail, ret is %d\n", ret);
        return ret;
    }
    
    cli_print_out("mib state:  %8d.\n", action);
    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_mib_set(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    yt_enable_t action = YT_DISABLE;

    if (!osal_strcmp(vline->index[3], "enable"))
    {
        action = YT_ENABLE;
    }
    else if (!osal_strcmp(vline->index[3], "disable"))
    {
         action = YT_DISABLE;
    }
    else
    {
        return CLI_EXE_PARA_INVALID;
    }

    if (!CLI_ABSTR_OPS->cli_mib_set_state)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_mib_set_state(unitId, action);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_mib_set_state", ret);
        return ret;
    }
    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_mib_get_count_by_port(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_mib_get_params_t mib_get_params;

    memset_s(&mib_get_params, sizeof(cli_mib_get_params_t), 0, sizeof(cli_mib_get_params_t));

    mib_get_params.port = cli_str_convert_uint(vline->index[3], &ret);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the port para is invalid. \n");
        return ret;
    }
    if (!CLI_ABSTR_OPS->cli_mib_get_port)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_mib_get_port(unitId, &mib_get_params);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_mib_get_port", ret);
        return ret;
    }
    cli_print_out("RX_BROADCAST :      %8u\n", mib_get_params.mib_port_cnt.RX_BROADCAST);
    cli_print_out("RX_PAUSE :          %8u\n", mib_get_params.mib_port_cnt.RX_PAUSE);
    cli_print_out("RX_MULTICAST :      %8u\n", mib_get_params.mib_port_cnt.RX_MULTICAST);
    cli_print_out("RX_FCS_ERR :        %8u\n", mib_get_params.mib_port_cnt.RX_FCS_ERR);
    cli_print_out("RX_ALIGNMENT_ERR :  %8u\n", mib_get_params.mib_port_cnt.RX_ALIGNMENT_ERR);
    cli_print_out("RX_UNDERSIZE :      %8u\n", mib_get_params.mib_port_cnt.RX_UNDERSIZE);
    cli_print_out("RX_FRAGMENT :       %8u\n", mib_get_params.mib_port_cnt.RX_FRAGMENT);
    cli_print_out("RX_64B :            %8u\n", mib_get_params.mib_port_cnt.RX_64B);
    cli_print_out("RX_65_127B :        %8u\n", mib_get_params.mib_port_cnt.RX_65_127B);
    cli_print_out("RX_128_255B :       %8u\n", mib_get_params.mib_port_cnt.RX_128_255B);
    cli_print_out("RX_256_511B :       %8u\n", mib_get_params.mib_port_cnt.RX_256_511B);
    cli_print_out("RX_512_1023B :      %8u\n", mib_get_params.mib_port_cnt.RX_512_1023B);
    cli_print_out("RX_1024_1518B :     %8u\n", mib_get_params.mib_port_cnt.RX_1024_1518B);
    cli_print_out("RX_JUMBO :          %8u\n", mib_get_params.mib_port_cnt.RX_JUMBO);
    cli_print_out("RX_OKBYTE :         %8llu\n", mib_get_params.mib_port_cnt.RX_OKBYTE);
    cli_print_out("RX_NOT_OKBYTE :     %8llu\n", mib_get_params.mib_port_cnt.RX_NOT_OKBYTE);
    cli_print_out("RX_OVERSIZE :       %8u\n", mib_get_params.mib_port_cnt.RX_OVERSIZE);
    cli_print_out("RX_DISCARD :        %8u\n", mib_get_params.mib_port_cnt.RX_DISCARD);
    cli_print_out("TX_BROADCAST :      %8u\n", mib_get_params.mib_port_cnt.TX_BROADCAST);
    cli_print_out("TX_PAUSE :          %8u\n", mib_get_params.mib_port_cnt.TX_PAUSE);
    cli_print_out("TX_MULTICAST :      %8u\n", mib_get_params.mib_port_cnt.TX_MULTICAST);
    cli_print_out("TX_UNDERSIZE :      %8u\n", mib_get_params.mib_port_cnt.TX_UNDERSIZE);
    cli_print_out("TX_64B :            %8u\n", mib_get_params.mib_port_cnt.TX_64B);
    cli_print_out("TX_65_127B :        %8u\n", mib_get_params.mib_port_cnt.TX_65_127B);
    cli_print_out("TX_128_255B :       %8u\n", mib_get_params.mib_port_cnt.TX_128_255B);
    cli_print_out("TX_256_511B :       %8u\n", mib_get_params.mib_port_cnt.TX_256_511B);
    cli_print_out("TX_512_1023B :      %8u\n", mib_get_params.mib_port_cnt.TX_512_1023B);
    cli_print_out("TX_1024_1518B :     %8u\n", mib_get_params.mib_port_cnt.TX_1024_1518B);
    cli_print_out("TX_JUMBO :          %8u\n", mib_get_params.mib_port_cnt.TX_JUMBO);
    cli_print_out("TX_OKBYTE :         %8llu\n", mib_get_params.mib_port_cnt.TX_OKBYTE);
    cli_print_out("TX_COLLISION :      %8u\n", mib_get_params.mib_port_cnt.TX_COLLISION);
    cli_print_out("TX_EXCESSIVE_COLLISION :   %u\n", mib_get_params.mib_port_cnt.TX_EXCESSIVE_COLLISION);
    cli_print_out("TX_MULTI_COLLISION :%8u\n", mib_get_params.mib_port_cnt.TX_MULTI_COLLISION);
    cli_print_out("TX_SINGLE_COLLISION:%8u\n", mib_get_params.mib_port_cnt.TX_SINGLE_COLLISION);
    cli_print_out("TX_OK_PKT :         %8llu\n", mib_get_params.mib_port_cnt.TX_OKBYTE);
    cli_print_out("TX_DEFER :          %8u\n", mib_get_params.mib_port_cnt.TX_DEFER);
    cli_print_out("TX_LATE_COLLISION : %8u\n", mib_get_params.mib_port_cnt.TX_LATE_COLLISION);
    cli_print_out("RX_OAM_COUNTER :    %8u\n", mib_get_params.mib_port_cnt.RX_OAM_COUNTER);
    cli_print_out("TX_OAM_COUNTER :    %8u\n", mib_get_params.mib_port_cnt.TX_OAM_COUNTER);
    cli_print_out("RX_UNICAST :        %8u\n", mib_get_params.mib_port_cnt.RX_UNICAST);
    cli_print_out("TX_UNICAST :        %8u\n", mib_get_params.mib_port_cnt.TX_OAM_COUNTER);

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_mib_clear_all(void)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_mib_clear_params_t mib_clear_params;
    
    memset_s(&mib_clear_params, sizeof(cli_mib_clear_params_t), 0, sizeof(cli_mib_clear_params_t));
    mib_clear_params.mib_clear_mode = ALL;

    if (!CLI_ABSTR_OPS->cli_mib_get_clear)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_mib_get_clear(unitId, mib_clear_params);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("yt_stat_mib_port_clear", ret);
        return ret;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_mib_clear_port(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_mib_clear_params_t mib_clear_params;
    
    memset_s(&mib_clear_params, sizeof(cli_mib_clear_params_t), 0, sizeof(cli_mib_clear_params_t));
    mib_clear_params.mib_clear_mode = PORT;
    mib_clear_params.port = cli_str_convert_uint(vline->index[3], &ret);
    if (!CLI_ABSTR_OPS->cli_mib_get_clear)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_mib_get_clear(unitId, mib_clear_params);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("yt_stat_mib_port_clear", ret);
        return ret;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_mib_execute(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    if (NULL == vline)
    {
        return CLI_EXE_FAIL;
    }

    if (vline->max < 3 ||vline->max > 4)
    {
        return CLI_EXE_UNKNOW_COMMAND;
    }
    if (vline->max == 3 && !osal_strcmp(vline->index[0], "mib")
        && !osal_strcmp(vline->index[1], "get")
        && !osal_strcmp(vline->index[2], "state"))
    {
        /*mib get state*/
        ret = cli_mib_get();
    }
    else if (vline->max == 4 && !osal_strcmp(vline->index[0], "mib")
        && !osal_strcmp(vline->index[1], "set")
        && !osal_strcmp(vline->index[2], "state"))
    {
        /* mib set state [STATE] */
        ret = cli_mib_set(vline);
    }
    else if (vline->max == 4 && !osal_strcmp(vline->index[0], "mib")
        && !osal_strcmp(vline->index[1], "get")
        && !osal_strcmp(vline->index[2], "port"))
    {
        /* mib get port [PORT] */
        ret = cli_mib_get_count_by_port(vline);
    }
    else if (vline->max == 3 && !osal_strcmp(vline->index[0], "mib")
        && !osal_strcmp(vline->index[1], "clear")
        && !osal_strcmp(vline->index[2], "all"))
    {
        /* mib clear all */
        ret = cli_mib_clear_all();
    }
    else if (vline->max == 4 && !osal_strcmp(vline->index[0], "mib")
        && !osal_strcmp(vline->index[1], "clear")
        && !osal_strcmp(vline->index[2], "port"))
    {
        /* mib clear port [PORT] */
        ret = cli_mib_clear_port(vline);
    }
    else
    {
        return CLI_EXE_UNKNOW_COMMAND;
    }
    return ret;
}

cli_exe_result_t
cli_fdb_learn_state(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    cli_fdb_learn_params_t fdb_learn;

    memset_s (&fdb_learn, sizeof(cli_fdb_learn_params_t), 0, sizeof(cli_fdb_learn_params_t));
    if (!osal_strcmp(vline->index[5], "enable"))
    {
        fdb_learn.state = YT_ENABLE;
    }
    else if (!osal_strcmp(vline->index[5], "disable"))
    {
        fdb_learn.state = YT_DISABLE;
    }
    else
    {
        return CLI_EXE_PARA_INVALID;
    }
    fdb_learn.port  = cli_str_convert_uint(vline->index[3], &ret);
    if (CLI_EXE_SUCCESS != ret)
    {
        cli_print_out("the port para is invalid. \n");
        return CLI_EXE_FAIL;
    }
    if (!CLI_ABSTR_OPS->cli_fdb_set_learn_state)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }

    ret = CLI_ABSTR_OPS->cli_fdb_set_learn_state(unitId, fdb_learn);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_fdb_set_learn_state", ret);
        return ret;
    }

    return CLI_EXE_SUCCESS;
}

cli_exe_result_t
cli_fdb_execute(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    if (NULL == vline)
    {
        return CLI_EXE_FAIL;
    }
    if (vline->max == 6 && !osal_strcmp(vline->index[0], "fdb")
        && !osal_strcmp(vline->index[1], "learn")
        && !osal_strcmp(vline->index[2], "port")
        && !osal_strcmp(vline->index[4], "state"))
    {
        /* fdb learn port [PORT] state [STATE] */
        ret = cli_fdb_learn_state(vline);
    }
    else
    {
        return CLI_EXE_UNKNOW_COMMAND;
    }
    return ret;
}

cli_exe_result_t
cli_quit_execute(void)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;

    if (!CLI_ABSTR_OPS->cli_exit)
    {
        cli_print_out("No callback function\n");
        return CLI_EXE_FAIL;
    }
    ret = CLI_ABSTR_OPS->cli_exit(unitId);
    if (CLI_EXE_SUCCESS != ret)
    {
        CLI_RET_FAILED("cli_exit", ret);
        return ret;
    }
    cli_abstr_exit();
    cli_print_out("%s success !\n", __func__);
    return ret;
}

cli_exe_result_t
cli_execute_commnad(cli_items_p vline)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    char *command;
    if (vline == NULL)
    {
        return CLI_EXE_FAIL;
    }
    if (vline->max == 1 && osal_strncmp(vline->index[0], "quit", 4) && osal_strncmp(vline->index[0], "help", 4))
    {
        ret = CLI_EXE_UNKNOW_COMMAND;
        return ret;
    }
    else if (vline->max == 1 && !osal_strncmp(vline->index[0], "quit", 4))
    {
        if (!cli_quit_execute())
        {
            terminal_status = 0;
            ret = CLI_EXE_QUIT;
        }
        else
        {
            ret = CLI_EXE_FAIL;
        }
        return ret;
    }
    else if (vline->max == 1 && !osal_strncmp(vline->index[0], "help", 4))
    {
        cli_usage();
        return ret;
    }
    else
    {
        command = CLI_ITEMS_SLOT(vline, 0);
        if (command[0] == 's')
        {
            ret = cli_switch_init_execute(vline);
        }
#ifdef SYS_INCLUDED
        else if (command[0] == 'r')
        {
            ret = cli_reg_execute(vline);
        }
#endif
#ifdef PORT_INCLUDED
        else if (!osal_strcmp(vline->index[0], "port"))
        {
            ret = cli_port_execute(vline);
        }
        else if (!osal_strcmp(vline->index[0], "phy"))
        {
            ret = cli_phy_reg_execute(vline);
        }
#endif
#ifdef STAT_INCLUDED
        else if (command[0] == 'm')
        {
            ret = cli_mib_execute(vline);
        }
#endif
#ifdef L2_INCLUDED
        else if (command[0] == 'f')
        {
            ret = cli_fdb_execute(vline);
        }
#endif
#ifdef VLAN_INCLUDED
        else if (command[0] == 'v')
        {
            ret = cli_vlan_execute(vline);
        }
#endif
        else
        {
            ret = CLI_EXE_UNKNOW_COMMAND;
        }
    }
    return ret;
}

void
cli_free_items (cli_items_p items)
{
    uint32_t i;
    char *cp;

    if (!items)
    {
        return;
    }
    for (i = 0; i < CLI_ITEMS_MAX (items); i++)
    {
        if ((cp = CLI_ITEMS_SLOT (items, i)) != NULL)
        {
            osal_free (cp);
            cp = NULL;
        }
    }
    cli_items_free (items);
}


void
cli_items_free (cli_items_p items)
{
    if (NULL != items->index)
    {
        osal_free (items->index);
        items->index = NULL;
    }

    if (NULL != items)
    {
        osal_free (items);
        items = NULL;
    }
}

uint32_t
cli_items_set (cli_items_p items, void *val)
{
    uint32_t i;

    i = cli_items_slot (items);
    cli_items_allocated_ensure (items, i);

    items->index[i] = val;

    if (items->max <= i)
    {
        items->max = i + 1;
    }
    return i;
}

uint32_t
cli_items_slot (cli_items_p items)
{
    uint32_t i;

    if (items->max == 0)
    {
        return 0;
    }

    for (i = 0; i < items->max; i++)
    {
        if (items->index[i] == 0)
        {
            return i;
        }
    }
    return i;
}

void
cli_items_allocated_ensure (cli_items_p items, uint32_t num)
{
    if (items->allocated > num)
    {
        return;
    }

    items->index = osal_realloc (items->index, sizeof (void *) * (items->allocated * 2));
    if (!items->index)
    {
        cli_print_out("malloc err!\n");
        return;
    }

    memset_s(&items->index[items->allocated], sizeof (void *) * items->allocated, 0, sizeof (void *) * items->allocated);
    items->allocated *= 2;

    if (items->allocated <= num)
    {
        cli_items_allocated_ensure (items, num);
    }
}
