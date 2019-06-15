#include "encoder.h"
#include "../lib/huffman_encoder.h"

Encoder::Encoder(char const *in, char const *out) : 
	fin(in, std::ios::binary), fout(out, std::ios::binary) {}

Encoder::~Encoder() {
	fin.close();
	fout.close();
}

void Encoder::encode() {
	Huffman_encoder q;
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
	while (fin) {
		read_block();
		n = fin.gcount();
		if (n == 0)
			break;
		q.count(a, n);
	}
	q.make_huffman_codes();

    fin.clear();
    fin.seekg(0, std::ios::beg);
	if (fin.fail())
		q.error("couldn't reset input file");
	q.code_tree(b, m);
	print();
	while (fin) {
		read_block();
		n = fin.gcount();
		if (n == 0)
			break;
		q.code(a, n, b, m);
		print();
	}
	q.code_fin(b, m);
	print();
	fin.close();
	fout.close();
}
