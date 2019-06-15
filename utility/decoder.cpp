#include "decoder.h"
#include "../lib/huffman_decoder.h"

Decoder::Decoder(char const *in, char const *out, bool const bg) : 
	fin(in, std::ios::binary), fout(out, std::ios::binary), big(bg) {}

Decoder::~Decoder() {
	fin.close();
	fout.close();
}

void Decoder::decode() {
	Huffman_decoder q(big);
    if (fin.fail()) {
        q.error("couldn't open input file");
	}
    if (fout.fail()) {
        q.error("couldn't open output file");
	}
	char a[q.ISZ], b[q.OSZ];
	size_t n, m;
	auto print = [&]() {
		fout.write(b, m);
		if (fout.fail())
			q.error("couldn't write to output file");
	};
	auto read_block = [&]() {
		fin.read(a, sizeof(a));
		if (fin.fail() && !fin.eof())
			q.error("couldn't read from input file");
	};
	bool st = false;
	while (fin) {
		read_block();
		n = fin.gcount();
		if (n == 0)
			break;
		if (!st) {
			q.code_tree(a, n, b, m);
			st = true;
		} else {
			q.code(a, n, b, m);
		}
		print();
	}
	if (!st)
		q.error("file is empty, frequencies expected");
	q.code_fin(b, m);
	print();
	fin.close();
	fout.close();
}
