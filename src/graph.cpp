#include "graph.h"
#include "id_encoder.h"
#include <cassert>
#include <sstream>
#include <utility>
#include <spdlog/spdlog.h>
#include <pugixml.hpp>
#include "xml_parser.h"

using namespace pugi;

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

        // get string information about page node
        auto it = parse_page_node(node);

        // this one is being skipped
        if (!it)
            continue;

        // we're using this
        auto [page_name, page_text] = *it;
        spdlog::info("keeping page=[{}]", page_name);

        // register this page on this idx
        bool was_set = m_ids.set_id(page_name);

        // already exists, so skip
        if (!was_set)
            continue;

        // add this entries data
        m_texts.emplace_back(std::move(page_text));

        // track count
        count++;
    }

    return;

    m_adjlist.reserve(n);
    for (size_t page = 0; page < count; page++) {
        // parse links in this pages text
        std::vector<int64_t> out_links = parse_page_text(m_ids, m_texts[page]);

        // push to dsa
        m_adjlist.emplace_back(std::move(out_links));
    }
}
