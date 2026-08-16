/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************/

#ifndef _CLI_PARSER_H_
#define _CLI_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "yt_types.h"
#include "cli_types.h"

#define CLI_ITEMS_MAX(ITEMS) ((ITEMS)->max)
#define CLI_ITEMS_SLOT(ITEMS,I)  ((ITEMS)->index[(I)])
#define CLI_READ_MAX_SIZE    100
#define CLI_ALLOCATED_MIN_SIZE 1
#define CLI_PROMPT    "YT.0>"

#define CLI_RET_FAILED(STR1, RET) \
do { \
    cli_print_out("??? Failed: ret (%u)  in function (%s).\n", (RET), (STR1));\
    return CLI_EXE_FAIL;\
} while (0)

/* extern */
extern uint32_t terminal_status;
extern yt_unit_t unitId;

/* struct */
struct cli_items_s 
{
    uint32_t allocated;		/* number of allocated slot */
    uint32_t max;		        /* max number of used slot */
    void **index;			/* index to data */
};
typedef struct cli_items_s *cli_items_p;

/* function */
uint32_t cli_input_commnad_read(char *buf, uint32_t buf_size);
void cli_prompt(void);
uint32_t cli_print_out (const char *format, ...);
void cli_str_convert_portmask(char* str, cli_exe_result_t* ret, yt_port_mask_t* portMask);
cli_items_p cli_split_input_buf(char *string);
cli_items_p cli_items_attr_init (uint32_t size);
void cli_free_items (cli_items_p items);
void cli_items_free (cli_items_p items);
uint32_t cli_items_set (cli_items_p items, void *val);
uint32_t cli_items_slot (cli_items_p items);
void cli_items_allocated_ensure (cli_items_p items, uint32_t num);
cli_exe_result_t cli_execute_commnad(cli_items_p vline);
cli_exe_result_t cli_switch_init(void);
cli_exe_result_t cli_switch_init_execute(cli_items_p vline);
void cli_usage();
cli_exe_result_t cli_phy_reg_read(cli_items_p vline);
cli_exe_result_t cli_phy_reg_write(cli_items_p vline);
cli_exe_result_t cli_reg_execute(cli_items_p vline);
cli_exe_result_t cli_port_execute(cli_items_p vline);
cli_exe_result_t cli_vlan_execute(cli_items_p vline);
cli_exe_result_t cli_mib_execute(cli_items_p vline);
cli_exe_result_t cli_fdb_execute(cli_items_p vline);
cli_exe_result_t cli_reg_read(cli_items_p vline);
cli_exe_result_t cli_reg_write(cli_items_p vline);
cli_exe_result_t cli_phy_reg_execute(cli_items_p vline);
cli_exe_result_t cli_get_port_extifmode(cli_items_p vline);
cli_exe_result_t cli_get_port_linkstatus(cli_items_p vline);
cli_exe_result_t cli_get_port_state(cli_items_p vline);
cli_exe_result_t cli_set_port_extifmode(cli_items_p vline);
cli_exe_result_t cli_get_port_autoneg(cli_items_p vline);
cli_exe_result_t cli_set_port_state(cli_items_p vline);
cli_exe_result_t cli_set_port_autoneg(cli_items_p vline);
cli_exe_result_t cli_set_port_fc_state(cli_items_p vline);
cli_exe_result_t cli_set_port_duplex_and_speed(cli_items_p vline);
cli_exe_result_t cli_mib_get(void);
cli_exe_result_t cli_mib_set(cli_items_p vline);
cli_exe_result_t cli_mib_get_count_by_port(cli_items_p vline);
cli_exe_result_t cli_mib_clear_all(void);
cli_exe_result_t cli_mib_clear_port(cli_items_p vline);
cli_exe_result_t cli_fdb_learn_state(cli_items_p vline);
cli_exe_result_t cli_vlan_set_port_pvid(cli_items_p vline);
cli_exe_result_t cli_vlan_set_port_memberport_untag(cli_items_p vline);
cli_exe_result_t cli_quit_execute(void);
char *extifmode_enum_type_to_string(yt_extif_mode_t type);
char *speed_status_enum_type_to_string(yt_port_link_status_t speed_status);
char *speed_enum_type_to_string(yt_port_speed_t speed);
char *duplex_enum_type_to_string(yt_port_duplex_t duplex);
void cli_usage(void);
uint32_t cli_str_convert_uint(char* str, cli_exe_result_t *ret);

#ifdef __cplusplus
}
#endif

#endif // _CLI_PARSER_H_
