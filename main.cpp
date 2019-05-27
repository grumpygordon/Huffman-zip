#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include "compress_testing.h"
#include "utility/utility.h"

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        std::cout << "Сonsole utility for compressing/decopressing files using Huffman coding\n";
        std::cout << "Usage: <mode> <input file> <output file>\n";
        std::cout << "Modes:\n";
        std::cout << "-c    Compress file\n";
        std::cout << "-d    Decompress file\n";
        return 0;
    } else if (argc == 2 && strcmp(argv[1], "-test") == 0) {
        test();
        return 0;
    } else if (argc != 4) {
        std::cout << "Incorrect number of arguments.\n--help for more information.\n";
        return 0;
    }
    char mode = 0;

    if (strcmp(argv[1], "-c") == 0) {
        mode = 1;
    } else if (strcmp(argv[1], "-d") == 0) {
        mode = 2;
    } else {
        std::cout << "Incorrect mode.\n--help for more information.\n";
        return 0;
    }

    auto start_time = clock();

    try {
        if (mode == 1) {
            encode(argv[2], argv[3]);
        } else {
            decode(argv[2], argv[3]);
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what();
        return 0;
    }

    std::cout << "Finished in " << std::fixed << (double) (clock() - start_time) / CLOCKS_PER_SEC << " seconds.\n";
    return 0;
}
