#ifndef HUFFMAN_LIB_H
#define HUFFMAN_LIB_H

#include <string>
#include <vector>

typedef unsigned char uchar;

typedef uint32_t uint;

class Huffman {
private:

	static const size_t SZ = 256;
	
	uint32_t cnt[SZ], cur_v;
	
	int32_t e[2 * SZ][2];

	std::string t[SZ], srem;

	uint32_t get_rem();

	std::string bits(char a);
	
	void concat(std::vector<uchar> &a, std::vector<uchar> &b);
public:

	static const size_t OSZ = 16384, ISZ = 2048;

	Huffman();
	
	~Huffman();

	void error (std::string s);

	void get_codes();
	
	void count(char *a, size_t n);
	
	void encode_tree(char* a, size_t &n);
	
	void encode(char *a, size_t n, char* b, size_t &m);

	void encode_fin(char* a, size_t &n);
	
	void decode_tree(char *a, size_t n, char* b, size_t &m);
	
	void decode(char *a, size_t n, char* b, size_t &m);
	
	void decode_fin(char* a, size_t &n);

};

#endif //HUFFMAN_LIB_H
