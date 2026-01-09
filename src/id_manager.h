#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// NOTE: may be necessary to upgrade entity to int128_t
using Id = int64_t;

// Compresses from T namespace to unique id namespace
// Can be used to compress from string -> int
// Or int -> int if in memory range is smaller
template <typename T>
class Compressor {
public:
    Id get_id();

private:
    Id count = 0;
    // entity -> name
    std::vector<T> m_to_leftside;
    // name -> leftside
    std::unordered_map<T, int64_t> m_to_entity;
};
