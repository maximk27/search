#include "xml_docs.h"
#include "gtest/gtest.h"
#include <boost/regex.hpp>
#include <cassert>
#include <string_view>
#include "../wiki_parser.h"

using namespace pugi;
using namespace boost;

// ---------------------------------- helper ----------------------------------
namespace {

// according to xml links in notes
// return [page_name, page_text] success
// return nullopt failure (we ignore this name)
std::optional<std::pair<std::string, std::string>>
parse_page_node(const xml_node &node) {
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

    // is namespaced, so skip
    if (is_namespaced(page_name)) {
        spdlog::debug("skiping page=[{}]", page_name);
        return std::nullopt;
    }

    // modify page name: control name
    control_name(page_name);

    std::string page_text = text.as_string();
    return std::make_pair(page_name, page_text);
}

} // namespace

// ---------------------------------- public ----------------------------------

std::optional<XMLDocs> XMLDocs::create(const std::string &path, size_t n,
                                       IdEncoder<std::string> &encoder) {
    xml_document input;
    xml_parse_result res = input.load_file(path.data());
    if (!res) {
        return std::nullopt;
    }

    return XMLDocs(input, n, encoder);
}

std::string_view XMLDocs::text(int64_t id) {
    assert(id < int64_t(m_texts.size()));
    return m_texts[id];
}

// for each
void XMLDocs::for_each(
    std::function<bool(int64_t id, std::string_view text)> fn) const {
    for (size_t id = 0; id < m_texts.size(); id++) {
        if (fn(int64_t(id), m_texts[id]))
            break;
    }
}
size_t XMLDocs::size() const {
    return m_texts.size();
}

// ---------------------------------- private ----------------------------------
XMLDocs::XMLDocs(pugi::xml_document &doc, size_t n,
                 IdEncoder<std::string> &encoder) {
    xml_node first = doc.child("mediawiki");

    m_texts.reserve(n);

    int64_t count = 0;
    for (xml_node node : first.children("page")) {
        // hit max
        if (count == int64_t(n))
            break;

        // get string information about page node
        auto it = parse_page_node(node);

        // this one is being skipped
        if (!it)
            continue;

        // we're using this
        auto [page_name, page_text] = *it;
        spdlog::info("keeping page=[{}]", page_name);

        // already exists, so skip
        if (encoder.contains(page_name))
            continue;

        // register this page on this idx
        int64_t id = encoder.encode(page_name);

        // should be dense for xml_docs
        assert(id == count);

        // add this entries data
        m_texts.emplace_back(std::move(page_text));

        // track count
        count++;
    }
}
