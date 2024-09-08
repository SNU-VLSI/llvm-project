#!/bin/zsh

# clang -O1 --target=riscv64 -S -emit-llvm test.c -o test.ll --sysroot="/opt/riscv/sysroot" --gcc-toolchain="/opt/riscv"
# clang -O1 --target=riscv64 -S -emit-llvm test.c -o test.ll --sysroot="/opt/riscv/sysroot" -I/opt/riscv/sysroot/usr/include
clang -O0 --target=riscv64 -S -emit-llvm test.c -o test.ll --sysroot="/opt/riscv/riscv64-unknown-elf" --gcc-toolchain="/opt/riscv"
opt -S -passes='mem2reg' ./test.ll -o test.opt.ll
llc -march=riscv64 test.opt.ll -o test.opt.s