#include "wiki_parser.h"
#include <spdlog/spdlog.h>
#include <boost/regex.hpp>

using namespace boost;

bool is_namespaced(const std::string_view &name) {
    bool seen_letter = false;
    for (char c : name) {
        if (isalpha(c)) {
            seen_letter = true;
        } else if (c == ':') {
            return seen_letter;
        } else {
            break;
        }
    }
    return false;
}

void control_name(std::string &input) {
    for (char &c : input) {
        if (isalpha(c) && isupper(c)) {
            c = tolower(c);
        } else if (c == ' ') {
            c = '_';
        }
    }
}
