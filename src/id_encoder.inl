#pragma once
#include "id_encoder.h"

template <typename T>
void IdEncoder<T>::set_id(const T &val) {
    auto it = m_to_id.find(val);

    // already set
    if (it != m_to_id.end()) {
        throw new std::runtime_error("double set");
    }

    // create for first time
    int64_t id = m_to_id.size();

    // both ways
    m_to_data.emplace(id, val);
    m_to_id.emplace(val, id);
}

template <typename T>
std::optional<int64_t> IdEncoder<T>::get_id(const T &val) {
    auto it = m_to_id.find(val);
    // exists, so return it
    if (it != m_to_id.end()) {
        return it->second;
    }
    return std::nullopt;

    // create for first time
    int64_t id = m_to_id.size();

    // both ways
    m_to_data.emplace(id, val);
    m_to_id.emplace(val, id);

    return id;
}

template <typename T>
const T &IdEncoder<T>::get_data(int64_t id) {
    // requires that this entry exists
    return m_to_data.at(id);
}

template <typename T>
int64_t IdEncoder<T>::size() const {
    assert(m_to_id.size() == m_to_data.size());
    return m_to_id.size();
}
