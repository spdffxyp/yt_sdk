/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************/

#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <libgen.h>
#include <stdlib.h>
#include "yt_types.h"
#include "cli_parser.h"
#include "cli_types.h"
#include "cli_abstr.h"
#include "osal_mem.h"
#include "securec.h"
#include "yt_init.h"

uint32_t terminal_status;
yt_unit_t unitId = 0;

/* cmd engine shell main routine. */
int32_t
main (void)
{
    cli_exe_result_t ret = CLI_EXE_SUCCESS;
    terminal_status = 1;
    char* pread_buf = NULL;
    ret = cli_abstr_init();
    if (CLI_EXE_SUCCESS != ret)
    {
        return 0;
    }

    pread_buf = osal_malloc(CLI_READ_MAX_SIZE);
    memset_s(pread_buf, CLI_READ_MAX_SIZE, 0, CLI_READ_MAX_SIZE);
    cli_prompt();
    while (terminal_status == 1)
    {
        cli_items_p vline;
        cli_input_commnad_read(pread_buf, CLI_READ_MAX_SIZE);
        vline = cli_split_input_buf(pread_buf);
        ret = cli_execute_commnad(vline);
        switch(ret)
        {
            case CLI_EXE_SUCCESS:
                cli_prompt();
                break;
            case CLI_EXE_FAIL:
                cli_prompt();
                break;
            case CLI_EXE_PARA_INVALID:
                cli_print_out("command params invalid.\n");
                cli_prompt();
                break;
            case CLI_EXE_UNKNOW_COMMAND:
                cli_print_out("unsupport command\n");
                cli_prompt();
            default:
                break;
        }
        memset_s(pread_buf, CLI_READ_MAX_SIZE, 0, CLI_READ_MAX_SIZE);
        cli_free_items(vline);
    }

    if (NULL != pread_buf)
    {
        osal_free(pread_buf);
        pread_buf = NULL;
    }
    return 0;
}
