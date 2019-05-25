#include "../lib/compress_lib.h"
#include "utility.h"
#include <cassert>

typedef uint32_t uint;
typedef unsigned char uchar;

void compress(char *input, char *output) {
    uint cnt[256] = {};

    std::ifstream fin(input);
    std::ofstream fout(output);

    if (fin.fail() || fout.fail()) {
        throw std::runtime_error("Something went wrong with input/output file.\n");
    }

    char buf[1024], ubuf[1024];
    
    fin.read((char *)buf, sizeof(buf));

	size_t upos = 0, pos = 0, len = fin.gcount();
	
	auto can = [&]() {
		return pos < len;
	};
	
	auto read = [&]() {
		assert(can());
		char w = buf[pos++];
		if (pos == len) {
			fin.read(buf, sizeof(buf));
			len = fin.gcount();
			pos = 0;
		}
		return uchar(w);
	};

	auto clear = [&]() {
		pos = 0;
		fin.clear();
		fin.seekg(0, std::ios::beg);
		fin.read(buf, sizeof(buf));
		len = fin.gcount();
	};
    
	auto print = [&](char w) {
		ubuf[upos++] = w;
		if (upos == 1024) {
			fout.write(ubuf, upos);
			upos = 0;
		}
	};
	
	auto finish = [&]() {
		if (upos > 0)
			fout.write(ubuf, upos);
	};

    while (can()) {
		cnt[read()]++;
	}

	clear();
	
	const uchar big = 255;
	
	const uint flag = uint(big) << 24; 

    for (size_t i = 0; i < 256; i++) {
		uint w = cnt[i];
		assert(w < flag);
        if (w == 0) {
			print(big);
			continue;
		}
		for (signed char j = 3; j >= 0; j--) {
			uchar o = (w >> (8 * j));
			print(o);
			w -= uint(o) << (8 * j);
		}
    }
    std::vector<std::string> t = get_codes(cnt);

    uint lens = 0, ost = 0;
    for (size_t i = 0; i < 256; i++) {
        lens += cnt[i] * t[i].size();
    }
    ost = (lens % 8 ? 8 - lens % 8 : 0);
	print(ost);

    std::string cur_buf = "";

    while (can()) {
        uchar w = read();
		cur_buf += t[w];
		if (cur_buf.size() >= 8) {
			size_t i = 0;
			for (; i + 7 < cur_buf.size(); i += 8) {
				char v = 0;
				for (size_t j = i; j < i + 8; j++) {
					v = (v << 1) ^ (cur_buf[j] & 1);
				}
				print(v);
			}
			cur_buf = cur_buf.substr(i, cur_buf.size() - i);
		}
    }

    if (ost) {
        uchar c;
        for (size_t j = 0; j < 8 - ost; j++) {
            c = (c << 1) ^ (cur_buf[j] & 1);
        }
        c <<= ost;
        print(c);
    }
	finish();
	//std::cerr << "Done compressing\n";
}
