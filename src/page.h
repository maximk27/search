#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <pugixml.hpp>
#include "id_encoder.h"

class Page {
public:
    Page(IdEncoder<std::string> &encoder, const pugi::xml_node &page_node);

public:
    int64_t page_id;
    std::vector<int64_t> out_links;
};
