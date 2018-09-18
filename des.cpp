#include <bitset>
#include "des_parameters.h"
using namespace std;


template<int inSize, int outSize> 
bitset<outSize> permutation(bitset<inSize> block, int[] pBox)
{
	bitset<outSize> processed; 
	for(int i = 1; i <= outSize; ++i)
		processed[outSize - i] = block[inSize - pBox[i]];
	
	return processed;
}



