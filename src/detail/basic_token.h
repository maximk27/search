#pragma once
#include "../api/token.h"

class BasicToken : public Token {
public:
    std::string_view text() const override {
        return data;
    }

    BasicToken(std::string_view _data) : data(_data) {
    }

    std::string_view data;
};
