#!/bin/bash

( echo set env DYLD_INSERT_LIBRARIES $PWD/build/preloady.dylib ;
  echo set env DYLD_FORCE_FLAT_NAMESPACE 1 ;
  echo set env PYTHONPATH="py"
  echo run 192.168.1.13 8191
  cat ) | gdb $PWD/test/tcp_test
