#include "stopword_filter.h"
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>

// ---------------------------------- public ----------------------------------
std::optional<StopwordFilter> StopwordFilter::create(std::string_view path) {
    std::ifstream ifs{std::string{path}};
    if (!ifs.is_open()) {
        return std::nullopt;
    }
    return StopwordFilter(ifs);
}

void StopwordFilter::apply(const Token &token,
                           std::function<void(const Token &)> fn) {
    // TODO: make this another filter as well?
    //  need to change the token interface to support altering the text()

    // force tolower for the comparison
    std::string s(token.text());
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return static_cast<char>(tolower(c));
    });

    // check if part of skip list
    if (m_set.freq(s)) {
        spdlog::debug("stopword filter skip s=[{}]", token.text());
        return;
    }

    // otherwise apply
    fn(token);
}
// ---------------------------------- private ----------------------------------
StopwordFilter::StopwordFilter(std::ifstream &ifs) {
    std::string s;
    while (ifs >> s) {
        m_set.insert(s);
    }
}
