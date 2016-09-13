#!/bin/bash

function build()
{
    #cmake -H. -Bbin_$1 -D$2=ON -DCMAKE_BUILD_TYPE=Release -G"MSYS Makefiles"
    cmake -H. -Bbin_$1 -D$2=ON -DCMAKE_BUILD_TYPE=Release
    cmake --build bin_$1 --target all
}

function runTests()
{
    bin_$1/except_test all > bin_$1_all.txt
    bin_$1/except_test readNullPointer > bin_$1_readNullPointer.txt
    bin_$1/except_test writeNullPointer > bin_$1_writeNullPointer.txt
    bin_$1/except_test nullSharePointer > bin_$1_nullSharePointer.txt
    bin_$1/except_test outOfBoundsVector > bin_$1_outOfBoundsVector.txt
    bin_$1/except_test divisionByZero > bin_$1_divisionByZero.txt
}

export CXX=g++

build gcc_sigaction USE_POSIX_SIGACTION
build gcc_signal USE_POSIX_SIGNAL

#export CXX=clang-3.5
export CXX=clang++

build clang_sigaction USE_POSIX_SIGACTION
build clang_signal USE_POSIX_SIGNAL

runTests gcc_sigaction
runTests gcc_signal
runTests clang_sigaction
runTests clang_signal
