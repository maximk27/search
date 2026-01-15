#pragma once

#include "token.h"
#include <functional>
#include <string_view>

// for tokenizing doc text to be processed by
class Tokenizer {
public:
    // iterates over text, separating into tokens, which have fn run on them
    virtual void tokenize(std::string_view text,
                          std::function<void(const Token &token)> fn) = 0;
};
