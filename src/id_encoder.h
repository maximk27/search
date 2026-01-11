#pragma once

#include "gtest/gtest.h"
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>

// Encodes from T namespace to unique id namespace
// Can be used to compress from string -> int
// Or int -> int if in memory range is smaller
template <typename T>
class IdEncoder {
public:
    // sets id, requires not already registered
    void set_id(const T &val);

    // tries to get id, nullopt if not registered
    std::optional<int64_t> get_id(const T &val);

    // requires that data is registered
    const T &get_data(int64_t id);

    // size
    int64_t size() const;

private:
    // data -> id
    std::unordered_map<T, int64_t> m_to_id;

    // id -> data
    std::unordered_map<int64_t, T> m_to_data;
};

#include "id_encoder.inl"
