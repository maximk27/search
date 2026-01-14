#include <spdlog/spdlog.h>
#include "src/docs.h"
#include "src/graph.h"
#include "src/xml_docs.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: usage ./cli [path] [count]");
        return 1;
    }

    spdlog::set_level(spdlog::level::debug);

    std::string path = argv[1];
    size_t count = std::stoi(argv[2]);
    IdEncoder<std::string> encoder;
    auto docs = XMLDocs::create(path, count, encoder);

    if (!docs) {
        exit(1);
    }

    Graph graph(*docs, encoder);
    spdlog::info("graph avg_deg={}", graph.avg_outdeg());
}
