cmake_minimum_required(VERSION 3.29.2)
project(qsc)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

find_library(TS_PARSER, tree-sitter,
             PATHS /home/alexander/Coding/qsc/tree-sitter-qs)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
include_directories(src /home/alexander/Coding/qsc/tree-sitter-qs)

add_executable(${PROJECT_NAME} ${SOURCES})

llvm_map_components_to_libnames(llvm_libs support core irreader)

link_directories(../tree-sitter-qs)
target_link_libraries(${PROJECT_NAME} ${LLVM_LIBRARIES} ${TS_PARSER})
