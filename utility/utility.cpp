#include "utility.h"
#include "../lib/huffman_encoder.h"
#include "../lib/huffman_decoder.h"
#include <fstream>
void encode(char const *in, char const *out) {
	std::ifstream fin(in, std::ios::binary);
	std::ofstream fout(out, std::ios::binary);
	Huffman_encoder q;
    if (fin.fail()) {
        q.error("couldn't open input file");
	}
    if (fout.fail()) {
        q.error("couldn't open output file");
	}
	char a[Huffman::ISZ], b[Huffman::OSZ];
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

void decode(char const *in, char const *out, const bool big) {
	std::ifstream fin(in, std::ios::binary);
	std::ofstream fout(out, std::ios::binary);
	Huffman_decoder q(big);
    if (fin.fail()) {
        q.error("couldn't open input file");
	}
    if (fout.fail()) {
        q.error("couldn't open output file");
	}
	char a[Huffman::ISZ], b[Huffman::OSZ];
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

