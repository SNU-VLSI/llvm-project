#!/bin/bash

mkdir -p ./output

# get file ext and filename without ext
basename=$(basename $1)
file_ext=$(echo "$basename" | sed 's/.*\.\(.*\)/\1/')
filename_no_ext=$(echo "$basename" | sed 's/\(.*\)\..*/\1/')

# emit llvm ir if .cpp, else move .ll to output
if [[ $file_ext == "cpp" ]]; then
  clang -O1 --target=IMCE -S -emit-llvm $1 -I../test_cpp -o output/$filename_no_ext.ll
elif [[ $file_ext == "ll" ]]; then
  mv $1 output/$filename_no_ext.ll
else
  echo "Invalid file extension"
fi

# generate .s with debug flag
llc --march=IMCE output/$filename_no_ext.ll -o ./output/$filename_no_ext.s \
  -force-hardware-loops \
  -force-nested-hardware-loop \
  -debug

# generate .o with debug flag
llc --march=IMCE output/$filename_no_ext.ll -o ./output/$filename_no_ext.o \
  -force-hardware-loops \
  -force-nested-hardware-loop \
  -filetype=obj \
  -debug

# create binary using ld.lld
ld.lld -e 0 -Ttext 0x0 -o output/$filename_no_ext output/$filename_no_ext.o

# dump .o to .dump
llvm-objdump -dr --triple=IMCE output/$filename_no_ext.o > output/$filename_no_ext.dump

# generate binary with llvm-objcopy
llvm-objcopy -O binary --only-section=.text output/$filename_no_ext.o output/$filename_no_ext.bin