#ifndef __OSAL_MEM_H__
#define __OSAL_MEM_H__
#if defined(__KERNEL__)
#include <linux/vmalloc.h>
#include <linux/string.h>
#endif


/*
 * Macro Definition
 */
#if defined(__KERNEL__)
#define osal_malloc     vmalloc
#define osal_free       vfree
#else
#define osal_malloc     malloc
#define osal_calloc     calloc
#define osal_realloc    realloc
#define osal_free       free
#endif
#define osal_memset     memset
#define osal_memcpy     memcpy
#define osal_memcmp     memcmp
#define osal_read    read
#define osal_write    write


#endif
