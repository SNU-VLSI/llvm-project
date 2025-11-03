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
include_directories(${LLVM_INCLUDE_DIRS} /root/project/llvm-project/llvm/lib/Target)
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core target targetparser)

# Add custom test targets for IMCE tests
add_custom_target(test_imce
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/sh/test_all_imce.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running IMCE tests"
)

add_custom_target(test_imce_debug
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/sh/test_all_imce.sh -d
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running IMCE tests (debug mode)"
)

# Add custom test targets for INODE tests
add_custom_target(test_inode
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/sh/test_all_inode.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running INODE tests"
)

add_custom_target(test_inode_debug
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/sh/test_all_inode.sh -d
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running INODE tests (debug mode)"
)

# Add a target to run all tests
add_custom_target(test_all
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/sh/test_all_imce.sh
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/sh/test_all_inode.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all IMCE and INODE tests"
)