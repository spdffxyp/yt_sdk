/*******************************************************************************
*                                                                              *
*  Copyright (c), 2023, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "ioctl_share.h"
#include "module_api.h"
#include "cmd_smpl_ioctl.h"
#include <linux/device.h>

static cmd_dev_t gCmdDev;

static const intercomm_cmd_ops_t cmdOps[] = {
#ifdef SYS_INCLUDED
    {
        .cmd = REG_READ_REGADDR_SET,
        .ops = &reg_read_regaddr,
    },
    {
        .cmd = REG_WRITE_REGADDR_VALUE_SET,
        .ops = &reg_write_regaddr_value,
    },
#endif
#ifdef PORT_INCLUDED
    {
        .cmd = PHY_REG_REGADDR_READ,
        .ops = &phy_reg_read_regaddr,
    },
    {
        .cmd = PHY_REG_REGADDR_WRITE,
        .ops = &phy_reg_write_regaddr,
    },
    {
        .cmd = PORT_SET_PORT_EXTIFMODE,
        .ops = &port_set_port_extifmode,
    },
    {
        .cmd = PORT_GET_PORT_EXTIFMODE,
        .ops = &port_get_port_extifmode,
    },
    {
        .cmd = PORT_GET_PORT_LINKSTATUS,
        .ops = &port_get_port_linkstatus,
    },
    {
        .cmd = PORT_SET_PORT_AUTO_NEG_STATE,
        .ops = &port_set_port_auto_neg_state,
    },
    {
        .cmd = PORT_GET_PORT_AUTO_NEG_STATE,
        .ops = &port_get_port_auto_neg_state,
    },
    {
        .cmd = PORT_SET_PORT_STATE,
        .ops = &port_set_port_state,
    },
    {
        .cmd = PORT_GET_PORT_STATE,
        .ops = &port_get_port_state,
    },
    {
        .cmd = PORT_SET_PORT_DUPLEX_SPEED,
        .ops = &port_set_port_duplex_speed,
    },
    {
        .cmd = PORT_SET_PORT_FC_STATE,
        .ops = &port_set_port_fc_state,
    },
#endif
#ifdef VLAN_INCLUDED
    {
        .cmd = VLAN_SET_PORT_PVID,
        .ops = &vlan_set_port_pvid,
    },
    {
        .cmd = VLAN_SET_VID_MEMBERPORTS_UNTAGPORTS,
        .ops = &vlan_set_vid_memberports_untagports,
    },
#endif
#ifdef STAT_INCLUDED
    {
        .cmd = MIB_SET_STATE,
        .ops = &mib_set_state,
    },
    {
        .cmd = MIB_GET_STATE,
        .ops = &mib_get_state,
    },
    {
        .cmd = MIB_GET_PORT,
        .ops = &mib_get_port,
    },
    {
        .cmd = MIB_GET_CLEAR,
        .ops = &mib_get_clear,
    },
#endif
#ifdef L2_INCLUDED
    {
        .cmd = FDB_LEARN_STATE,
        .ops = &fdb_set_learn_state,
    },
#endif
    {
        .cmd = SWITCH_INIT,
        .ops = &intercomm_switch_init,
    },
    {
        .cmd = SWITCH_EXIT,
        .ops = &intercomm_switch_exit,
    },
};
const intercomm_cmd_ops_t *intercomm_find_ops(uint16_t cmd)
{
    uint32_t size = ARRAY_SIZE(cmdOps);
    uint32_t i;
    const intercomm_cmd_ops_t *tmp;
    for (i = 0; i < size; i++)
    {
        tmp = &cmdOps[i];
        if (tmp && tmp->cmd == cmd)
        {
            return tmp;
        }
    }

    return NULL;
}

static int32_t cmd_process(cli_ioctl_data_t *data)
{
    const intercomm_cmd_ops_t *intercommOps;

    intercommOps = intercomm_find_ops(data->hdr.cmd);
    if (!intercommOps)
    {
        pr_err("%s %d find subcmd ops fail cmd %u\n",
                __func__, __LINE__, data->hdr.cmd);
        return -EINVAL;
    }
    return intercommOps->ops(data);
}

void cmd_free_data(cli_ioctl_data_t *data)
{
    cli_ioctl_tlv_t *tlv;
    int32_t i;

    for (i = 0; i < CLI_MAX_TLV; i++)
    {
        tlv = &data->tlv[i];
        if (tlv->val)
        {
            kfree(tlv->val);
            tlv->val = NULL;
        }
    }
}

int32_t cmd_copy_data_from_user(cli_ioctl_data_t *data, cli_ioctl_data_t __user *dataUser)
{
    cli_ioctl_tlv_t *tlv;
    cli_ioctl_tlv_t __user *tlvUser;
    int32_t i;

    if (!dataUser)
    {
        pr_err("%s %d parameter error\n", __func__, __LINE__);
        return -EINVAL;
    }

    if (copy_from_user((void *)data, (void *)dataUser, sizeof(cli_ioctl_hdr_t)))
    {
        pr_err("%s %d copy data hdr from user error\n", __func__, __LINE__);
		return -EFAULT;
    }

    for (i = 0; i < CLI_MAX_TLV; i++)
    {
        tlv = &data->tlv[i];
        tlvUser = &dataUser->tlv[i];
        if (copy_from_user((void *)tlv, (void *)tlvUser, sizeof(cli_ioctl_tlv_hdr_t)))
        {
            pr_err("%s %d copy tlv hdr from user error\n", __func__, __LINE__);
            return -EFAULT;
        }
        if (tlv->tlvhdr.len)
        {
            tlv->val = kzalloc(tlv->tlvhdr.len, GFP_KERNEL);
            if (!tlv->val)
            {
                pr_err("%s %d kzalloc error\n", __func__, __LINE__);
                goto free;
            }
            if (copy_from_user((void *)tlv->val, (void *)tlvUser->val, tlv->tlvhdr.len))
            {
                pr_err("%s %d copy tlv value from user error\n", __func__, __LINE__);
                goto free;
            }
        }
    }

    return 0;

free:
    cmd_free_data(data);

    return -EFAULT;
}

int32_t cmd_copy_data_to_user(cli_ioctl_data_t *data, cli_ioctl_data_t __user *dataUser)
{
    cli_ioctl_tlv_t *tlv;
    cli_ioctl_tlv_t __user *tlvUser;
    int32_t i;
    if (copy_to_user((void *)dataUser, (void *)data, sizeof(cli_ioctl_hdr_t)))
    {
        pr_err("%s %d copy data hdr to user error\n", __func__, __LINE__);
		return -EFAULT;
    }

    for (i = 0; i < CLI_MAX_TLV; i++)
    {
        tlvUser = &dataUser->tlv[i];
        tlv = &data->tlv[i];
        if (copy_to_user((void *)tlvUser, (void *)tlv, sizeof(cli_ioctl_tlv_hdr_t)))
        {
            pr_err("%s %d copy tlv hdr to user error\n", __func__, __LINE__);
            return -EFAULT;
        }
        if (tlv->tlvhdr.len && tlv->val && tlvUser->val && (tlvUser->tlvhdr.len == tlv->tlvhdr.len))
        {
            if (copy_to_user((void *)tlvUser->val, (void *)tlv->val, tlv->tlvhdr.len))
            {
                pr_err("%s %d copy tlv data to user error\n", __func__, __LINE__);
                return -EFAULT;
            }
        }
    }

    return 0;
}

static long cmd_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    cli_ioctl_data_t data;
    cli_ioctl_data_t __user *dataUser = (cli_ioctl_data_t __user *)arg;
    int32_t ret = 0;

    memset_s(&data, sizeof(data), 0, sizeof(data));
    ret = cmd_copy_data_from_user(&data, dataUser);
    if (ret)
    {
        pr_err("%s %d copy data error ret %d\n", __func__, __LINE__, ret);
        return ret;
    }
    switch (cmd)
    {
        case CMDIOCTL:
            ret = cmd_process(&data);
            if (ret)
            {
                pr_err("%s %d cmd_process error ret %d\n", __func__, __LINE__, ret);
                cmd_free_data(&data);
                return ret;
            }
            ret = cmd_copy_data_to_user(&data, dataUser);
            cmd_free_data(&data);
            break;
        default:
            ret = -EOPNOTSUPP;
            break;
    }

    return ret;
}

static const struct file_operations fileOps = {
    .owner		= THIS_MODULE,
    .unlocked_ioctl = cmd_ioctl,
};


int32_t __init cmd_cdev_init(void)
{
    int32_t ret;

    memset_s(&gCmdDev, sizeof(gCmdDev), 0, sizeof(gCmdDev));
    ret = alloc_chrdev_region(&gCmdDev.devno, 0, 1, "cmdDev");
    if (ret)
    {
        pr_err("%s %d alloc chrdev fail ret %d\n", __func__, __LINE__, ret);
        goto alloc_cdev_err;
    }

    cdev_init(&gCmdDev.cdev, &fileOps);
    gCmdDev.cdev.owner = THIS_MODULE;
    ret = cdev_add(&gCmdDev.cdev, gCmdDev.devno, 1);
    if (ret)
    {
        pr_err("%s %d add cdev fail ret %d\n", __func__, __LINE__, ret);
        goto add_cdev_err;
    }

    gCmdDev.cls = class_create(THIS_MODULE, "cmdclass");
    if (IS_ERR(gCmdDev.cls))
    {
        ret = PTR_ERR(gCmdDev.cls);
        pr_err("%s %d create class fail ret %d\n", __func__, __LINE__, ret);
        goto class_err;
    }

    gCmdDev.dev = device_create(gCmdDev.cls, NULL, gCmdDev.devno, NULL, "cmdDev");
    if (IS_ERR(gCmdDev.dev))
    {
        ret = PTR_ERR(gCmdDev.dev);
        pr_err("%s %d create device fail ret %d\n", __func__, __LINE__, ret);
        goto device_err;
    }
    pr_info("%s %d cmd cdev init OK\n", __func__, __LINE__);

    return 0;

device_err:
    class_destroy(gCmdDev.cls);
class_err:
    cdev_del(&gCmdDev.cdev);
add_cdev_err:
    unregister_chrdev_region(gCmdDev.devno, 1);
alloc_cdev_err:
    return ret;
}

void __exit cmd_cdev_exit(void)
{
    cdev_del(&gCmdDev.cdev);
    unregister_chrdev_region(gCmdDev.devno, 1);
    device_destroy(gCmdDev.cls, gCmdDev.devno);
    class_destroy(gCmdDev.cls);
}

//module_init(cmd_cdev_init);
//module_exit(cmd_cdev_exit);