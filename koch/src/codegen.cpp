#include "codegen.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include <unordered_map>

static mlir::Value codegenExpr(ExprAST *expr, mlir::OpBuilder &builder,
                               std::unordered_map<std::string, mlir::Value> &namedValues) {
    if (auto *N = dynamic_cast<NumberExprAST*>(expr)) {
        auto f64Type = builder.getF64Type();
        return builder.create<mlir::arith::ConstantOp>(
            builder.getUnknownLoc(), f64Type, builder.getF64FloatAttr(N->Val));
    }
    if (auto *V = dynamic_cast<VariableExprAST*>(expr)) {
        return namedValues[V->Name];
    }
    return nullptr;
}

bool codegenFunction(FunctionAST* funcAst, mlir::OpBuilder &builder, mlir::ModuleOp module) {
    std::vector<mlir::Type> paramTypes(funcAst->Params.size(), builder.getF64Type());
    mlir::FunctionType funcType = builder.getFunctionType(paramTypes, {builder.getF64Type()});

    auto func = builder.create<mlir::func::FuncOp>(
        builder.getUnknownLoc(), funcAst->Name, funcType);
    module.push_back(func);

    auto &entryBlock = *func.addEntryBlock();
    builder.setInsertionPointToStart(&entryBlock);

    std::unordered_map<std::string, mlir::Value> namedValues;
    for (unsigned i = 0; i < funcAst->Params.size(); ++i)
        namedValues[funcAst->Params[i]] = entryBlock.getArgument(i);

    if (funcAst->ReturnExpr) {
        auto retVal = codegenExpr(funcAst->ReturnExpr.get(), builder, namedValues);
        builder.create<mlir::func::ReturnOp>(builder.getUnknownLoc(), retVal);
    }

    return true;
}
