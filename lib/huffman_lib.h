#ifndef HUFFMAN_LIB_H
#define HUFFMAN_LIB_H

#include <string>
#include <vector>

class Huffman {

private:
	
	void concat(std::vector<unsigned char> &a, std::vector<unsigned char> const &b) const;

protected:

	static const size_t SZ = 256, TSZ = 32;
	
	uint32_t cnt[SZ];

	unsigned char srem, grem;
	
	int32_t stage;

	std::string const s_err;

	void bad_stage(int32_t stg) const;

	void make_huffman_codes(unsigned char t[SZ][TSZ], size_t tlen[SZ]);
	
public:

	static const size_t OSZ = 16384, ISZ = 2048;

	Huffman(std::string const &s);
	
	~Huffman();
	
	void error(std::string const &s) const;
	
	//virtual void code(char const *in, size_t n, char *out, size_t &m) = 0;

	//virtual void code_fin(char *out, size_t &n) = 0;
};

#endif //HUFFMAN_LIB_H
