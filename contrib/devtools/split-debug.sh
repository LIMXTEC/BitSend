#!/bin/sh

if [ $# -ne 3 ];
    then echo "usage: $0 <input> <stripped-binary> <debug-binary>"
fi

/c/mingw32/bin/objcopy --enable-deterministic-archives -p --only-keep-debug $1 $3
/c/mingw32/bin/objcopy --enable-deterministic-archives -p --strip-debug $1 $2
/c/mingw32/bin/strip --enable-deterministic-archives -p -s $2
/c/mingw32/bin/objcopy --enable-deterministic-archives -p --add-gnu-debuglink=$3 $2
