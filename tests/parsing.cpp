#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "../src/parser.h"

TEST(parsing, basic) {
    std::string path = "../data_test/basic.xml";
    parse(path);
}
