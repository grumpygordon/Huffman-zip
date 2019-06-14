#include "huffman_encoder.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <cassert>

Huffman_encoder::Huffman_encoder() : Huffman("encode"), t(), rlen(0), tlen() {}

Huffman_encoder::~Huffman_encoder() = default;

void Huffman_encoder::count(char const *a, size_t n) {
	bad_stage(0);
	for (size_t i = 0; i != n; i++)
		cnt[(unsigned char)a[i]]++;
}

void Huffman_encoder::make_huffman_codes() {
	bad_stage(0);
	stage++;
	Huffman::make_huffman_codes(t, tlen);
}

void Huffman_encoder::code_tree(char* output, size_t &m) {
	bad_stage(1);
	stage++;
	
	m = 0;

	const unsigned char big = 255;
	
	const uint32_t flag = uint32_t(big) << 24; 

    for (size_t i = 0; i < 256; i++) {
		uint32_t w = cnt[i];
		if (w >= flag)
			error("character with code " + std::to_string(i) + " appears more than " + std::to_string(flag) + " times");
		if (w == 0) {
			output[m++] = big;
			continue;
		}
		for (signed char j = 3; j >= 0; j--) {
			output[m++] = (w >> (8 * j));
			w -= uint32_t(output[m - 1]) << (8 * j);
		}
    }
	output[m++] = grem;
}

void Huffman_encoder::code(char const *input, size_t n, char* output, size_t &m) {
	bad_stage(2);

	m = 0;
	for (size_t iter = 0; iter != n; iter++) {
        unsigned char w = input[iter], dlen = tlen[w] >> 3, last = t[w][dlen];
		for (size_t i = 0; i < dlen; i++) {
			output[m++] = (srem ^ (t[w][i] >> rlen));
			srem = t[w][i] << (8 - rlen);
		}
		dlen = tlen[w] & 7;
		if (rlen + dlen >= 8) {
			output[m++] = (srem ^ (last >> rlen));
			srem = last << (8 - rlen);
			rlen += dlen - 8;
		} else {
			srem ^= last >> rlen;
			rlen += dlen;
		}
    }
}

void Huffman_encoder::code_fin(char *a, size_t &n) {
	bad_stage(2);
	stage++;
	n = 0;
	if (grem > 0)
		a[n++] = char(srem);
}
