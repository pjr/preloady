#include <stdarg.h>
#include <arpa/inet.h>

#include "embedpy.h"
#include "log.h"

PyObject *module;
extern int in_py;

void hexdump(char *, void *, int);
    
void hexdump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);
    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).
    
        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);
        
            // Output the offset.
            printf ("  %04x ", i);
        }
    
        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

int
pl_py_init(void) 
{
  PyObject *name;

  Py_Initialize();

  if ((name = PyString_FromString(DEFAULT_PRELOADY_MODULE)) == NULL) {
    pl_log(MSG_ERROR, "PyString_FromString(%s) threw an error", DEFAULT_PRELOADY_MODULE);
    PyErr_Print();
    return RET_FAIL;
  }

  if ((module = PyImport_Import(name)) == NULL) {
    pl_log(MSG_ERROR, "PyImport_Import(%s) threw an error", DEFAULT_PRELOADY_MODULE);
    Py_DECREF(name);
    PyErr_Print();
    return RET_FAIL;
  }

  Py_DECREF(name);
  return RET_SUCCESS;
}

// TODO: DECREF memory at each MSG_ERROR portion
PyObject *
pl_py_call(const char *func, const char *fmt, ...)
{
  va_list argp;
  in_py = 1;

  PyObject *pyfunc = NULL, *pyargs = NULL, *pyres = NULL;

  pl_log(MSG_INFO, "pl_py_call(): Running PyObject_GetAttrString on '%s'", func);
  if ((pyfunc = PyObject_GetAttrString(module, func)) == NULL) {
    pl_log(MSG_ERROR, "pl_py_call(): PyObject_GetAttrString failed '%s'", func);
  }

  va_start(argp, fmt);
  pl_log(MSG_INFO, "pl_py_call(): Running PyBuild_Value on args with func '%s' fmt '%s'", func, fmt);
  if ((pyargs = Py_VaBuildValue(fmt, argp)) == NULL) {
    pl_log(MSG_ERROR, "Could not build value");
    va_end(argp);
  } 

  pl_log(MSG_INFO, "pl_py_call(): Running PyEval_CallObject");

  if ((pyres = PyObject_CallObject(pyfunc, pyargs)) == NULL) {
    pl_log(MSG_ERROR, "pl_py_call(): PyEval_CallObject() failed");
    PyErr_Print();
  }

  pl_log(MSG_ERROR, "pl_py_call(): Finished");
  in_py = 0;
  return pyres;
}

int
pl_py_parse(PyObject *obj, const char *fmt, ...) 
{
  va_list argp;

  va_start(argp, fmt);
  pl_log(MSG_INFO, "pl_py_prase_str(): Running PyArg_VaParse");
  if (!PyArg_VaParse(obj, fmt, argp)) {
    PyErr_Print();
    pl_log(MSG_ERROR, "pl_py_parse_str(): PyArg_Parse() - ERROR");
  }

  pl_log(MSG_INFO, "pl_py_parse returned successfully");
 
  return RET_SUCCESS;
}

void
pl_py_finish(void)
{
  Py_Finalize();
  Py_XDECREF(module);

  return;
}
