#ifndef _PRELOADY_EMBEDPY_H_
#define _PRELOADY_EMBEDPY_H_

#include <Python.h>

#define DEFAULT_PRELOADY_MODULE "preloady"

#define RET_FAIL      0
#define RET_SUCCESS   1
#define RET_SKIP_ORIG 2
#define RET_CALL_ORIG 3

int       pl_py_init    (void); 
PyObject *pl_py_call    (const char *, const char *, ...);
void      pl_py_finish  (void);
int       pl_py_parse   (PyObject *, const char *, ...);

#endif
