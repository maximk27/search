#pragma once

#include <functional>
#include <string_view>
// for filtering tokens
class Filter {
public:
    // can be wrapped over tokenize to transform tokens before processed
    // or to decide if we ignore a token altogether
    virtual void apply(std::string_view token,
                       std::function<void(std::string_view)> fn) = 0;
};
