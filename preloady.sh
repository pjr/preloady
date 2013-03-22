#!/bin/bash
export DYLD_INSERT_LIBRARIES=/Users/philip.reynolds/src/c/preloady/build/preloady.dylib
export DYLD_FORCE_FLAT_NAMESPACE=1
PYTHONPATH="py" $@
