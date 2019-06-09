#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include "huffman_lib.h"

class Huffman_encoder : public Huffman {
private:

	unsigned char t[SZ][TSZ], rlen;
	
	size_t tlen[SZ];

public:

	Huffman_encoder();
	
	~Huffman_encoder();

	void count(char const *in, size_t n);

	void make_huffman_codes();
	
	void code_tree(char *out, size_t &n);
	
	void code(char const *in, size_t n, char* out, size_t &m);

	void code_fin(char* out, size_t &n);
};

#endif //HUFFMAN_ENCODER_H
