#include <iostream>
#include <memory.h>
#include <stdio.h>
#include "md5_parameters.h"
using namespace std;


unsigned int g(unsigned int x, unsigned int y, unsigned int z, int roundNum)
{
	// function g(..., 1)--F, g(..., 2)--G, g(..., 3)--H, g(..., 4)--I
	unsigned int ans;
	if(roundNum == 1)
		ans = (x & y) | (~x & z);
	else if(roundNum == 2)
		ans = (x & z) | (y & ~z);
	else if(roundNum == 3)
		ans = x ^ y ^ z;
	else
		ans = y ^ (x | ~z);
}

void transform(unsigned int* vector, unsigned int* msg)
{
	// 4*16 iterations
	unsigned int temp2[numParts];
	unsigned int temp1[numParts];
	memcpy(temp1, vector, numParts * sizeof(unsigned int));
	for(int i = 0; i < numIters; ++i)
	{
		temp2[0] = temp1[3];
		temp2[2] = temp1[1];
		temp2[3] = temp1[2];
		
		// calculate B = B + ((A + g(B, C, D) + M[k] + T[i]) <<< s)
		temp2[1] = temp1[0] + g(temp1[1], temp1[2], temp1[3], i / 16 + 1) + msg[M[i]] + T[i];
		temp2[1] = (temp2[1] >> (wordLen - S[i])) | (temp2[1] << S[i]);  // rotate-shift left
		temp2[1] += temp1[1];
		
		memcpy(temp1, temp2, numParts * sizeof(unsigned int));
		
		cout << i << ": " <<endl;
		for(int j = 0; j < numParts; ++j)
			printf("temp1: %x\n", temp1[j]);
	}
	
	// final additions
	for(int i = 0; i < numParts; ++i)
		temp1[i] += vector[i];
	 
	cout << "final: " <<endl;
	for(int j = 0; j < numParts; ++j)
		printf("temp1: %x\n", temp1[j]);
		
	memcpy(vector, temp1, numParts * sizeof(unsigned int));
}

