#include <iostream>
#include <bitset>
#include <memory.h>
#include "md5_parameters.h"
#include "encryptor.cpp"
using namespace std;


int main()
{
	//bool status = md5("in.txt", "out.txt");
	//cout << status << endl;
	unsigned int vector[4] = {A, B, C, D};
	unsigned int msg[16] = {3901340032, 0,0,0,0,0,0,0,0,0,0,0,0,0, 0,24};
	char* out = (char*)malloc(sizeof(char)*(outBytes));
	transform(vector, msg);
	for(int i = 0; i < numParts; ++i)
	{
		bitset<wordLen> temp(vector[i]);
		reverseCopy<bitset<wordLen>*, char*>(&temp, out + i * wordLen / 8, wordLen / 8);
	}
	bitset<128> outB;
	reverseCopy<char*, bitset<128>*>(out, &outB, 16);
	cout << outB << endl;

return 0;
}
