#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include "huffman_lib.h"

class Huffman_decoder : public Huffman {
private:

	const size_t threshold;

	int16_t cur_v, e[2 * SZ][2], *q;
	
	std::vector<std::vector<unsigned char> > qv;

	bool rlen, big;

public:

	Huffman_decoder();
	
	Huffman_decoder(size_t w);
	
	~Huffman_decoder();
	
	void code_tree(char const *in, size_t n, char *out, size_t &m);
	
	void code(char const *in, size_t n, char *out, size_t &m);

	void code_fin(char *out, size_t &n);
};

#endif //HUFFMAN_DECODER_H
