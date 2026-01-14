#include "graph.h"
#include "id_encoder.h"
#include <cassert>
#include <sstream>
#include <utility>
#include <spdlog/spdlog.h>
#include <pugixml.hpp>
#include <boost/regex.hpp>
#include "wiki_parser.h"

using namespace boost;
using namespace pugi;

// ---------------------------------- helper ----------------------------------
namespace {

std::vector<int64_t> parse_page_text(IdEncoder<std::string> &encoder,
                                     std::string_view text) {
    // catch all of form [[(link)|(text)]]
    // where |(text) is optional
    static const regex re_links(R"(\[\[([^\[\]\|]+)\|?[^\[\]\|]*\]\])");

    // ids of pages for out links
    std::vector<int64_t> out_links;

    // for text that matches expression
    regex_iterator<const char *> it(text.begin(), text.end(), re_links);
    regex_iterator<const char *> end;
    for (; it != end; it++) {
        const match_results<const char *> &match = *it;
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
}; // namespace

// ---------------------------------- public ----------------------------------
Graph::Graph(Docs &docs, IdEncoder<std::string> &encoder) {
    adjlist.resize(encoder.size());
    for (size_t id = 0; id < adjlist.size(); id++) {
        // parse links in this pages text
        std::vector<int64_t> out_links =
            parse_page_text(encoder, docs.text(id));

        // move to dsa
        adjlist[id] = std::move(out_links);
    }
}

double Graph::avg_outdeg() const {
    long double total = 0;
    for (size_t i = 0; i < adjlist.size(); i++) {
        spdlog::debug("size={}", adjlist[i].size());
        total += adjlist[i].size();
    }
    return total / adjlist.size();
}
