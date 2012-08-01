#!/bin/sh

curl -JLO "http://sourceforge.net/projects/libircclient/files/latest/download"
tar xf libircclient*tar*
rm libircclient*tar*
mv libircclient* libircclient
cd libircclient
./configure --enable-debug
make
cd ..
