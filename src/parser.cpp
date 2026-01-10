#include "parser.h"
#include "page.h"
#include <pugixml.hpp>
#include <spdlog/spdlog.h>
#include <utility>

using namespace pugi;

void parse(const std::string &path) {
    // reading data
    xml_document input;
    xml_parse_result res = input.load_file(path.data());
    spdlog::info("description: {}", res.description());

    xml_node mediawiki_in = input.child("mediawiki");
    IdEncoder<std::string> id_encoder;

    // iterate over all pages
    for (xml_node node : mediawiki_in.children("page")) {
        Page page(id_encoder, node);
    }

    // write the full data set to file as well
}
