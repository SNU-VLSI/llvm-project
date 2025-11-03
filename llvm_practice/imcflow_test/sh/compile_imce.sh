#!/bin/bash

mkdir -p ./output

# get file ext and filename without ext
fn_base=$(basename $1)
fn_ext=$(echo "$fn_base" | sed 's/.*\.\(.*\)/\1/')
fn_no_ext=$(echo "$fn_base" | sed 's/\(.*\)\..*/\1/')

# compile with clang/llc and output to .o
if [[ $fn_ext == "cpp" ]]; then
  clang -O1 --target=IMCE -c \
        -mllvm="-force-hardware-loops" \
        -mllvm="-force-nested-hardware-loop" \
        $1 -o output/$fn_no_ext.o
elif [[ $fn_ext == "ll" ]]; then
  llc --march=IMCE \
      -force-hardware-loops \
      -force-nested-hardware-loop \
      -filetype=obj \
      $1 -o output/$fn_no_ext.o
else
  echo "Invalid file extension $fn_ext"
fi

# create binary using ld.lld
ld.lld -e 0 -Ttext 0x0 -o output/$fn_no_ext output/$fn_no_ext.o

# extract .text with llvm-objcopy
llvm-objcopy -O binary --only-section=.text output/$fn_no_ext output/$fn_no_ext.bin