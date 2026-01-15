#pragma once
#include "id_encoder.h"
#include <cassert>
#include <spdlog/spdlog.h>
#include <stdexcept>

// ---------------------------------- public ----------------------------------
template <typename T>
int64_t IdEncoder<T>::encode(const T &val) {
    auto it = m_to_id.find(val);
    // exists
    if (it != m_to_id.end())
        return it->second;

    // next id
    int64_t id = size();

    // move into place
    m_to_id.emplace(val, id);
    m_to_data.push_back(val);

    return id;
}

template <typename T>
int64_t IdEncoder<T>::fetch_encoding(const T &val) const {
    auto it = m_to_id.find(val);
    if (it == m_to_id.end())
        throw std::runtime_error("encoded item must exist");
    return it->second;
}

template <typename T>
const T &IdEncoder<T>::decode(int64_t id) const {
    // must contain
    if (!contains(id)) {
        throw std::runtime_error("must contain");
    }
    return m_to_data[id];
}

template <typename T>
bool IdEncoder<T>::contains(const T &val) const {
    auto it = m_to_id.find(val);
    return it != m_to_id.end();
}

template <typename T>
bool IdEncoder<T>::contains(int64_t id) const {
    return id < size();
}

template <typename T>
size_t IdEncoder<T>::size() const {
    assert(m_to_id.size() == m_to_data.size());
    return m_to_id.size();
}
