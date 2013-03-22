#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <arpa/inet.h>
#include "embedpy.h"
#include "preloady.h"
#include "log.h"

int in_py;

void
_init(void)
{
  pl_log(MSG_INFO, "PYTHONPATH = [%s]", getenv("PYTHONPATH"));
  if (pl_py_init() == RET_FAIL) {
    pl_log(MSG_ERROR, "pl_py_init() returned NULL");
    return;
  }

  pl_log(MSG_INFO, "_init(): Setting orig_connect to dlsym(RTLD_NEXT, connect)");

  orig_connect = dlsym(RTLD_NEXT, "connect"); 
  orig_send    = dlsym(RTLD_NEXT, "send"); 
  orig_write   = dlsym(RTLD_NEXT, "write"); 

  pl_log(MSG_INFO, "_init(): done.");
  
  in_py = 0;
}

int
connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
  if (in_py == 0) {
    PyObject *obj;
  
    pl_log(MSG_INFO, "connect(): Entered ... now running pl_py_call()");
    obj = pl_py_call("connect", "(is#)", socket, address, sizeof(struct sockaddr));
  
    pl_log(MSG_INFO, "connect(): Running pl_py_parse()");
    printf("SOCKET BEFORE = [%d]\n", socket);
    if (pl_py_parse(obj, "is#", &socket, &address, &address_len) != RET_SUCCESS) {
      pl_log(MSG_ERROR, "connect(): Got NULL back from pl_py_parse");
    }
  
    printf("SOCKET AFTER = [%d]\n", socket);
      
    pl_log(MSG_INFO, "connect(): Running orig_connect()");
  }
  return orig_connect(socket, (struct sockaddr *)address, address_len);
}

ssize_t
send(int socket, const void *buf, size_t length, int flags)
{
  pl_log(MSG_INFO, "send(): Entered now running pl_py_call()");
  return orig_send(socket, buf, length, flags);
}

ssize_t
write(int fildes, const void *buf, size_t nbyte)
{
  char *str = NULL;

  pl_log(MSG_INFO, "write(): Entered now running pl_py_call()");
  /*
  obj = pl_py_call("write", "(s#)", buf, nbyte);
  if ((str = pl_py_parse_str(obj)) == NULL) {
    pl_log(MSG_ERROR, "write(): Got NULL back from pl_py_parse_str");
  }
  */
  
  pl_log(MSG_INFO, "write(): Running orig_write()");
  return orig_write(fildes, str, strlen(str) + 1);
}
