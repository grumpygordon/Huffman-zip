#ifndef HUFFAMN_UTILITY_H
#define HUFFAMN_UTILITY_H

#include <cstdint>
#include <cstddef>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <vector>
#include <map>

void decompress(char *input, char *output);

void compress(char *input, char *output);

#endif //HUFFAMN_UTILITY_H
