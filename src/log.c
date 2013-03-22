#include <stdio.h>

#include "log.h"

void
pl_log(log_t level, const char *fmt, ...)
{
  va_list argp;
  va_start(argp, fmt);
  pl_vlog(level, fmt, argp);
  va_end(argp);
}

void
pl_vlog(log_t level, const char *fmt, va_list argp)
{
  fprintf(stderr, "--> ");
  vfprintf(stderr, fmt, argp);
  fprintf(stderr, "\n");
}


