#ifndef ENCODER_H
#define ENCODER_H
#include <fstream>
class Encoder {
private:
	std::ifstream fin;
	std::ofstream fout;
public:

	Encoder(char const *in, char const *out);

	~Encoder();

	void encode();

};
#endif //ENCODER_H
