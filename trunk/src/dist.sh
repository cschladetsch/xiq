#!/bin/sh

export NAME=xiq-0.2-src

doxygen

mkdir $NAME
cp xiq.cbp *.cpp *.h README $NAME
zip $NAME.zip $NAME/*
rm -rf $NAME
mv -f $NAME.zip archive
