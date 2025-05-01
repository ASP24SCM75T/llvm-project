#pragma once
#include "ast.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinOps.h"

bool codegenFunction(FunctionAST* funcAst, mlir::OpBuilder &builder, mlir::ModuleOp module);
