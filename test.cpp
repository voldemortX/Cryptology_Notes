#include <iostream>
#include <cstring>
#include <memory>
#include "des.cpp"
#include "des_parameters.h"
using namespace std;


int main()
{
	//bitset<48> k(string("111010101010101010101010101010101010101010101010"));
	//cout << k << endl;
	//bitset<32> kk = substitution<48, 32, 4, 16, 8>(k, S_BOX);
	//cout << kk << endl;
	//k = k << 24;
	//k = k >> 24;
	//bitset<24> kk;
	//kk |= k;
	//cout << kk << endl;
	bitset<blockSize> k(string("0110000101100010011000110110010001100101011001100110011101101000"));  // abcdefgh
	cout << k << endl;
	bitset<blockSize> key(string("0011000100110010001100110011010000110101001101100011011100111000"));  // 12345678
	bitset<subkeySize> subkeys[roundNum];
	keyGeneration(subkeys, key);
	k = encrypt(k, subkeys);
	cout << k << endl;
	k = decrypt(k, subkeys);
	cout << k << endl;
	
return 0;
}


