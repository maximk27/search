#include "wiki_tokenizer.h"
#include <functional>
#include <string>

void WikiTokenizer::tokenize(std::string_view text,
                             std::function<void(const Token &token)> fn) {
}
