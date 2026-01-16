#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <array>
#include <memory>
#include <stack>
#include <string_view>

// WARNING: depends on lifetime in traversal, using raw ptr
class TrieSet {
public:
    template <typename Iterator>
    TrieSet(Iterator begin, Iterator end) {
        for (auto it = begin; it != end; it++) {
            std::string_view s = *it;
            insert(s);
        }
    }

    void insert(std::string_view s) {
        Trie *node = &m_root;
        for (uint8_t c : s) {
            std::unique_ptr<Trie> &next = node->nexts[c];
            // if not exist, make next
            if (!next) {
                next = std::make_unique<Trie>();
            }
            // move next
            node = next.get();
        }
        // track word on char we end on
        node->freq++;

        // adjust counters
        m_count++;
        if (node->freq == 1)
            m_size++;
    }

    void erase_one(std::string_view s) {
        Trie *node = &m_root;

        std::stack<Trie *> st;
        for (uint8_t c : s) {
            // track prev
            st.push(node);

            // move next
            node = node->nexts[c].get();
        }
        node->freq--;

        // adjust counts
        m_count--;

        // done
        if (node->freq > 0)
            return;

        // can prune this branch
        m_size--;

        // st[i] is prev trie that goes next with s[i]
        assert(st.size() == s.size());

        // process nodes from back first
        auto it = s.rbegin();
        while (!st.empty()) {
            Trie *top = st.top();
            st.pop();

            // reset the next
            uint8_t c = *it;
            auto &next = top->nexts[c];
            next.reset();

            // if any still valid, then stop pruning
            if (std::any_of(top->nexts.begin(), top->nexts.end(),
                            [](const auto &node) { return node != nullptr; })) {
                break;
            }

            it++;
        }
    }

    void erase_all(std::string_view s) {
    }

    int freq(std::string_view s) {
        Trie *node = &m_root;
        for (uint8_t c : s) {
            std::unique_ptr<Trie> &next = node->nexts[c];
            // if not exist, then done
            if (!next) {
                return 0;
            }
            // else move next
            node = next.get();
        }
        // checkout word count on char we end on
        return node->freq;
    }

    // total unique
    size_t size() const {
    }

    // count including duplicate
    size_t count() const {
    }

private:
    struct Trie {
        int freq = 0;
        std::array<std::unique_ptr<Trie>, UINT8_MAX> nexts;
    };

private:
    Trie m_root;
    size_t m_size = 0;
    size_t m_count = 0;
};
