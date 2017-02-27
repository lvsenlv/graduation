#!/bin/bash

if [ ! -z $1 ];then
    DIR_DEPTH=$1
else
    DIR_DEPTH=1
fi 

SRCS_PATH=`find ../src/ -maxdepth $DIR_DEPTH -type d | sed 's/^'"..\/src\/"'//g'`
cd ../src/ 

for dir in $SRCS_PATH;
do
    rm -rf $dir/Makefile
done

