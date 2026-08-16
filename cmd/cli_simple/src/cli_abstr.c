/*
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "cli_abstr.h"
#include "osal_mem.h"
#if defined(CMD_SMPL_ACCESS_SDK_DIRECT)
#include "cli_smpl.h"
#endif
#if defined(CMD_SMPL_ACCESS_SDK_IOCTL)
#include "cli_ioctl.h"
#endif
#include "securec.h"

cli_abstr_info_t gAbstrInfo;

static const cli_abstr_t abstrArray[] = 
{
#if defined(CMD_SMPL_ACCESS_SDK_DIRECT)
    {
        .id = CLI_SMPL,
        .ops = &smplOps,
    },
#endif

#if defined(CMD_SMPL_ACCESS_SDK_IOCTL)
    {
        .id = CLI_IOCTL,
        .ops = &ioctlOps,
    },
#endif
};

static const cli_abstr_t *
cli_find_abstr_info_by_id(cli_abstr_id_t id)
{
    uint32_t i;
    const cli_abstr_t *tmp = NULL;

    for (i = 0; i < ARRAY_SIZE(abstrArray); i++)
    {
        tmp = &abstrArray[i];
        if (tmp->id == id)
        {
            return tmp;
        }
    }

    return NULL;
}

int32_t
cli_abstr_init(void)
{
    memset_s(&gAbstrInfo, sizeof(gAbstrInfo), 0, sizeof(gAbstrInfo));
#if defined(CMD_SMPL_ACCESS_SDK_DIRECT)
    gAbstrInfo.abstr = cli_find_abstr_info_by_id(CLI_SMPL);
    if (NULL == gAbstrInfo.abstr)
    {
        return CLI_ABSTR_INIT_FAIL;
    }
    return CLI_EXE_SUCCESS;
#elif defined(CMD_SMPL_ACCESS_SDK_IOCTL)
    int32_t ret = CLI_EXE_SUCCESS;
    int32_t fd;
    gAbstrInfo.abstr = cli_find_abstr_info_by_id(CLI_IOCTL);
    if (!gAbstrInfo.abstr)
    {
        return CLI_ABSTR_INIT_FAIL;
    }
    gAbstrInfo.info = &ioctlComm;
    if (!gAbstrInfo.info || !gAbstrInfo.info->create)
    {
        return CLI_ABSTR_INIT_FAIL;
    }
    ret = gAbstrInfo.info->create(gAbstrInfo.info);
    return ret;
#else
    return CLI_ABSTR_INIT_FAIL;
#endif
}

void
cli_abstr_exit(void)
{
#if defined(CMD_SMPL_ACCESS_SDK_IOCTL)
    if (gAbstrInfo.info && gAbstrInfo.info->destroy)
    {
        gAbstrInfo.info->destroy(gAbstrInfo.info);
    }
#endif
    return;
}