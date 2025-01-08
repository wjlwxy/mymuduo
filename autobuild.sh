#!/bin/bash

set -e

if [ ! -d $PWD/build ]; then
    mkdir $PWD/build
fi

rm -rf $PWD/build/*

cd $PWD/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/mymuduo  so库拷贝到 /usr/lib  PATH
dir="/usr/include/mymuduo"
if [ ! -d $dir ]; then
    mkdir $dir
fi

for header in *.h
do
    cp "$header" /usr/include/mymuduo
done

cp $PWD/lib/libmymuduo.so /usr/lib

ldconfig