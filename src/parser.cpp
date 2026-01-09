#include "parser.h"
#include <pugixml.hpp>
#include <spdlog/spdlog.h>

using namespace pugi;

void checkout(const std::string &path) {
    xml_document doc;
    xml_parse_result res = doc.load_file(path.data());
    spdlog::info("description: {}", res.description());

    xml_node mediawiki = doc.child("mediawiki");
    for (xml_node page : mediawiki.children("page")) {
        xml_text title = page.child("title").text();
        xml_node first_reivision = page.child("revision");
        xml_text text = first_reivision.child("text").text();
        spdlog::info("title={}, text={}", title.as_string(), text.as_string());
    }
}

void parse(const std::string &path) {
    // reading data
    xml_document input;
    xml_parse_result res = input.load_file(path.data());
    spdlog::info("description: {}", res.description());

    // output data
    xml_document output;

    size_t i = 0;
    xml_node mediawiki = input.child("mediawiki");
    // iterate over all pages
    for (xml_node page : mediawiki.children("page")) {

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
        i++;
    }
}
