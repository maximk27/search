#pragma once

#include <functional>
#include <string_view>
#include "token.h"

// for filtering tokens
class Filter {
public:
    // can be wrapped over tokenize to transform tokens before processed
    // or to decide if we ignore a token altogether
    virtual void apply(const Token &token,
                       std::function<void(const Token &)> fn) = 0;
};
