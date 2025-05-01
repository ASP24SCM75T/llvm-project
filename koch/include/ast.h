#pragma once
#include <memory>
#include <vector>
#include <string>

class ExprAST {
public:
    virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST {
public:
    double Val;
    NumberExprAST(double val) : Val(val) {}
};

class VariableExprAST : public ExprAST {
public:
    std::string Name;
    VariableExprAST(const std::string &name) : Name(name) {}
};

class BinaryExprAST : public ExprAST {
public:
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;
    BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
        : Op(op), LHS(std::move(lhs)), RHS(std::move(rhs)) {}
};

class CallExprAST : public ExprAST {
public:
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
    CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> args)
        : Callee(callee), Args(std::move(args)) {}
};

class VarDeclAST {
public:
    std::string Name;
    std::unique_ptr<ExprAST> InitExpr;
    VarDeclAST(std::string name, std::unique_ptr<ExprAST> init)
        : Name(std::move(name)), InitExpr(std::move(init)) {}
};

class ExprStmtAST {
public:
    std::unique_ptr<ExprAST> Expr;
    ExprStmtAST(std::unique_ptr<ExprAST> expr) : Expr(std::move(expr)) {}
};

class FunctionAST {
public:
    std::string Name;
    std::vector<std::string> Params;
    std::vector<std::unique_ptr<VarDeclAST>> VarDecls;
    std::vector<std::unique_ptr<ExprStmtAST>> Body;
    std::unique_ptr<ExprAST> ReturnExpr;

    FunctionAST(std::string name, std::vector<std::string> params)
        : Name(std::move(name)), Params(std::move(params)) {}
};
