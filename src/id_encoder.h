#pragma once

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
    int64_t get_id(const T &val) {
        auto it = m_to_id.find(val);
        // exists, so return it
        if (it != m_to_id.end()) {
            return it->second;
        }

        // create for first time
        int64_t id = m_to_id.size();

        // both ways
        m_to_data.emplace(id, val);
        m_to_id.emplace(val, id);

        return id;
    }

    const T &get_data(int64_t id) {
        // requires that this entry exists
        return m_to_data.at(id);
    }

private:
    // data -> id
    std::unordered_map<T, int64_t> m_to_id;

    // id -> data
    std::unordered_map<int64_t, T> m_to_data;
};
