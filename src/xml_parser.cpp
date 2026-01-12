#include "xml_parser.h"

#include "gtest/gtest.h"
#include <boost/regex.hpp>
#include <cassert>
#include <string_view>
using namespace pugi;
using namespace boost;

// ---------------------------------- helper ----------------------------------
namespace {
// variables

// modifies input to control name
// <space> -> "_"
// uppercase -> lowercase
void control_name(std::string &input) {
    // experiment
    // TODO: solve this issue
    if (!(!input.empty() && input[0] != ' ' && input.back() != ' ')) {
        spdlog::debug("input is={}", input);
    }

    for (char &c : input) {
        if (isalpha(c) && isupper(c)) {
            c = tolower(c);
        } else if (c == ' ') {
            c = '_';
        }
    }
}

// returns whether this name is namespaced or not
// [a-zA-Z]+\: <- matching this
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

} // namespace

// ---------------------------------- public ----------------------------------
std::optional<std::pair<std::string, std::string>>
parse_page_node(const xml_node &node) {
    xml_node first_revision = node.child("revision");

    // care only about title, text, optional<redirect>
    xml_text title = node.child("title").text();
    xml_attribute redirect = node.child("redirect").attribute("title");
    xml_text text = first_revision.child("text").text();

    // convert to str
    std::string title_s = title.as_string();
    std::string redirect_s = redirect.as_string(); // maybe empty

    // for purposes of links:
    // redirect is real page name if exists
    // else page name is title
    std::string page_name = !redirect_s.empty() ? redirect_s : title_s;

    // is namespaced, so skip
    if (is_namespaced(page_name)) {
        spdlog::debug("skiping page=[{}]", page_name);
        return std::nullopt;
    }

    // modify page name: control name
    control_name(page_name);

    std::string page_text = text.as_string();
    return std::make_pair(page_name, page_text);
}

std::vector<int64_t> parse_page_text(IdEncoder<std::string> &encoder,
                                     const std::string &text) {
    // catch all of form [[(link)|(text)]]
    // where |(text) is optional
    static const regex re_links(R"(\[\[([^\[\]\|]+)\|?[^\[\]\|]*\]\])");

    // ids of pages for out links
    std::vector<int64_t> out_links;

    // for text that matches expression
    sregex_iterator it(text.begin(), text.end(), re_links);
    sregex_iterator end;
    for (; it != end; it++) {
        const boost::smatch &match = *it;
        auto url_match = match[1];

        // skip a namespace links
        if (is_namespaced(
                std::string_view(url_match.begin(), url_match.end()))) {
            spdlog::debug("skip namespace s={}", url_match.str());
            continue;
        }

        // cut off the #section portion
        auto tag_it = std::find(url_match.begin(), url_match.end(), '#');

        // if not found, still matches full
        std::string url_name(url_match.begin(), tag_it);

        // modify url_name: control name
        control_name(url_name);

        auto link_id = encoder.get_id(url_name);
        // not known link
        if (!link_id) {
            spdlog::debug("skip not known s=[{}]", url_name);
            continue;
        }

        spdlog::info("keeping_name={}", url_name);
        out_links.push_back(*link_id);
    }

    return out_links;
}
