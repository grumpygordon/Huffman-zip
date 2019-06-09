#include "huffman_lib.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <cassert>

Huffman::Huffman(std::string const &s) : cnt(), srem(0), grem(0), stage(0), s_err(s) {}

Huffman::~Huffman() = default;

void Huffman::concat(std::vector<unsigned char> &a, std::vector<unsigned char> const &b) const {
    a.insert(a.end(), b.begin(), b.end());
}

void Huffman::bad_stage(int32_t stg) const {
	if (stg != stage)
		error("expected stage number is " + std::to_string(stg) + ", actuall is " + std::to_string(stage));
}

void Huffman::error(std::string const &s) const {
	throw std::runtime_error("Failed to " + s_err + " : " + s + ".\n");
}

void Huffman::make_huffman_codes(unsigned char t[SZ][TSZ], size_t tlen[SZ]) {
    std::pair<uint32_t, unsigned char> q[SZ];
    for (size_t i = 0; i < SZ; i++) {
        q[i] = {cnt[i], i};
	}
    std::stable_sort(q, q + SZ);
    std::vector<std::pair<uint32_t, std::vector<unsigned char> > > g;

    size_t it = 0;

	std::string arr[SZ];

	for (size_t i = 0; i < SZ; i++)
		arr[i] = "";

    auto add = [&](size_t i, char ts) {
		arr[i].push_back(ts + '0');
	};
    
    for (size_t i = 0; i <= SZ; i++) {
        while (it + 1 < g.size() && (i == SZ || q[i].first > g[it + 1].first)) {
            g.push_back({g[it].first + g[it + 1].first, {}});
            for (char j = 0; j < 2; j++)
                for (unsigned char id : g[it + j].second)
                    add(id, j);
            concat(g.back().second, g[it].second);
            concat(g.back().second, g[it + 1].second);
            it += 2;
        }
        if (i + 1 < SZ && (it == g.size() || g[it].first > q[i + 1].first)) {
            g.push_back({q[i].first + q[i + 1].first, {q[i].second, q[i + 1].second}});
			add(q[i].second, 0);
			add(q[i + 1].second, 1);
            i++;
        } else if (i < SZ && it < g.size()) {
			g.push_back(g[it]);
            for (unsigned char id : g.back().second)
				add(id, 0);
			add(q[i].second, 1);
            g.back().second.push_back(q[i].second);
            g.back().first += q[i].first;
            it++;
        }
    }
    for (size_t i = 0; i < SZ; i++) {
		std::string s = arr[i];
		std::reverse(s.begin(), s.end());
		for (char c : s) {
			bool ts = (c & 1);
			if (ts)
				t[i][tlen[i] >> 3] ^= (1 << ((tlen[i] & 7) ^ 7));
			tlen[i]++;
		}
	}
	uint32_t lens = 0;
    for (size_t i = 0; i < SZ; i++)
        lens += cnt[i] * tlen[i];
    lens %= 8;
    if (lens > 0)
		lens = 8 - lens;
	grem = lens;
}
