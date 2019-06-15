#include "huffman_decoder.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <cassert>

Huffman_decoder::Huffman_decoder() : Huffman_decoder(false) {}

Huffman_decoder::Huffman_decoder(const bool bg) : Huffman_decoder(200000000, bg) {}

Huffman_decoder::Huffman_decoder(size_t w, const bool bg) : 
	Huffman("decode"), threshold(w), root(new Huffman_tree()), cur_v(nullptr), rlen(false), big(bg) {}

Huffman_decoder::~Huffman_decoder() {
	delete root;
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
	for (size_t it = 0; it < SZ; it++) {
		cur_v = root;
		for (size_t i = 0; i < tlen[it]; i++) {
			bool c = !!(t[it][i >> 3] & (1 << ((i & 7) ^ 7)));
			if (cur_v->e[c] == nullptr)
				cur_v->e[c] = new Huffman_tree;
			cur_v = cur_v->e[c];
		}
		if (cur_v->e[0] != nullptr || cur_v->e[1] != nullptr || cur_v->t)
			error("couldn't make huffman tree");
		cur_v->t = true;
		cur_v->c = it;
	}
	
	if (sum >= threshold || big) {
		big = true;
		std::cerr << "Making precalculations.\n";
		root->dfs(root);
	} else {
		big = false;
	}
    cur_v = root;

    uint32_t dec_rem = read();
    
    if (dec_rem != grem)
		error("number of useless bits doesn't match");
    
    if (ipos < n)
		code(input + ipos, n - ipos, output, m);
}

void Huffman_decoder::code(char const *input, size_t n, char* output, size_t &m) {
	bad_stage(1);
	m = 0;
	if (n == 0)
		return;
	size_t st_i = 0;
	if (!rlen) {
		srem = input[0];
		for (ptrdiff_t i = 7; i >= grem; i--) {
			cur_v = cur_v->e[(srem >> i) & 1];
			if (cur_v == nullptr)
				error("couldn't find vertex in tree");
			if (cur_v->t) {
				output[m++] = cur_v->c;
				cur_v = root;
			}
		}
		srem <<= 8 - grem;
		rlen = true;
		st_i = 1;
	}
    for (size_t iter = st_i; iter != n; iter++) {
		unsigned char w = (srem ^ (((unsigned char)(input[iter])) >> grem));
		if (!big) {
			for (ptrdiff_t i = 7; i >= 0; i--) {
				cur_v = cur_v->e[(w >> i) & 1];
				if (cur_v == nullptr)
					error("couldn't find vertex in tree");
				if (cur_v->t) {
					output[m++] = cur_v->c;
					cur_v = root;
				}
			}
		} else {
			auto &pr = cur_v->q[w];
			if (pr.first == nullptr)
				error("couldn't find vertex in tree");
			for (const char &i : pr.second)
				output[m++] = i;
			cur_v = pr.first;
		}
		srem = input[iter];
		srem <<= 8 - grem;
    }
}

void Huffman_decoder::code_fin(char* a, size_t &n) {
	bad_stage(1);
	stage++;
	n = 0;
	size_t sum = 0;
	for (int i = 0; i < 256; i++)
		sum += cnt[i];
	if (cur_v->t) {
		a[n++] = cur_v->c;
		cur_v = root;
	}
	if (cur_v != root)
		error("end up not in root");
}
