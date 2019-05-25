#include "../lib/compress_lib.h"
#include "utility.h"

void compress(char *input, char *output) {
    uint32_t cnt[256] = {};

    std::ifstream fin(input);
    std::ofstream fout(output);

    if (fin.fail() || fout.fail()) {
        throw std::runtime_error("Something went wrong with input/output file.\n");
    }

    // fast counting
    unsigned char buf[1024];
    uint32_t count[8][256] = {};

    while (fin) {
        fin.read((char *) buf, sizeof(buf));
        size_t s = (size_t) fin.gcount() / 8 * 8;
        for (size_t i = 0; i < s;) {
            ++count[0][buf[i++]];
            ++count[1][buf[i++]];
            ++count[2][buf[i++]];
            ++count[3][buf[i++]];
            ++count[4][buf[i++]];
            ++count[5][buf[i++]];
            ++count[6][buf[i++]];
            ++count[7][buf[i++]];
        }
        for (; s < fin.gcount(); s++) {
            cnt[buf[s]]++;
        }
    }

    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 256; j++) {
            cnt[j] += count[i][j];
        }
    }
    // end of fast counting

    // put fin to start of file
    fin.clear();
    fin.seekg(0, std::ios::beg);

    const std::bitset<32> mask = (1 << 8) - 1;

    for (uint i = 0; i < 256; i++) {
        std::bitset<32> tmp = cnt[i];
        char a, b, c, d;
        d = (char) ((tmp & mask).to_ulong());
        c = (char) ((tmp & (mask << 8)).to_ulong() >> 8);
        b = (char) ((tmp & (mask << 16)).to_ulong() >> 16);
        a = (char) ((tmp & (mask << 24)).to_ulong() >> 24);
        fout << a << b << c << d;
    }

    std::vector<std::string> t = get_codes(cnt);

    // count last bits
    uint32_t len = 0, ost = 0;
    for (size_t i = 0; i < 256; i++) {
        len += cnt[i] * t[i].size();
    }
    ost = (len % 8 ? 8 - len % 8 : 0);
    fout << (char) ost;
    // end of counting last bits

    std::string cur_buf = "";

    while (fin) {
        fin.read((char *) buf, sizeof(buf));
        for (size_t i = 0; i < (size_t) fin.gcount(); i++) {
            cur_buf += t[buf[i]];
            if (cur_buf.size() >= 8) {
                for (size_t i = 0; i < cur_buf.size() / 8 * 8; i += 8) {
                    char v = 0;
                    for (size_t j = i; j < i + 8; j++) {
                        v = (v << 1) | (cur_buf[j] - '0');
                    }
                    fout << v;
                }

                std::string tmp = "";
                for (size_t i = cur_buf.size() / 8 * 8; i < cur_buf.size(); i++) {
                    tmp += cur_buf[i];
                }
                cur_buf = tmp;
            }
        }
    }
    if (ost) {
        char c;
        for (size_t j = 0; j < 8 - ost; j++) {
            c = (c << 1) | (cur_buf[j] - '0');
        }
        c <<= ost;
        fout << c;
    }
}