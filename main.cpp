#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include "testing/testing.h"
#include "utility/decoder.h"
#include "utility/encoder.h"
int main(int argc, char *argv[]) {
	bool mode = false, big = false;
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        std::cout << "Сonsole utility for compressing/decompressing files using Huffman coding\n";
        std::cout << "Usage: <mode> [-big] <input file> <output file>\n";
        std::cout << "Modes:\n";
        std::cout << "-c      :  Compress file\n";
        std::cout << "-d -big :  Decompress with precalculations\n";
        std::cout << "-d      :  Decompress file(with precalc if number of characters in output is at least 2*10^8)\n";
        std::cout << "-test   :  for testing on custom tests(without file names)\n";
        return 0;
    } else if (argc == 2 && strcmp(argv[1], "-test") == 0) {
        test();
        return 0;
    } else if (argc == 4) {
		if (strcmp(argv[1], "-c") == 0) {
			mode = false;
		} else if (strcmp(argv[1], "-d") == 0) {
			mode = true;
		} else {
			std::cout << "Incorrect mode.\n--help for more information.\n";
			return 0;
		}
    } else if (argc == 5 && strcmp(argv[1], "-d") == 0 && strcmp(argv[2], "-big") == 0) {
		mode = true;
		big = true;
	} else {
        std::cout << "Wrong usage.\n--help for more information.\n";
        return 0;
	}
    double start_time = clock();

    try {
		if (mode) {
			Decoder hf(argv[2 + size_t(big)], argv[3 + size_t(big)], big);
        	hf.decode();
        } else {
			Encoder hf(argv[2], argv[3]);
			hf.encode();
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what();
        return 0;
    }

    std::cout << "Finished in " << std::fixed << (double) (clock() - start_time) / CLOCKS_PER_SEC << " seconds.\n";
    return 0;
}
