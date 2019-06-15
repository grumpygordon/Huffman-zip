#ifndef DECODER_H
#define DECODER_H
#include <fstream>
class Decoder {
private:
	std::ifstream fin;
	std::ofstream fout;
	bool const big;
public:

	Decoder(char const *in, char const *out, bool const big);

	~Decoder();

	void decode();

};
#endif //DECODER_H
