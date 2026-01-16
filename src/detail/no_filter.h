#pragma once

#include "../api/filter.h"

// naive filter nothing at all
class NoFilter : public Filter {
public:
    virtual void apply(const Token &token,
                       std::function<void(const Token &)> fn) {
        fn(token);
    }
};
