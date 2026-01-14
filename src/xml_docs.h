#pragma once

#include <pugixml.hpp>
#include <utility>
#include <string>
#include <cstdint>
#include "id_encoder.h"
#include "docs.h"

class XMLDocs : public Docs {
public:
    // try create doc of wikipedia data using up to first n pages
    // from xml file pointed to by path
    static std::optional<XMLDocs> create(const std::string &path, size_t n,
                                         IdEncoder<std::string> &encoder);

    // getters
    std::string_view text(int64_t id) override;

    // for each
    void for_each(std::function<bool(int64_t id, std::string_view text)> fn)
        const override;

    size_t size() const override;

private:
    XMLDocs(pugi::xml_document &doc, size_t n, IdEncoder<std::string> &encoder);

private:
    std::vector<std::string> m_texts;
};
