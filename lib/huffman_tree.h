#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H
#include <vector>

class huffman_tree {
public:
	huffman_tree *e[2];
	bool t;
	char c;
	std::vector<std::pair<huffman_tree*, std::vector<char> > > q;
	
	huffman_tree();
	~huffman_tree();
	void dfs(huffman_tree *);
};

#endif // HUFFMAN_TREE_H
