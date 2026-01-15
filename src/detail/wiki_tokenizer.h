#pragma once
#include "../api/tokenizer.h"
#include "../api/token.h"
#include <cstddef>
#include <stack>
#include <string_view>

// Wiki Token types
enum class WikiTokenType {
    None = 0, // impl detail, not registered
    Text,     // <space>text<space>
    Section,  // ==text==
    Bold,     // '''text'''
    Italic,   // ''text''
    Tag,      // <name>text</name>
    Link,     // [[text]]
    Template, // {{text}}
};
// link and template do not allow
// inside nesting

// Token returned from wiki
class WikiToken : public Token {
public:
    std::string_view text() const override {
        return data;
    }

public:
    WikiTokenType type;
    std::string_view data;
    std::string_view opening_delim;
};

// For tokenizing doc wiki md text
class WikiTokenizer : public Tokenizer {
public:
    // iterates over text, separating into tokens, which have fn run on them
    void tokenize(std::string_view text,
                  std::function<void(const Token &token)> fn) override;
};
