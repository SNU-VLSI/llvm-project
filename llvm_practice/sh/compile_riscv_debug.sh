#!/bin/bash

mkdir -p ./output

# get file ext and filename without ext
fn_base=$(basename $1)
fn_ext=$(echo "$fn_base" | sed 's/.*\.\(.*\)/\1/')
fn_no_ext=$(echo "$fn_base" | sed 's/\(.*\)\..*/\1/')

# emit llvm ir if .cpp, else move .ll to output
if [[ $fn_ext == "cpp" ]]; then
  clang -O1 --target=riscv64 -S -emit-llvm $1 -I../test_cpp -o output/$fn_no_ext.ll
elif [[ $fn_ext == "ll" ]]; then
  cp $1 output/$fn_no_ext.ll
else
  echo "Invalid file extension $fn_ext"
fi

# generate .s with debug flag
llc --march=riscv64 output/$fn_no_ext.ll -o ./output/$fn_no_ext.s \
  -debug \
  --debug-pass=Details \
  -print-after-all

# generate .o with debug flag
llc --march=riscv64 output/$fn_no_ext.ll -o ./output/$fn_no_ext.o \
  -filetype=obj \
  -debug \
  --debug-pass=Details \
  -print-after-all

# create binary using ld.lld
ld.lld -e 0 -Ttext 0x0 --verbose -o output/$fn_no_ext output/$fn_no_ext.o

# dump .o to .dump
llvm-objdump -dr --triple=riscv64 output/$fn_no_ext.o > output/$fn_no_ext.dump

# generate binary with llvm-objcopy
llvm-objcopy -O binary --only-section=.text output/$fn_no_ext.o output/$fn_no_ext.bin