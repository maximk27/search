#include "wiki_tokenizer.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

// ----------------------------- helper tokenizer -----------------------------
namespace {

class Delimiter {
public:
    WikiTokenType type;
    bool is_closing;
    std::string_view tag_range;
};

// advance while =val, not end, less than length many
// returns iterator of ending pos
std::string_view::const_iterator advance_while(std::string_view view, char val,
                                               size_t length) {
    auto it = view.begin();
    while (*it == val && it != view.end() &&
           size_t(it - view.begin()) < length) {
        it++;
    }
    return it;
}

// view points begin of where we're iterating from
//  we move forward for the check
// returns token delimiter type
Delimiter check_delimiter(std::string_view view) {
    auto start = view.begin();
    auto it = start;

    WikiTokenType type = WikiTokenType::None;

    // default closing first
    bool is_closing = true;

    switch (*it) {
        case '=':
            // section
            it = advance_while(view, '=', SIZE_MAX);

            // enough
            if (it - start >= 2) {
                type = WikiTokenType::Section;
            }
            break;
        case ' ':
            // word
            it++;
            type = WikiTokenType::Text;
            break;
        case '\'': {
            // italic or bold
            it = advance_while(view, '\'', 3);

            size_t dist = it - start;
            assert(dist <= 3);

            // TODO: empty '''' will be break this

            // distinguish italic vs bold
            if (dist == 2) {
                type = WikiTokenType::Italic;
            } else if (dist == 3) {
                type = WikiTokenType::Bold;
            }
            break;
        }
        case '&': {
            // <tag> or </tag>
            std::string_view tag_start(it, std::min(it + 3, view.end()));

            // neither open nor close
            if (!tag_start.starts_with("&gt")) {
                break;
            }

            // closing if </, else opening <
            is_closing = tag_start == "&gt/";

            while (it < view.end()) {
                std::string_view symbol(it, std::min(it + 3, view.end()));
                if (symbol == "&lt") {
                    // jump to end of delimiter
                    it += 4;
                    break;
                }
            }

            // should not be breaking out for this reason
            assert(it != view.end());
            break;
        }
        case '[':
            // open [
            it = advance_while(view, '[', 2);
            if (it - start == 2) {
                type = WikiTokenType::Link;
                is_closing = false;
            }
            break;
        case '{':
            // open {
            it = advance_while(view, '{', 2);
            if (it - start == 2) {
                type = WikiTokenType::Template;
                is_closing = false;
            }
            break;
        case ']':
            // close ]
            it = advance_while(view, ']', 2);
            if (it - start == 2) {
                type = WikiTokenType::Link;
                is_closing = true;
            }
            break;
        case '}':
            // close }
            it = advance_while(view, '}', 2);
            if (it - start == 2) {
                type = WikiTokenType::Template;
                is_closing = true;
            }
            break;

        default:
            break;
    }
    return Delimiter{type, is_closing, std::string_view{start, it}};
}

} // namespace

// ----------------------------- public tokenizer -----------------------------

void WikiTokenizer::tokenize(std::string_view text,
                             std::function<void(const Token &token)> fn) {

    // open tokens (assuming no overlap)
    std::stack<WikiToken> tokens;

    for (auto it = text.begin(); it != text.end(); it++) {
        std::string_view view(it, text.end());
        Delimiter delim = check_delimiter(view);

        // ignore
        if (delim.type == WikiTokenType::None)
            continue;
    }
}
