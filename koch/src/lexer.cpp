#include "lexer.h"
#include <cctype>
#include <cstdio>

std::string IdentifierStr;
double NumVal;
int CurTok;

static int LastChar = ' ';

int getNextToken() {
    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def") return Token_Def;
        if (IdentifierStr == "var") return Token_Var;
        if (IdentifierStr == "return") return Token_Return;
        if (IdentifierStr == "print") return Token_Print;
        return Token_Identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return Token_Number;
    }

    if (LastChar == '#') {
        do {
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        return getNextToken();
    }

    if (LastChar == EOF || LastChar == -1)
        return Token_EOF;

    int ThisChar = LastChar;
    LastChar = getchar();
    switch (ThisChar) {
        case ';': return Token_Semicolon;
        case ',': return Token_Comma;
        case '(': return Token_LParen;
        case ')': return Token_RParen;
        case '{': return Token_LBrace;
        case '}': return Token_RBrace;
        case '+': return Token_Plus;
        case '-': return Token_Minus;
        case '*': return Token_Star;
        case '/': return Token_Slash;
    }

    return Token_EOF;
}
