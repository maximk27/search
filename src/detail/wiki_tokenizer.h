#pragma once
#include "../api/tokenizer.h"
#include "../api/token.h"

class WikiToken : public Token {
public:
};

// for tokenizing doc wiki md text
class WikiTokenizer : public Tokenizer {
public:
    // iterates over text, separating into tokens, which have fn run on them
    void tokenize(std::string_view text,
                  std::function<void(const Token &token)> fn) override;
};
