#include "gtest/gtest.h"
#include <spdlog/spdlog.h>
#include "../src/dsa/trie_counter.h"

using std::cout;

void test(const std::vector<std::string> &insert,
          const std::vector<std::string> &erase) {
    TrieCounter set;
    for (auto &s : insert) {
        set.insert(s);
    }
    for (auto &s : erase) {
        set.erase_one(s);
    }
    std::string res = set.traverse();
    cout << res;
}

TEST(trie, insert) {
    test({"a", "b", "c", "abc", "abcd", "a"}, {});
}

TEST(trie, erase_no_prune_next) {
    test({"a", "b", "c", "abc", "abcd", "a"}, {"abc", "a", "b"});
}

TEST(trie, erase_no_prune_more) {
    test({"a", "b", "c", "abcd", "abcd", "a"}, {"abcd", "a", "b"});
}

TEST(trie, limited_prune) {
    test({"abcDefg", "abcD0"}, {"abcDefg"});
}
