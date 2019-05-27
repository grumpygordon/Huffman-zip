#include "huffman_lib.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <cassert>

Huffman::Huffman() {
	memset(cnt, 0, sizeof(cnt));
	memset(e, 0, sizeof(e));
	for (size_t i = 0; i < SZ; i++) {
		t[i] = "";
	}
	srem = "";
	cur_v = 0;
}

void Huffman::error(std::string s) {
	throw std::runtime_error("Error occured : " + s + ".\n");
}

Huffman::~Huffman() = default;

void Huffman::concat(std::vector<uchar> &a, std::vector<uchar> &b) {
    a.insert(a.end(), b.begin(), b.end());
}

void Huffman::get_codes() {
    std::pair<uint, uchar> q[SZ];
    for (size_t i = 0; i < SZ; i++) {
        q[i] = {cnt[i], i};
	}
    std::stable_sort(q, q + SZ);
    std::vector<std::pair<uint, std::vector<uchar> > > g;

    size_t it = 0;
    
    for (size_t i = 0; i <= SZ; i++) {
        while (it + 1 < g.size() && (i == SZ || q[i].first > g[it + 1].first)) {
            g.push_back({g[it].first + g[it + 1].first, {}});
            for (uchar j = 0; j < 2; j++)
                for (uchar id : g[it + j].second)
                    t[id].push_back(48 + j);
            concat(g.back().second, g[it].second);
            concat(g.back().second, g[it + 1].second);
            it += 2;
        }
        if (i + 1 < SZ && (it == g.size() || g[it].first > q[i + 1].first)) {
            g.push_back({q[i].first + q[i + 1].first, {q[i].second, q[i + 1].second}});
            t[q[i].second].push_back(48);
            t[q[i + 1].second].push_back(49);
            i++;
        } else if (i < SZ && it < g.size()) {
			g.push_back(g[it]);
            for (uchar id : g.back().second)
                t[id].push_back(48);
			t[q[i].second].push_back(49);
            g.back().second.push_back(q[i].second);
            g.back().first += q[i].first;
            it++;
        }
    }
    for (size_t i = 0; i < SZ; i++)
        std::reverse(t[i].begin(), t[i].end());
}


void Huffman::count(char *a, size_t n) {
	for (size_t i = 0; i != n; i++)
		cnt[uchar(a[i])]++;
}

uint32_t Huffman::get_rem() {
	uint lens = 0;
    for (size_t i = 0; i < 256; i++)
        lens += cnt[i] * t[i].size();
    lens %= 8;
    if (lens > 0)
		lens = 8 - lens;
	return lens;
}

void Huffman::encode_tree(char* output, size_t &m) {
	m = 0;
	
	auto print = [&](char w) {
		assert(m < OSZ);
		output[m++] = w;
	};

	const uchar big = 255;
	
	const uint flag = uint(big) << 24; 

    for (size_t i = 0; i < 256; i++) {
		uint w = cnt[i];
		if (w >= flag)
			error("some character appears more than " + std::to_string(flag) + " times");
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
	print(get_rem());
}

void Huffman::encode(char *input, size_t n, char* output, size_t &m) {
	m = 0;
	
	auto print = [&](char w) {
		assert(m < OSZ);
		output[m++] = w;
	};

	for (size_t iter = 0; iter != n; iter++) {
        uchar w = input[iter];
		srem += t[w];
		if (srem.size() >= 8) {
			size_t i = 0;
			for (; i + 7 < srem.size(); i += 8) {
				char v = 0;
				for (size_t j = i; j < i + 8; j++) {
					v = (v << 1) ^ (srem[j] & 1);
				}
				print(v);
			}
			srem = srem.substr(i, srem.size() - i);
		}
    }
}

void Huffman::encode_fin(char* a, size_t &n) {
	n = 0;
	uint rem = get_rem();
	if (rem > 0) {
        uchar c = 0;
        for (size_t j = 0; j < 8 - rem; j++)
            c = (c << 1) ^ (srem[j] & 1);
		a[n++] = char(c << rem);
    }
    srem = "";   
	for (size_t i = 0; i < SZ; i++)
		t[i] = "";
	memset(cnt, 0, sizeof(cnt));
}

std::string Huffman::bits(char w) {
    std::string s;
    for (size_t i = 0; i < 8; i++) {
        s.push_back(48 + (w & 1));
        w >>= 1;
    }
    std::reverse(s.begin(), s.end());
    return s;
}

void Huffman::decode_tree(char *input, size_t n, char* output, size_t &m) {
	size_t ipos = 0;
	
	m = 0;

	auto read = [&]() {
		if (ipos >= n)
			error("not enough data to restore the tree, maybe file was damaged");
		return uchar(input[ipos++]);
	};

	auto print = [&](char w) {
		assert(m < OSZ);
		output[m++] = w;
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

	get_codes();
	
	size_t sz = 1;

	for (size_t it = 0; it < SZ; it++) {
		size_t w = 0;
		for (char c : t[it]) {
			c -= 48;
			if (e[w][c] == 0)
				e[w][c] = sz++;
			w = e[w][c];
		}
		e[w][0] = e[w][1] = -1 - it;
	}
    
    
    cur_v = 0;

    uint dec_rem = read();
    
    if (dec_rem != get_rem())
		error("number of useless bits doesn't match");
    
    if (ipos < n)
		decode(input + ipos, n - ipos, output, m);
}

void Huffman::decode(char *input, size_t n, char* output, size_t &m) {
	m = 0;
	auto print = [&](char w) {
		assert(m < OSZ);
		output[m++] = w;
	};

	uint &v = cur_v;

	auto go = [&](size_t o) {
		if (e[v][o] < 0) {
			print(uchar(-e[v][o] - 1));
			v = 0;
		}
		if (e[v][o] == 0)
			error("couldn't find vertex in huffman tree");
		v = e[v][o];
	};

	uint dec_rem = get_rem();

    for (size_t iter = 0; iter != n; iter++) {
		if (!srem.empty()) {
			for (size_t i = 8 - dec_rem; i < 8; i++)
				go(srem[i] - 48);
		}
		srem = bits(input[iter]);
		for (size_t i = 0; i < 8 - dec_rem; i++)
			go(srem[i] - 48);
    }

}

void Huffman::decode_fin(char* a, size_t &n) {
	n = 0;
	uint &v = cur_v;
	if (e[v][0] < 0) {
		a[n++] = char(-e[v][0] - 1);
		v = 0;
	}
	if (v != 0)
		error("end up not in root");
}
