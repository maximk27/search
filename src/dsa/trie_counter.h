#pragma once

#include <cassert>
#include <cstdint>
#include <array>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <algorithm>

// WARNING: depends on lifetime in traversal, using raw ptr
class TrieCounter {
public:
    TrieCounter() = default;

    template <typename Iterator>
    TrieCounter(Iterator begin, Iterator end) {
        for (auto it = begin; it != end; it++) {
            std::string_view s = *it;
            insert(s);
        }
    }

    // insert s into set
    void insert(std::string_view s);

    // erase one
    void erase_one(std::string_view s);

    // erase all
    void erase_all(std::string_view s);

    // freq of this item
    int freq(std::string_view s);

    // total unique
    size_t size() const;

    // count including duplicate
    size_t count() const;

    // for debugging
    std::string traverse() const;

private:
    struct Trie {
        bool exist_next() const {
            return std::any_of(
                nexts.begin(), nexts.end(),
                [](const auto &curr) { return curr != nullptr; });
        }

        int freq = 0;
        std::array<std::unique_ptr<Trie>, UINT8_MAX> nexts;
    };

    void traverse_help(const Trie *node, std::ostringstream &os,
                       std::string &curr) const;

private:
    Trie m_root;
    size_t m_size = 0;
    size_t m_count = 0;
};
