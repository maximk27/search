#pragma once

#include <string>
#include <optional>
#include <cstdint>
#include <vector>
#include <pugixml.hpp>

#include "id_encoder.h"

class Graph {
public:
    // try create graph of wikipedia data using up to first n pages
    // from xml file pointed to by path
    static std::optional<Graph> create(const std::string &path, size_t n);

    double avg_outdeg() const;

private:
    Graph(pugi::xml_document &doc, size_t n);

private:
    // SOA where idx is a pages_id
    std::vector<std::string> m_texts;
    std::vector<std::vector<int64_t>> m_adjlist;

    IdEncoder<std::string> m_ids;
};
