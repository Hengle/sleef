#!/bin/bash
set -ev
cd /build/build-cross
make -j 1 all
export OMP_WAIT_POLICY=passive
export CTEST_OUTPUT_ON_FAILURE=TRUE
ctest -j `nproc`
make install
