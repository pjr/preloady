#ifndef _PRELOADY_LOG_H_
#define _PRELOADY_LOG_H_

#include <stdarg.h>

typedef enum { 
  MSG_INFO, 
  MSG_WARNING, 
  MSG_ERROR 
} log_t;

void pl_log   (log_t, const char *, ...);
void pl_vlog  (log_t, const char *, va_list);

#endif

