#include "trie_counter.h"
#include <stack>
#include <sstream>
#include <string>

// ---------------------------------- public ----------------------------------
void TrieCounter::insert(std::string_view s) {
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

void TrieCounter::erase_one(std::string_view s) {
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
    if (node->freq == 0)
        m_size--;

    // this alive or next alivee
    if (node->freq != 0 || node->exist_next())
        return;

    // otherwise we can prune this branch

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

        // if still any next, we stop pruning
        if (top->exist_next())
            break;

        it++;
    }
}

void TrieCounter::erase_all([[maybe_unused]] std::string_view s) {
    // TODO:
    throw std::runtime_error("implement");
}

int TrieCounter::freq(std::string_view s) {
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

size_t TrieCounter::size() const {
    return m_size;
}

size_t TrieCounter::count() const {
    return m_count;
}

std::string TrieCounter::traverse() const {
    const Trie *node = &m_root;
    std::ostringstream os;
    std::string curr;
    traverse_help(node, os, curr);
    return os.str();
}

// ---------------------------------- private ----------------------------------
void TrieCounter::traverse_help(const Trie *node, std::ostringstream &os,
                                std::string &s) const {
    if (!node)
        return;

    os << ";;   " << s << "\n";

    if (node->freq > 0) {
        os << "word=[" << s << "], freq=" << node->freq << "\n";
    }

    for (uint8_t i = 0; i < node->nexts.size(); i++) {
        s += static_cast<unsigned char>(i);
        traverse_help(node->nexts[i].get(), os, s);
        s.pop_back();
    }
}
