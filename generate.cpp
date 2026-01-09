#include "src/parser.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: usage ./generate [path]");
        return 1;
    }
    std::string path = argv[1];
    parse(path);
}
