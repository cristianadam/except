@echo off

call cmake -H. -B%1 -D%2=ON -DCMAKE_BUILD_TYPE=Release -G%3 
REM call cmake -H. -B%1 -D%2=ON -DCMAKE_BUILD_TYPE=Release -G%3 -T %4

call cmake --build %1 --target "ALL_BUILD" --config Release
REM call cmake --build %1 --target "all" --config Release

set SUBDIR=Release
REM set SUBDIR=

call %1\%SUBDIR%\except_test.exe all > %1_all.txt
call %1\%SUBDIR%\except_test.exe readNullPointer > %1_readNullPointer.txt
call %1\%SUBDIR%\except_test.exe writeNullPointer > %1_writeNullPointer.txt
call %1\%SUBDIR%\except_test.exe nullSharePointer > %1_nullSharePointer.txt
call %1\%SUBDIR%\except_test.exe outOfBoundsVector > %1_outOfBoundsVector.txt
call %1\%SUBDIR%\except_test.exe divisionByZero > %1_divisionByZero.txt
