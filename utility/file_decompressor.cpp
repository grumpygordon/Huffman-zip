#include "../lib/compress_lib.h"
#include "utility.h"
#include <cassert>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <cstring>

typedef unsigned char uchar;

typedef uint32_t uint;

std::string bits(char t) {
    std::string answ = "";
    for (size_t i = 0; i < 8; i++) {
        answ += (char) (48 + (t & 1));
        t >>= 1;
    }
    std::reverse(answ.begin(), answ.end());
    return answ;
}

void decompress(char *input, char *output) {
    uint cnt[256] = {};

    std::ifstream fin(input);
    std::ofstream fout(output);

    if (fin.fail()) {
        throw std::runtime_error("Couldn't open input file.\n");
    }
    if (fout.fail()) {
        throw std::runtime_error("Couldn't open output file.\n");
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
	
	for (size_t i = 0; i < 256; i++) {
		uint w = read();
		if (w == 255) {
			cnt[i] = 0;
		} else {
			for (char j = 1; j < 4; j++) {
				w = (w << 8) + read();
			}
			cnt[i] = w;
		}
	}

    char ost;
    ost = read();

    std::vector<std::string> t = get_codes(cnt);

	int32_t e[512][2], sz = 1;

	memset(e, 0, sizeof(e));
	for (size_t it = 0; it < t.size(); it++) {
		size_t w = 0;
		for (char c : t[it]) {
			c -= 48;
			if (e[w][c] == 0)
				e[w][c] = sz++;
			w = e[w][c];
		}
		e[w][0] = e[w][1] = -1 - it;
	}

    char c;
	size_t v = 0;
	std::queue<char> g;
    while (can()) {
		c = read();
		for (char o : bits(c))
			g.push(o);
		while (g.size() > ost) {
			size_t o = g.front() - 48;
			g.pop();
			if (e[v][o] < 0) {
				print(uchar(-e[v][o] - 1));
				v = 0;
			}
			if (e[v][o] == 0)
				throw std::runtime_error("Compressed file was damaged.\n");
			v = e[v][o];
		}
    }

	if (e[v][0] < 0)
		print(uchar(-e[v][0] - 1));
	if (ost != g.size())
		throw std::runtime_error("Compressed file was damaged.\n");

	finish();

	//std::cerr << "Done decompressing\n";
}
