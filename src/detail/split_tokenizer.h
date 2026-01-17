#pragma once
#include <array>
#include <climits>
#include <cstdint>
#include <string>
#include <functional>
#include <string_view>
#include "../api/token.h"
#include "../api/tokenizer.h"
#include "basic_token.h"

// tokenize on delimiters as the split
class SplitTokenizer : public Tokenizer {
public:
    SplitTokenizer(const std::string &delimiters) {
        for (uint8_t val : delimiters) {
            m_delimiters[val] = true;
        }
    }

    void tokenize(std::string_view text,
                  std::function<void(const Token &token)> fn) override {
        int l = 0;
        for (size_t r = 0; r < text.size(); r++) {
            char c = text[r];
            if (m_delimiters[c]) {
                // [l, r)
                BasicToken token{
                    std::string_view{text.begin() + l, text.begin() + r}};

                fn(token);
                l = r + 1;
            }
        }
    }

private:
    // all false initially
    std::array<bool, CHAR_MAX> m_delimiters{};
};
