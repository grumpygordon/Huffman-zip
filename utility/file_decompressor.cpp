#include "../lib/compress_lib.h"
#include "utility.h"

std::string bits(char t) {
    std::string answ = "";
    for (size_t i = 0; i < 8; i++) {
        answ += (char) ('0' + (t & 1));
        t >>= 1;
    }
    std::reverse(answ.begin(), answ.end());
    return answ;
}

void decompress(char *input, char *output) {
    uint32_t cnt[256];

    std::ifstream fin(input);

    if (fin.fail()) {
        throw std::runtime_error("Something went wrong with input file.\n");
    }

    // fast reading of cnt array
    unsigned char buf[1024];

    fin.read((char *) buf, sizeof(buf));

    if (fin.gcount() != 1024) {
        throw std::runtime_error("Input file was damaged. Not enough data to restore cnt array");
    }
    // end of fast reading

    char ost;
    fin >> ost;

    std::ofstream fout(output);

    if (fout.fail()) {
        throw std::runtime_error("Something went wrong with output file.\n");
    }

    for (size_t i = 0; i < 1024; i += 4) {
        cnt[i >> 2] = buf[i + 3] | (buf[i + 2] << 8) | (buf[i + 1] << 16) | (buf[i] << 24);
    }

    std::vector<std::string> t = get_codes(cnt);

    std::map<std::string, std::pair<bool, char>> ht;

    for (size_t i = 0; i < t.size(); i++) {
        ht[t[i]] = {true, i};
    }

    char c;
    std::string cur_buf = "";
    while (fin.get(c)) {
        cur_buf += bits(c);
        while (cur_buf.size() > (size_t) ost) {
            std::string tmp = "";
            size_t p = 0;
            bool ok = false;
            for (size_t i = 0; i < cur_buf.size(); i++) {
                tmp += cur_buf[i];
                if (ht[tmp].first) {
                    ok = true;
                    p = i + 1;
                    break;
                }
            }

            if (!ok) {
                break;
            }

            fout << ht[tmp].second;
            tmp = "";
            for (; p < cur_buf.size(); p++) {
                tmp += cur_buf[p];
            }
            cur_buf = tmp;
        }
        if (cur_buf.size() > 512) {
            throw std::runtime_error("Compressed file was damaged.\n");
        }
    }

    while (cur_buf.size() > (size_t) ost) {
        std::string tmp = "";
        size_t p = 0;
        bool ok = false;
        for (size_t i = 0; i < cur_buf.size(); i++) {
            tmp += cur_buf[i];
            if (ht[tmp].first) {
                ok = true;
                p = i + 1;
                break;
            }
        }

        if (!ok) {
            break;
        }
        fout << ht[tmp].second;
        tmp = "";
        for (; p < cur_buf.size(); p++) {
            tmp += cur_buf[p];
        }
        cur_buf = tmp;
    }

    if (cur_buf.size() != (size_t) ost) {
        throw std::runtime_error("Compressed file was damaged.\n");
    }
}