#pragma once

#include "../api/filter.h"
#include <fstream>
#include <string>
#include <string_view>

class StopwordFilter : public Filter {
public:
    static std::optional<StopwordFilter> create(const std::string &path);

    void apply(const Token &token, std::function<void(const Token &)> fn);

private:
    // NOTE: expect format
    // word1 (whitespace) word2
    StopwordFilter(std::ifstream &ifs);

private:
};
