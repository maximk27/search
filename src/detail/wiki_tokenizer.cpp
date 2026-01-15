#include "wiki_tokenizer.h"
#include "wiki_token.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <stack>

// ----------------------------- helper tokenizer -----------------------------
namespace {

class Delimiter {
public:
    WikiTokenType type;
    std::optional<bool> is_closing; // nullopt if undirectional
    std::string_view delim;
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
    std::optional<bool> is_closing;

    switch (*it) {
        case '=':
            // section
            it = advance_while(view, '=', SIZE_MAX);

            // enough
            if (it - start >= 2) {
                type = WikiTokenType::Section;
            }
            break;
        case '\n':
        case '\t':
        case ' ':
            // whitespace
            it++;
            type = WikiTokenType::Text;
            break;
        case '\'': {
            // italic or bold
            it = advance_while(view, '\'', 3);

            size_t dist = it - start;
            assert(dist <= 3);

            // TODO: empty '''' will be break this
            // so will '''''hi''' wassup'' where outer is italic, inner is bold

            // distinguish italic vs bold
            if (dist == 2) {
                type = WikiTokenType::Italic;
            } else if (dist == 3) {
                type = WikiTokenType::Bold;
            }
            break;
        }
        case '<': {
            // <tag> or </tag>
            static constexpr std::string_view close_pre = "</";

            // check next2
            std::string_view next{it,
                                  std::min(it + close_pre.size(), view.end())};
            is_closing = next == close_pre;

            // until closing this delimiter
            while (it != view.end() && *it != '>')
                it++;

            // shouldnt end because of this
            assert(it != view.end());

            // include the >
            it++;

            type = WikiTokenType::Tag;
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
            // advance it
            it++;
            break;
    }
    return Delimiter{type, is_closing, std::string_view{start, it}};
}

// returns whether this delimiter is closing
//  (useful for undirectional delims like ==)
bool should_close(const Delimiter &delim,
                  const std::stack<Delimiter> &open_delims) {

    // if directional, trust it
    if (delim.is_closing.has_value()) {
        return *delim.is_closing;
    }

    // else close if there is an open of the type
    //  we assume non overlapping to make this check possible
    return !open_delims.empty() && open_delims.top().type == delim.type;
}

} // namespace

// ----------------------------- public tokenizer -----------------------------

void WikiTokenizer::tokenize(std::string_view text,
                             std::function<void(const Token &token)> fn) {

    // open tokens (assuming no overlap)
    std::stack<Delimiter> open_delims;

    auto word_begin = text.begin();

    auto it = text.begin();
    while (it != text.end()) {
        std::string_view view(it, text.end());
        spdlog::debug("text=[{}]", view);
        Delimiter delim = check_delimiter(view);

        if (delim.type == WikiTokenType::Text) {
            if (it != word_begin) {
                // if not empty

                // just collect word
                WikiToken token{std::string_view(word_begin, it)};

                // checkout this token
                fn(token);
            }
            // next word starts outside
            word_begin = delim.delim.end();
        } else if (delim.type != WikiTokenType::None) {
            // still process
            if (should_close(delim, open_delims)) {
                // is closing, ready to emit
                assert(!open_delims.empty());

                Delimiter open = open_delims.top();
                open_delims.pop();

                // better be same type
                assert(open.type == delim.type);

                // create token
                WikiToken token{open.type, open.delim, delim.delim};

                // checkout this token
                fn(token);
            } else {
                // is opening
                open_delims.push(delim);
            }

            // next word starts outside
            word_begin = delim.delim.end();
        }

        // advance to end of tag
        it = delim.delim.end();
    }

    // should be closed
    assert(open_delims.empty());
}
