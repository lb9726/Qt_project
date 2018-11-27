#! /bin/bash
EXE='VideoPlayer'
PWD=`pwd`
files=`ldd $EXE | awk '{ if(match($3,"^/"))
        printf("%s "),$3 }'`
cp $files $PWD
