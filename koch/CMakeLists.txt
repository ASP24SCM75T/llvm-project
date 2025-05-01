cmake_minimum_required(VERSION 3.15)
project(kochCompiler CXX)

set(CMAKE_CXX_STANDARD 17)

# Adjust these to where you built LLVM/MLIR
find_package(LLVM REQUIRED CONFIG HINTS "/workspaces/llvm-project/build/lib/cmake/llvm")
find_package(MLIR REQUIRED CONFIG HINTS "/workspaces/llvm-project/build/lib/cmake/mlir")

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# 👇 Add this to include generated .inc files!
include_directories("/workspaces/llvm-project/build/tools/mlir/include")

add_definitions(${LLVM_DEFINITIONS})
link_directories(${LLVM_LIBRARY_DIRS})
link_directories(${MLIR_LIBRARY_DIRS})

add_executable(kochc
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/codegen.cpp
)

target_link_libraries(kochc
    MLIRIR
    MLIRParser
    MLIRSupport
    MLIRTransforms
    MLIRExecutionEngine

    # ✅ These three are essential for most MLIR-based compilers
    MLIRArithDialect
    MLIRFuncDialect
    MLIRControlFlowDialect

    LLVMCore
    LLVMSupport
    LLVMExecutionEngine
)