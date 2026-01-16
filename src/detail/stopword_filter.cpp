#include "stopword_filter.h"

// ---------------------------------- public ----------------------------------
std::optional<StopwordFilter> StopwordFilter::create(const std::string &path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        return std::nullopt;
    }
    return StopwordFilter(ifs);
}

void StopwordFilter::apply(const Token &token,
                           std::function<void(const Token &)> fn) {
}
// ---------------------------------- private ----------------------------------
StopwordFilter::StopwordFilter(std::ifstream &ifs) {
    std::string s;
    while (ifs >> s) {
    }
}
