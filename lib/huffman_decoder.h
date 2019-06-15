#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include "huffman_lib.h"
#include "huffman_tree.h"

class Huffman_decoder : public Huffman {
private:

	const size_t threshold;

	Huffman_tree *root, *cur_v;

	bool rlen, big;

public:

	Huffman_decoder();

	Huffman_decoder(const bool bg);
	
	Huffman_decoder(const size_t w, const bool bg);
	
	~Huffman_decoder();
	
	void code_tree(char const *in, size_t n, char *out, size_t &m);
	
	void code(char const *in, size_t n, char *out, size_t &m);

	void code_fin(char *out, size_t &n);
};

#endif //HUFFMAN_DECODER_H
