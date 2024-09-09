cmake_minimum_required(VERSION 3.20.0)
project(SimpleProject)

set(LLVM_DIR /root/project/llvm-project/builddir/lib/cmake/llvm)
set(CMAKE_BUILD_TYPE Debug)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
include_directories(${LLVM_INCLUDE_DIRS} /root/project/llvm-project/llvm/lib/Target /root/project/llvm-project/llvm/lib/Target/RISCV /root/project/llvm-project/llvm/lib/Target/RISCV/TargetInfo /root/project/llvm-project/llvm/lib/Target/RISCV/AsmParser /root/project/llvm-project/llvm/lib/Target/RISCV/Disassembler /root/project/llvm-project/llvm/lib/Target/RISCV/MCTargetDesc /root/project/llvm-project/builddir/lib/Target/RISCV)
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Now build our tools
add_executable(test_dag test_dag.cpp)
add_executable(test_target_related test_target_related.cpp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core target targetparser riscvtargetmca riscvdisassembler riscvasmparser riscvcodegen riscvdesc riscvinfo)

# llvm_map_components_to_libnames(llvm_libs support core target targetparser riscvtargetmca riscvdisassembler riscvasmparser riscvcodegen riscvdesc riscvinfo imcetargetmca imceasmparser imcecodegen imcedesc imceinfo)

# llvm_map_components_to_libnames(llvm_libs all)
# execute_process(COMMAND llvm-config --libs all OUTPUT_VARIABLE LLVM_LIBRARIES)

# -lLLVMRISCVTargetMCA -lLLVMRISCVDisassembler -lLLVMRISCVAsmParser -lLLVMRISCVCodeGen -lLLVMRISCVDesc -lLLVMRISCVInfo

# Link against LLVM libraries
target_link_libraries(test_dag ${llvm_libs} /usr/local/lib/gtest/libgtest.a /usr/local/lib/gtest/libgtest_main.a)
target_link_libraries(test_target_related ${llvm_libs} /usr/local/lib/gtest/libgtest.a /usr/local/lib/gtest/libgtest_main.a)