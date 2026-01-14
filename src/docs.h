#pragma once
#include "id_encoder.h"
#include <cstdint>
#include <string>
#include <string_view>

// api for doc collection
class Docs {
public:
    // getters
    virtual std::string_view text(int64_t id) = 0;

    // for each
    // break on return true eval
    // func(id, text)
    virtual void for_each(
        std::function<bool(int64_t id, std::string_view text)> fn) const = 0;

    virtual size_t size() const = 0;
};
