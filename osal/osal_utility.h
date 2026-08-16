#ifndef __OSAL_UTILITY_H__
#define __OSAL_UTILITY_H__

#if defined(OS_CYGWIN) || defined(OS_LINUX_USR) ||defined(OS_Linux)
#define osal_open                   open
#define osal_close                  close
#endif

#endif
