#include "parser.h"
#include "graph.h"
#include <spdlog/spdlog.h>
#include <utility>

void parse(const std::string &path) {
    spdlog::set_level(spdlog::level::debug);

    auto graph = Graph::create(path, 200);
    if (!graph) {
        exit(1);
    }
    spdlog::info("graph avg_deg={}", graph->avg_outdeg());
}
