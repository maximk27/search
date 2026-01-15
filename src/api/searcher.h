#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

// for ranking docs
class Searcher {
public:
    // returns ranked list size <= k most relevant to query s
    virtual std::vector<int64_t> query(std::string_view query, int64_t k) = 0;
};
