#ifndef HUFFMAN_UTILITY_H
#define HUFFMAN_UTILITY_H
#include <fstream>
class Huffman_utility {
private:
	std::ifstream fin;
	std::ofstream fout;
	const bool big;
public:

	Huffman_utility(char const *in, char const *out, const bool bg);

	~Huffman_utility();

	void decode();

	void encode();

};
#endif //HUFFAMN_UTILITY_H
