#include "utility.h"
#include "../lib/huffman_lib.h"
#include <cstdio>
#include <fstream>

Huffman_utility::Huffman_utility(char const *in, char const *out) : fin(in), fout(out) {}

Huffman_utility::~Huffman_utility() {
	fin.close();
	fout.close();
}

void Huffman_utility::encode() {
	Huffman q;
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
	while (fin) {
		fin.read(a, sizeof(a));
		if (fin.fail() && !fin.eof())
			q.error("couldn't read from input file");
		n = fin.gcount();
		if (n == 0)
			break;
		q.count(a, n);
	}
	q.make_codes();

    fin.clear();
    fin.seekg(0, std::ios::beg);

	q.encode_tree(b, m);
	print();
	while (1) {
		fin.read(a, sizeof(a));
		if (fin.fail() && !fin.eof())
			q.error("couldn't read from input file");
		n = fin.gcount();
		if (n == 0)
			break;
		q.encode(a, n, b, m);
		print();
	}
	q.encode_fin(b, m);
	print();
	fin.close();
	fout.close();
}

void Huffman_utility::decode() {
	Huffman q;
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
	bool st = 0;
	while (fin) {
		fin.read(a, sizeof(a));
		if (fin.fail() && !fin.eof())
			q.error("couldn't read from input file");
		n = fin.gcount();
		if (n == 0)
			break;
		if (!st) {
			q.decode_tree(a, n, b, m);
			st = 1;
		} else {
			q.decode(a, n, b, m);
		}
		print();
	}
	q.decode_fin(b, m);
	print();
	fin.close();
	fout.close();
}
