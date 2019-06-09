#include "huffman_decoder.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <cassert>

Huffman_decoder::Huffman_decoder() : Huffman_decoder(1000000) {}

Huffman_decoder::Huffman_decoder(size_t w) : Huffman("decode"), threshold(w), cur_v(0), e(), q(nullptr), rlen(false), big(false) {}

Huffman_decoder::~Huffman_decoder() {
	delete[] q;
}

void Huffman_decoder::code_tree(char const *input, size_t n, char* output, size_t &m) {
	bad_stage(0);
	stage++;
	size_t ipos = 0;
	m = 0;
	auto read = [&]() {
		if (ipos >= n)
			error("not enough data to restore the tree, maybe file was damaged or buffer is too small");
		return (unsigned char)input[ipos++];
	};
	
	for (size_t i = 0; i < 256; i++) {
		uint32_t w = read();
		if (w == 255) {
			cnt[i] = 0;
		} else {
			for (char j = 1; j < 4; j++) {
				w = (w << 8) + read();
			}
			cnt[i] = w;
		}
	}

	unsigned char t[SZ][TSZ] = {};
	
	size_t tlen[SZ] = {};

	make_huffman_codes(t, tlen);
	size_t sum = 0;
	for (size_t i = 0; i < 256; i++) {
		sum += cnt[i];
	}
	size_t sz = 1;

	for (size_t it = 0; it < SZ; it++) {
		size_t w = 0;
		for (size_t i = 0; i < tlen[it]; i++) {
			bool c = !!(t[it][i >> 3] & (1 << ((i & 7) ^ 7)));
			if (e[w][c] == 0)
				e[w][c] = sz++;
			w = e[w][c];
		}
		if (e[w][0] != 0 || e[w][1] != 0)
			error("couldn't make huffman tree");
		e[w][0] = e[w][1] = -1 - it;
	}
	
	if (sum >= threshold) {
		big = true;
		//std::cerr << "I'm big!\n";
		q = new int16_t[2 * SZ * SZ];
		qv.resize(2 * SZ * SZ);
		for (size_t i = 0; i < sz; i++) {
			for (size_t j = 0; j < 256; j++) {
				int16_t v = i;
				if (e[v][0] < 0) {
					qv[i * SZ + j].push_back(-e[v][0] - 1);
					v = 0;
				}
				for (signed char k = 7; k >= 0; k--) {
					bool t = ((j >> k) & 1);
					if (e[v][t] == 0) {
						v = -1;
						break;
					}
					v = e[v][t];
					if (e[v][0] < 0) {
						qv[i * SZ + j].push_back(-e[v][0] - 1);
						v = 0;
					}
				}
				q[i * SZ + j] = v;
			}
		}
	}
    cur_v = 0;

    uint32_t dec_rem = read();
    
    if (dec_rem != grem)
		error("number of useless bits doesn't match");
    
    if (ipos < n)
		code(input + ipos, n - ipos, output, m);
}
size_t keker;

void Huffman_decoder::code(char const *input, size_t n, char* output, size_t &m) {
	bad_stage(1);
	m = 0;
	if (n == 0)
		return;
	auto print = [&](char w) {
		assert(m < OSZ);
		output[m++] = w;
	};

	int16_t &v = cur_v;

	auto go = [&](bool o) {
		if (e[v][o] < 0) {
			unsigned char id = -e[v][o] - 1;
			print(id);
			v = 0;
		}
		if (e[v][o] == 0)
			error("couldn't find vertex in huffman tree");
		v = e[v][o];
	};
	auto go_b = [&](unsigned char w) {
		if (big && q[v * SZ + w] >= 0) {
			for (unsigned char &i : qv[v * SZ + w])
				print(i);
			v = q[v * SZ + w];
			keker++;
		} else {
			for (signed char i = 7; i >= 0; i--)
				go(!!(w & (1 << i)));
		}
	};
	{
		if (rlen) {
			go_b(srem ^ (((unsigned char)(input[0])) >> grem));
			srem = input[0];
			srem <<= 8 - grem;
		} else {
			srem = input[0];
			for (signed char i = 7; i >= grem; i--)
				go(!!(srem & (1 << i)));
			srem <<= 8 - grem;
		}
	}
	rlen = true;
    for (size_t iter = 1; iter != n; iter++) {
		go_b(srem ^ (((unsigned char)(input[iter])) >> grem));
		srem = input[iter];
		srem <<= 8 - grem;
    }
}

void Huffman_decoder::code_fin(char* a, size_t &n) {
	bad_stage(1);
	stage++;
	n = 0;
	int16_t &v = cur_v;
	size_t sum = 0;
	for (int i = 0; i < 256; i++)
		sum += cnt[i];
	if (e[v][0] < 0) {
		unsigned char id = -e[v][0] - 1;
		a[n++] = id;
		if (cnt[id] == 0)
			error("character with code " + std::to_string(uint32_t(id)) + " appears more than expected");
		cnt[id]--;
		v = 0;
	}
	if (v != 0)
		error("end up not in root");
}
