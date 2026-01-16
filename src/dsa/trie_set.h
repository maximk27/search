#pragma once

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
        node->count++;
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

        // st[i] is prev before next into s[i]
        assert(st.size() == s.size());

        // process nodes from back first
        auto it = s.rbegin();
        while (!st.empty()) {
            assert(it != s.rend());
            Trie *top = st.top();
            st.pop();

            uint8_t c = *it;

            // decrement
            std::unique_ptr<Trie> &next = top->nexts[c];
            next->count--;

            // erase
            if (next->count == 0) {
                next.reset();
            }

            it++;
        }
    }

    void erase_all(std::string_view s) {
    }

    int count(std::string_view s) {
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
        return node->count;
    }

    // total unique
    size_t size() const {
    }

    // sum count including duplicate
    size_t sum() const {
    }

private:
    struct Trie {
        int count = 0;
        std::array<std::unique_ptr<Trie>, UINT8_MAX> nexts;
    };

private:
    Trie m_root;
};
