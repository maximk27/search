#pragma once

#include "../api/searcher.h"
#include "../id_encoder.h"

class TfIdfSearcher : public Searcher {
public:
    // ctor cal
    TfIdfSearcher(const IdEncoder<std::string> &encoder);

    // query
    std::vector<int64_t> query(std::string_view query, int64_t k) override;

private:
    const IdEncoder<std::string> &m_encoder;
};
