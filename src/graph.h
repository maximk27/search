#pragma once

#include <string>
#include <optional>
#include <cstdint>
#include <vector>
#include <pugixml.hpp>

#include "api/docs.h"
#include "id_encoder.h"

class Graph {
public:
    Graph(Docs &docs, IdEncoder<std::string> &encoder);
    double avg_outdeg() const;

public:
    std::vector<std::vector<int64_t>> adjlist;
};
