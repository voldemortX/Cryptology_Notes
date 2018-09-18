#include <bitset>
#include <iostream>
#include "des_parameters.h"
using namespace std;


template<int inSize, int outSize> 
bitset<outSize> permutation(bitset<inSize> block, int pBox[])
{
	bitset<outSize> processed; 
	for(int i = 1; i <= outSize; ++i)
		processed[outSize - i] = block[inSize - pBox[i]];
	
	return processed;
}

template<int inSize, int outSize, int sHeight, int sWidth, int parts>
bitset<outSize> substitution(bitset<inSize> block, int sBox[][sHeight][sWidth])
{	
	const int inLen = inSize / parts;
	const int outLen = outSize / parts;
	bitset<outSize> processed;
	// bitset: least significant bit first 
	for(int i = parts - 1; i >= 0; --i)
	{
		int p1 = block[i * inLen] + 2 * block[(i + 1) * inLen - 1];
		int p2 = block[i * inLen + 1] + block[i * inLen + 2] * 2 + block[i * inLen + 3] * 4 + block[i * inLen + 4] * 8;
		cout << " p1: " << p1 << " p2: "<< p2 << endl;
		bitset<outLen> temp(sBox[parts - i - 1][p1][p2]);
		cout << temp << endl;
		for(int j = 0; j < outLen; ++j)
			processed[j + i * outLen] = temp[j];
	}
	return processed;
}

 



