#pragma once
#include "../api/token.h"
#include <string>
// -------------------------------- token type --------------------------------
// Wiki Token types
enum class WikiTokenType {
    None = 0, // impl detail, not recognized token
    Text,     // <space>text<space>
    Section,  // ==text==
    Bold,     // '''text'''
    Italic,   // ''text''
    Tag,      // <name>text</name>
    Link,     // [[text]]
    Template, // {{text}}
};
// link and template do not allow
// inside nesting

// to name
inline std::string to_string(WikiTokenType type) {
    static constexpr std::string_view names[8] = {
        "None", "Text", "Section", "Bold", "Italic", "Tag", "Link", "Template"};
    return std::string(names[int(type)]);
}

// -------------------------------- wiki token --------------------------------
// Token returned from wiki
class WikiToken : public Token {
public:
    // for non text
    WikiToken(WikiTokenType _type, std::string_view _open_delim,
              std::string_view _close_delim)
        : type(_type), open_delim(_open_delim), close_delim(_close_delim),
          content(open_delim.end(), close_delim.begin()) {
    }

    // for text
    WikiToken(std::string_view _content)
        : type(WikiTokenType::Text), content(_content) {
    }

    std::string_view text() const override {
        return content;
    }

public:
    WikiTokenType type;
    std::string_view open_delim;
    std::string_view close_delim;
    std::string_view content;
};
