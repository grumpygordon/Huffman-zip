#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H
#include <vector>

class Huffman_tree {
public:
	Huffman_tree *e[2];
	bool t;
	char c;
	std::vector<std::pair<Huffman_tree*, std::vector<char> > > q;
	
	Huffman_tree();
	~Huffman_tree();
	void dfs(Huffman_tree *);
};

#endif // HUFFMAN_TREE_H
