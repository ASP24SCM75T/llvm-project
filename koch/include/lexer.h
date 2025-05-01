#pragma once
#include <string>

enum Token {
    Token_EOF = 0,
    Token_Def,
    Token_Var,
    Token_Return,
    Token_Print,
    Token_Identifier,
    Token_Number,
    Token_Semicolon,
    Token_Comma,
    Token_LParen, Token_RParen,
    Token_LBrace, Token_RBrace,
    Token_Plus, Token_Minus, Token_Star, Token_Slash
};

extern std::string IdentifierStr;
extern double NumVal;
extern int CurTok;

int getNextToken();
