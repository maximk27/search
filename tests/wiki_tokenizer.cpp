#include <cstddef>
#include <gtest/gtest.h>
#include <iostream>
#include <print>
#include <string>
#include "../src/id_encoder.h"
#include "../src/detail/xml_docs.h"
#include "../src/detail/wiki_tokenizer.h"
#include "../src/detail/wiki_token.h"

void test(std::string path) {
    size_t count = 1000;

    IdEncoder<std::string> doc_encoder;

    IdEncoder<std::string> encoder;
    auto docs = XMLDocs::create(path, count, doc_encoder);

    if (!docs) {
        exit(1);
    }

    WikiTokenizer tokenizer;
    tokenizer.tokenize(docs->text(0), [](const Token &tok) {
        const WikiToken &token = static_cast<const WikiToken &>(tok);
        std::print("type=[{}], content=[{}]", to_string(token.type),
                   token.text());
    });
}

TEST(tokenizer, basic) {
    spdlog::set_level(spdlog::level::debug);
    std::string path = "../data_test/tokenizer_basic.xml";
    test(path);
}

TEST(tokenizer, nested) {
    spdlog::set_level(spdlog::level::debug);
    std::string path = "../data_test/tokenizer_nested.xml";
    test(path);
}
