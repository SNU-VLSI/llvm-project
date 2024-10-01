#!/bin/zsh
basename=$(basename $1)
filename_no_ext=$(echo "$basename" | sed 's/\(.*\)\..*/\1/')
# clang -O1 --target=riscv64 -S -emit-llvm test.c -o test.ll --sysroot="/opt/riscv/sysroot" --gcc-toolchain="/opt/riscv"
# clang -O1 --target=riscv64 -S -emit-llvm test.c -o test.ll --sysroot="/opt/riscv/sysroot" -I/opt/riscv/sysroot/usr/include
clang -O0 --target=riscv64 -S -emit-llvm $1 -o ${filename_no_ext}.ll --sysroot="/opt/riscv/riscv64-unknown-elf" --gcc-toolchain="/opt/riscv"
opt -S -passes='mem2reg' ${filename_no_ext}.ll -o ${filename_no_ext}.opt.ll
llc -march=riscv64 ${filename_no_ext}.opt.ll -o ${filename_no_ext}.opt.s