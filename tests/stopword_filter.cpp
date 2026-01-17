#include <cassert>
#include <gtest/gtest.h>
#include "../src/detail/stopword_filter.h"
#include "../src/detail/basic_token.h"
#include <iostream>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

using std::cout;

TEST(stopword_filter, basic) {
    spdlog::set_level(spdlog::level::debug);

    std::string_view path = "../resources/stopwords.txt";
    auto filter = StopwordFilter::create(path);
    assert(filter);

    auto words = {"a", "ABOUT", "AFTER", "back", "because", "maxim", "instant"};
    for (auto &s : words) {
        BasicToken tok{s};
        filter->apply(tok, [](const Token &token) {
            cout << ";; process=[" << token.text() << "]\n";
        });
    }
}
