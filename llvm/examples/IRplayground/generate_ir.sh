# script to generate and LLVM IR for *.cpp files
# usage: ./generate_ir.sh <source_file.cpp>

# get file name with the .cpp extension
fn=${1%.cpp}

# generate the LLVM IR in a .ll file
clang++ -S -O1 -mllvm -disable-llvm-optzns -emit-llvm $fn.cpp

# generate a mem2reg optimized LLVM IR in a .mem2reg.ll file
llvm-as < $fn.ll | opt -passes=mem2reg | llvm-dis > $fn.mem2reg.ll