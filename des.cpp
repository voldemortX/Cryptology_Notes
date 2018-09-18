#include <bitset>
#include <iostream>
#include "des_parameters.h"
using namespace std;


template<int inSize, int outSize> 
bitset<outSize> permutation(bitset<inSize> block, int pBox[])
{
	bitset<outSize> processed; 
	for(int i = 1; i <= outSize; ++i)
		processed[outSize - i] = block[inSize - pBox[i - 1]];
	
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
		//cout << " p1: " << p1 << " p2: "<< p2 << endl;
		bitset<outSize> temp(sBox[parts - i - 1][p1][p2]);
		//cout << temp << endl;
		temp <<= outLen * i;
		processed |= temp;
	}
	return processed;
}

// not exactly f
void f(bitset<blockSize / 2>* l0, bitset<blockSize / 2>* r0, bitset<subkeySize> subkey)
{
	// round function
	const int half = blockSize / 2;
	bitset<half> l1 = *r0;
	bitset<subkeySize> tr1 = permutation<half, subkeySize>(*r0, E);  // E permutation(32->48)
	tr1 ^= subkey;  // xor round key
	bitset<half> r1 = substitution<subkeySize, half, 4, 16, 8>(tr1, S_BOX);  // S-Box
	r1 = permutation<half, half>(r1, P);  // P-Box
	r1 ^= *l0;  // xor l0
	// return values
	*l0 = l1;
	*r0 = r1;
	
}

void oddShift(bitset<keySize>* key, int bits)
{
	// divide
	const int half = keySize / 2; 
	bitset<half> left, right;
	for(int i = 0; i < half; ++i)
		right[i] = (*key)[i];
	for(int i = half; i < keySize; ++i)
		left[i - half] = (*key)[i];
	
	// bitwise ops
	left = (left >> half - bits) | (left << bits);
	right = (right >> half - bits) | (right << bits);
	
	// reunite
	for(int i = 0; i < half; ++i)
		(*key)[i] = right[i];
	for(int i = half; i < keySize; ++i)
		(*key)[i] = left[i - half];
	
}

void keyGeneration(bitset<subkeySize>* subkeys, bitset<blockSize> rawkey)
{
	bitset<keySize> key = permutation<blockSize, keySize>(rawkey, PC_1);  // permutation 1
	//cout << " orikey    : " << key << endl;
	
	// 16 rounds
	for(int i = 0; i < roundNum; ++i)
	{
		oddShift(&key, shiftBits[i]);  // interesting shift
		//cout << " aftershift: " << key << endl;
		subkeys[i] = permutation<keySize, subkeySize>(key, PC_2);  // permutation 2
		//cout << " subkey" << i << " : " << subkeys[i] << endl;
	}
}

// flag = 1: encryption
// flag = 0: decryption
bitset<blockSize> encryption(bitset<blockSize> block, bitset<subkeySize>* subkeys, bool flag)
{
	block = permutation<blockSize, blockSize>(block, IP);  // initial permutation
	
	// divide
	const int half = blockSize / 2; 
	bitset<half> left, right;
	for(int i = 0; i < half; ++i)
		right[i] = block[i];
	for(int i = half; i < blockSize; ++i)
		left[i - half] = block[i];
	
	// 16 rounds
	if(flag)  // encryption
		for(int i = 0; i < roundNum; ++i)
			f(&left, &right, subkeys[i]);  // SPN-round-function
	else  // decryption
		for(int i = roundNum - 1; i >= 0; --i)
			f(&left, &right, subkeys[i]);  // SPN-round-function
	
	// reunite	
	for(int i = 0; i < half; ++i)
		block[i] = left[i];
	for(int i = half; i < blockSize; ++i)
		block[i] = right[i - half];
		
	block = permutation<blockSize, blockSize>(block, IP_R);  // initial permutation reverse 
	
	return block;
}

bitset<blockSize> encrypt(bitset<blockSize> block, bitset<subkeySize>* subkeys)
{
	return encryption(block, subkeys, 1); 
} 

bitset<blockSize> decrypt(bitset<blockSize> block, bitset<subkeySize>* subkeys)
{
	return encryption(block, subkeys, 0); 
} 
