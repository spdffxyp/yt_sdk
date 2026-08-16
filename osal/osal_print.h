#ifndef __OSAL_PRINT_H__
#define __OSAL_PRINT_H__

#if defined(__KERNEL__)
#include <linux/string.h>
#endif

/*add other space later */
#if defined(__KERNEL__)
#define osal_printf                    printk
#define osal_strtoul                simple_strtoul
#define osal_strtou32            simple_strtoul
#else
#define osal_printf                    printf
#define osal_strtoul                strtoul
#define osal_strtou32            strtoul
#endif
#define osal_sprintf                  sprintf
#define osal_strncmp              strncmp
#define osal_strcmp                strcmp
#define osal_strlen                  strlen
#define osal_strcpy                  strcpy
#define osal_strdup                 strdup
#define osal_strchr                 strchr
#define osal_isspace               isspace


#endif
