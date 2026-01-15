#pragma once
#include <string_view>

// defines what a token does
class Token {
public:
    virtual std::string_view text() const = 0;
};
