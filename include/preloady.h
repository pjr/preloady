#ifndef _PRELOADY_PRELOADY_H_
#define _PRELOADY_PRELOADY_H_

#include <sys/types.h>
#include <sys/socket.h>

// init function
void _init(void) __attribute__ ((constructor));

// original functions that we're overriding
int     (*orig_connect) (int , const struct sockaddr *, socklen_t );
ssize_t (*orig_send)    (int , const void *, size_t, int );
ssize_t (*orig_write)   (int , const void *, size_t);

// Lookup table mapping system function to new function.

struct funcs {
  char *from;
  void **to;
};

struct funcs pl_funcs[] = {
  { "connect", (void **)&orig_connect },
};

#endif
