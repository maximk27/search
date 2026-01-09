#include "parser.h"
#include <pugixml.hpp>
#include <spdlog/spdlog.h>
#include <utility>

using namespace pugi;

// [url_name, text]
std::pair<std::string, std::string> parse_page(const xml_node &page) {
    // care only about first revision
    xml_node first_revision = page.child("revision");

    // care only about title, text, optional<redirect>
    xml_text title = page.child("title").text();
    xml_attribute redirect = page.child("redirect").attribute("title");
    xml_text text = first_revision.child("text").text();

    // convert to str
    std::string title_s = title.as_string();
    std::string text_s = text.as_string();
    std::string redirect_s = redirect.as_string(); // maybe empty

    // redirect is real url name if exists
    // else url name is title
    std::string url_name = !redirect_s.empty() ? redirect_s : title_s;

    return {url_name, text_s};
}

void parse(const std::string &path) {
    // reading data
    xml_document input;
    xml_parse_result res = input.load_file(path.data());
    spdlog::info("description: {}", res.description());

    // output data
    xml_document output;

    xml_node mediawiki_in = input.child("mediawiki");

    // mirror input
    output.append_child(mediawiki_in.name());
    xml_node mediawiki_out = output.child("mediawiki");

    size_t threshold = 10;
    size_t count = 0;

    // iterate over all pages
    for (xml_node page : mediawiki_in.children("page")) {
        // log scale
        if (count == threshold) {
            // write this to file
            threshold *= 10;
        }
    }

    // write the full data set to file as well
}
