#include "tf_idf_searcher.h"

// ---------------------------------- public ----------------------------------
TfIdfSearcher::TfIdfSearcher(const IdEncoder<std::string> &encoder)
    : m_encoder(encoder) {
}

std::vector<int64_t> TfIdfSearcher::query(std::string_view query, int64_t k) {
}
