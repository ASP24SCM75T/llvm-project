#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/ExecutionEngine/ExecutionEngine.h"
#include "mlir/ExecutionEngine/RunnerUtils.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Export.h"

#include "llvm/Support/TargetSelect.h"
#include <iostream>

int main() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    mlir::MLIRContext context;
    context.loadAllAvailableDialects();
    mlir::OpBuilder builder(&context);
    mlir::ModuleOp module = mlir::ModuleOp::create(builder.getUnknownLoc());

    while (true) {
        getNextToken();
        if (CurTok == Token_EOF)
            break;
        if (CurTok == Token_Def) {
            auto func = parseFunction();
            codegenFunction(func.get(), builder, module);
        }
    }

    module.dump();

    // Later: Add execution engine to run
}
