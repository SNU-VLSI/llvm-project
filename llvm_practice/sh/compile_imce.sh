#!/bin/bash

mkdir -p ./output

# get file ext and filename without ext
basename=$(basename $1)
file_ext=$(echo "$basename" | sed 's/.*\.\(.*\)/\1/')
filename_no_ext=$(echo "$basename" | sed 's/\(.*\)\..*/\1/')

# compile with clang/llc and output to .o
if [[ $file_ext == "cpp" ]]; then
  clang -O1 --target=IMCE -c \
        -mllvm="-force-hardware-loops" \
        -mllvm="-force-nested-hardware-loop" \
        $1 -o output/$filename_no_ext.o
elif [[ $file_ext == "ll" ]]; then
  llc --march=IMCE \
      -force-hardware-loops \
      -force-nested-hardware-loop \
      -filetype=obj \
      $1 -o output/$filename_no_ext.o
else
  echo "Invalid file extension"
fi

# create binary using ld.lld
ld.lld -e 0 -Ttext 0x0 -o output/$filename_no_ext output/$filename_no_ext.o

# extract .text with llvm-objcopy
llvm-objcopy -O binary --only-section=.text output/$filename_no_ext output/$filename_no_ext.bin