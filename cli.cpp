#include <spdlog/spdlog.h>
#include "src/graph.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: usage ./cli [path] [count]");
        return 1;
    }

    spdlog::set_level(spdlog::level::debug);

    std::string path = argv[1];
    size_t count = std::stoi(argv[2]);
    auto graph = Graph::create(path, count);

    if (!graph) {
        exit(1);
    }

    spdlog::info("graph avg_deg={}", graph->avg_outdeg());
}
