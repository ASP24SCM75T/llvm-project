#pragma once
#include "ast.h"
#include <memory>

std::unique_ptr<FunctionAST> parseFunction();
