#pragma once

#include <pugixml.hpp>
#include <utility>
#include <string>
#include <cstdint>
#include "id_encoder.h"

// according to xml links in notes
// return [page_name, page_text] success
// return nullopt failure (we ignore this name)
std::optional<std::pair<std::string, std::string>>
parse_page_node(const pugi::xml_node &node);

// returns out links
// according to xml links in notes
std::vector<int64_t> parse_page_text(IdEncoder<std::string> &encoder,
                                     const std::string &text);
