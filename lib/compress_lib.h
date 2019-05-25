#ifndef HUFLIB_COMPRESS_LIB_H
#define HUFLIB_COMPRESS_LIB_H

#include <cstdint>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <bitset>
#include <algorithm>
#include <vector>
#include <string>

std::vector<std::string> get_codes(uint32_t *cnt);

#endif //HUFLIB_COMPRESS_LIB_H