#include "utility.h"
#include "../lib/huffman_lib.h"
#include <cstdio>

void encode(char* in, char* out) {
	FILE *fin, *fout;
	Huffman q;
	if (!(fin = std::fopen(in, "rb")))
		q.error("couldn't open input file");
	if (!(fout = std::fopen(out, "wb")))
		q.error("couldn't open output file");
	char a[q.ISZ], b[q.OSZ];
	size_t n, m;
	auto print = [&]() {
		if (std::fwrite(b, 1, m, fout) != m)
			q.error("couldn't write to output file");
	};
	while (1) {
		n = std::fread(a, 1, sizeof(a), fin);
		if (n == 0)
			break;
		q.count(a, n);
	}
	q.get_codes();
	rewind(fin);
	q.encode_tree(b, m);
	print();
	while (1) {
		n = std::fread(a, 1, sizeof(a), fin);
		if (n == 0)
			break;
		q.encode(a, n, b, m);
		print();
	}
	q.encode_fin(b, m);
	print();
	std::fclose(fin);
	std::fclose(fout);
}

void decode(char* in, char* out) {
	FILE *fin, *fout;
	Huffman q;
	if (!(fin = std::fopen(in, "rb")))
		q.error("couldn't open input file");
	if (!(fout = std::fopen(out, "wb")))
		q.error("couldn't open output file");
	char a[q.ISZ], b[q.OSZ];
	size_t n, m;
	auto print = [&]() {
		if (std::fwrite(b, 1, m, fout) != m)
			q.error("couldn't write to output file");
	};
	n = fread(a, 1, sizeof(a), fin);
	q.decode_tree(a, n, b, m);
	print();
	while (1) {
		n = fread(a, 1, sizeof(a), fin);
		if (n == 0)
			break;
		q.decode(a, n, b, m);
		print();
	}
	q.decode_fin(b, m);
	print();
	std::fclose(fin);
	std::fclose(fout);
}
