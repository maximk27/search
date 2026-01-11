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
    // get an unique id
    int64_t get_id(const T &val);

    const T &get_data(int64_t id);

    bool contains(const T &val) const;

    int64_t size() const;

private:
    // data -> id
    std::unordered_map<T, int64_t> m_to_id;

    // id -> data
    std::unordered_map<int64_t, T> m_to_data;
};

#include "id_encoder.inl"
