#include "parser.h"
#include "lexer.h"
#include <cstdio>

// Forward declarations
static std::unique_ptr<ExprAST> parseExpression();

static std::unique_ptr<ExprAST> parsePrimary() {
    if (CurTok == Token_Number) {
        auto Result = std::make_unique<NumberExprAST>(NumVal);
        getNextToken();
        return std::move(Result);
    }
    if (CurTok == Token_Identifier) {
        std::string IdName = IdentifierStr;
        getNextToken();

        if (CurTok != Token_LParen)
            return std::make_unique<VariableExprAST>(IdName);

        getNextToken();
        std::vector<std::unique_ptr<ExprAST>> Args;
        if (CurTok != Token_RParen) {
            while (true) {
                if (auto Arg = parseExpression())
                    Args.push_back(std::move(Arg));
                else
                    return nullptr;

                if (CurTok == Token_RParen)
                    break;

                if (CurTok != Token_Comma)
                    return nullptr;
                getNextToken();
            }
        }
        getNextToken();
        return std::make_unique<CallExprAST>(IdName, std::move(Args));
    }

    return nullptr;
}

static std::unique_ptr<ExprAST> parseExpression() {
    auto LHS = parsePrimary();
    return LHS;
}

std::unique_ptr<FunctionAST> parseFunction() {
    getNextToken(); // eat 'def'

    if (CurTok != Token_Identifier)
        return nullptr;

    std::string FuncName = IdentifierStr;
    getNextToken();

    if (CurTok != Token_LParen)
        return nullptr;
    getNextToken();

    std::vector<std::string> Params;
    if (CurTok != Token_RParen) {
        while (true) {
            if (CurTok != Token_Identifier)
                return nullptr;
            Params.push_back(IdentifierStr);
            getNextToken();

            if (CurTok == Token_RParen)
                break;
            if (CurTok != Token_Comma)
                return nullptr;
            getNextToken();
        }
    }
    getNextToken(); // eat ')'

    if (CurTok != Token_LBrace)
        return nullptr;
    getNextToken(); // eat '{'

    auto FnAST = std::make_unique<FunctionAST>(FuncName, Params);

    while (CurTok != Token_RBrace && CurTok != Token_EOF) {
        if (CurTok == Token_Return) {
            getNextToken();
            FnAST->ReturnExpr = parseExpression();
            getNextToken();
        } else {
            auto Stmt = parseExpression();
            if (!Stmt)
                return nullptr;
            FnAST->Body.push_back(std::make_unique<ExprStmtAST>(std::move(Stmt)));
            getNextToken();
        }
    }
    getNextToken(); // eat '}'

    return FnAST;
}
