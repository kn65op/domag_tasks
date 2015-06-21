#!/bin/bash

function mkandcdandrmrfToDir
{
	mkdir -p $1 
	cd $1 && rm -rf *
	echo ${PWD}
}

function goBackToMainDir
{
	cd ..
}

rm warnings.txt
rm warnings_cmake.txt
rm warnings_test.txt


mkandcdandrmrfToDir "Debug-GCC"
( ../cmakeDebug .. 2>> ../warnings_cmake.txt && make -j 2>> ../warnings.txt && Test/Tester 2>> ../warnings_test.txt || ( echo "Debug gcc failed" && exit 1 ) ) || exit 1
goBackToMainDir

mkandcdandrmrfToDir "Release-GCC"
( ../cmakeRelease ..  2>> ../warnings_cmake.txt && make -j 2>>../warnings.txt || ( echo "Release gcc failed" && exit 1 ) ) || exit 1
goBackToMainDir

mkandcdandrmrfToDir "Debug-Clang"
( ../cmakeDebug -DCMAKE_CXX_COMPILER=clang++ .. 2>> ../warnings_cmake.txt && make -j 2>> ../warnings.txt && Test/Tester 2>> ../warnings_test.txt || ( echo "Debug clang failed" && exit 1 ) ) || exit 1
goBackToMainDir

mkandcdandrmrfToDir "Release-Clang"
( ../cmakeRelease DCMAKE_CXX_COMPILER=clang++ .. 2>> ../warnings_cmake.txt && make -j 2>> ../warnings.txt || ( echo "Release clang failed" && exit 1 ) ) || exit 1
goBackToMainDir

echo "All suceed"
