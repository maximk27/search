#include "page.h"
#include <sstream>
#include <utility>
#include <boost/regex.hpp>
#include <spdlog/spdlog.h>

using namespace pugi;
using namespace boost;
// ---------------------------------- helpers ----------------------------------

// according to xml links in notes
// return [page_name, page_text]
std::pair<std::string, std::string> parse_page_node(const xml_node &node) {
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

    std::string page_text = text.as_string();
    return std::make_pair(page_name, page_text);
}

// returns out links
// according to xml links in notes
std::vector<int64_t> parse_page_text(IdEncoder<std::string> &encoder,
                                     const std::string text) {
    // catch all of form [[(link)|(text)]]
    // where |(text) is optional
    static const regex re_links(R"(\[\[([^\[\]\|]+)\|?[^\[\]\|]*\]\])");

    // matches namespaced links
    static const regex re_namespace(R"([a-zA-Z]+\:)");

    // for text that matches expression
    sregex_iterator it(text.begin(), text.end(), re_links);
    sregex_iterator end;
    for (; it != end; it++) {
        const boost::smatch &match = *it;
        auto url_match = match[1];

        // is a namespace link
        if (regex_search(url_match.begin(), url_match.end(), re_namespace)) {
            spdlog::warn("skip s={}", url_match.str());
            continue;
        }

        // cut off the #section portion
        auto tag_it = std::find(url_match.begin(), url_match.end(), '#');

        // if tag_it == end, then we take full
        // else we take up to excluding tag_it
        std::string url_name(url_match.begin(),
                             std::min(tag_it, url_match.end()));

        spdlog::info("url_name={}", url_name);
    }

    std::vector<int64_t> res;
    return res;
}

// ---------------------------------- public ----------------------------------

Page::Page(IdEncoder<std::string> &encoder, const xml_node &node) {
    auto [page_name, page_text] = parse_page_node(node);

    // get page id
    page_id = encoder.get_id(page_name);

    // parse page text for outgoing links
    out_links = parse_page_text(encoder, page_text);
}
