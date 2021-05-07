#!/bin/bash

# Author: Michal Kukowski
# email: michalkukowski10@gmail.com

# Full path of this script
THIS_DIR=`readlink -f "${BASH_SOURCE[0]}" 2>/dev/null||echo $0`

# This directory path
DIR=`dirname "${THIS_DIR}"`

# Chose your install directory, or install to default path
DEFAULT_DIR=~/kassert
lib_dir=

# Check existing of argument
if [ ! -z "$1" ]; then
    lib_dir=$1
else
    lib_dir=$DEFAULT_DIR
fi

echo "DIR = $lib_dir"

# Compile KAssert
cd "$DIR"
cd ../
make lib

# Now we have compiled kassert into libkassert.a we need also a inc/ directory

# Create dirs and copy required files
echo "Installing kassert to $lib_dir ..."
mkdir -p "$lib_dir"
cp ./libkassert.a $lib_dir/
cp -R ./inc/ $lib_dir

echo "DONE"