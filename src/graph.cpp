#include "graph.h"
#include "id_encoder.h"
#include <cassert>
#include <sstream>
#include <utility>
#include <boost/regex.hpp>
#include <spdlog/spdlog.h>
#include <pugixml.hpp>

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
                                     const std::string &text) {
    // catch all of form [[(link)|(text)]]
    // where |(text) is optional
    static const regex re_links(R"(\[\[([^\[\]\|]+)\|?[^\[\]\|]*\]\])");

    // matches namespaced links
    static const regex re_namespace(R"([a-zA-Z]+\:)");

    // ids of pages for out links
    std::vector<int64_t> out_links;

    // for text that matches expression
    sregex_iterator it(text.begin(), text.end(), re_links);
    sregex_iterator end;
    for (; it != end; it++) {
        const boost::smatch &match = *it;
        auto url_match = match[1];

        // skip a namespace links
        if (regex_search(url_match.begin(), url_match.end(), re_namespace)) {
            spdlog::debug("skip namespace s={}", url_match.str());
            continue;
        }

        // cut off the #section portion
        auto tag_it = std::find(url_match.begin(), url_match.end(), '#');

        // if tag_it == end, then we take full
        // else we take up to excluding tag_it
        std::string url_name(url_match.begin(),
                             std::min(tag_it, url_match.end()));

        auto link_id = encoder.get_id(url_name);
        // not known link
        if (!link_id) {
            spdlog::debug("skip not known s={}", url_match.str());
            continue;
        }

        spdlog::info("keeping_name={}", url_name);
        out_links.push_back(*link_id);
    }

    return out_links;
}

// ---------------------------------- public ----------------------------------
std::optional<Graph> Graph::create(const std::string &path, size_t n) {
    xml_document input;
    xml_parse_result res = input.load_file(path.data());
    if (!res) {
        spdlog::error("Error reading file: {}", res.description());
        return std::nullopt;
    }
    return Graph(input, n);
}

double Graph::avg_outdeg() const {
    long double total = 0;
    for (size_t i = 0; i < m_adjlist.size(); i++) {
        spdlog::debug("size={}", m_adjlist[i].size());
        total += m_adjlist[i].size();
    }
    return total / m_adjlist.size();
}

// ---------------------------------- private ----------------------------------
Graph::Graph(pugi::xml_document &doc, size_t n) {
    xml_node first = doc.child("mediawiki");

    m_texts.reserve(n);

    size_t count = 0;
    for (xml_node node : first.children("page")) {
        // hit max
        if (count == n)
            break;

        auto [page_name, page_text] = parse_page_node(node);
        spdlog::info("known name={}", page_name);

        // register this page on this idx
        m_ids.get_id(page_name);

        // add this entries data
        m_texts.emplace_back(std::move(page_text));

        // track count
        count++;
    }

    m_adjlist.reserve(n);
    for (size_t page = 0; page < count; page++) {
        // parse links in this pages text
        std::vector<int64_t> out_links = parse_page_text(m_ids, m_texts[page]);

        // push to dsa
        m_adjlist.emplace_back(std::move(out_links));
    }
}
