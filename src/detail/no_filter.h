#pragma once

#include "../api/filter.h"

// naive filter nothing at all
class NoFilter : public Filter {
public:
    void apply(std::string_view token,
               std::function<void(std::string_view)> fn) override {
        fn(token);
    }
};
