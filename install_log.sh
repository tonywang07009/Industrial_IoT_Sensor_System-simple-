#!/bin/bash

set -e

cd "$(dirname "$0")"  # The Root Directory

# The gcc compile
gcc -fPIC -c Log/log.c -o Log/log.o
gcc -shared -o Log/liblog.so Log/log.o

# The duplicate the lib to your usr/local

sudo cp Log/liblog.so /usr/local/lib/
sudo cp Log/log.h /usr/local/include/

sudo ldconfig

echo "log.so installed." 

# The message echo
# finshed chmod +x install_log.sh
# sudo ./install_log.sh