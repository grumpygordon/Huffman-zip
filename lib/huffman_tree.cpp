#include "huffman_tree.h"

huffman_tree::huffman_tree() : e(), t(false), c() {}
huffman_tree::~huffman_tree() {
	for (int i = 0; i < 2; i++)
		if (e[i] != nullptr)
			delete e[i];
}

void huffman_tree::dfs(huffman_tree *root) {
	for (size_t j = 0; j < 256; j++) {
		q.push_back({nullptr, {}});
		huffman_tree *&v = q.back().first;
		v = this;
		for (ptrdiff_t k = 7; k >= 0; k--) {
			v = v->e[(j >> k) & 1];
			if (v == nullptr)
				break;
			if (v->t) {
				q.back().second.push_back(v->c);
				v = root;
			}
		}
	}
	for (size_t i = 0; i < 2; i++) {
		if (e[i] != nullptr)
			e[i]->dfs(root);
	}
}
