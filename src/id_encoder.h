#pragma once

#include "gtest/gtest.h"
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>

// Encodes from T namespace to unique id namespace
// Can be used to compress from string -> int
// Or int -> int if in memory range is smaller
// guaranteed to provide ids in dense [0, size)
template <typename T>
class IdEncoder {
public:
    // encode, set if not exist, else return existing
    int64_t encode(const T &val);

    // fetch existing encoding
    int64_t fetch_encoding(const T &val) const;

    // decode requires that encoding exists
    const T &decode(int64_t id) const;

    // this has been encoded before
    bool contains(const T &val) const;
    bool contains(int64_t id) const;

    // size
    size_t size() const;

private:
    // data -> id
    std::unordered_map<T, int64_t> m_to_id;

    // id -> data
    std::vector<T> m_to_data;
};

#include "id_encoder.inl"
