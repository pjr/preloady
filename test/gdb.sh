#!/bin/bash
( echo set env DYLD_INSERT_LIBRARIES /Users/philip.reynolds/src/c/preloady/build/preloady.dylib 
echo set env DYLD_FORCE_FLAT_NAMESPACE 1 ; 
echo set env PYTHONPATH /Users/philip.reynolds/src/c/preloady/py 
echo run 192.168.1.13 testing ;
cat ) | gdb /Users/philip.reynolds/src/c/preloady/test/tcp_test
