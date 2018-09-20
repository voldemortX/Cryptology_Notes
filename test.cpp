#include <iostream>
#include <string>
#include <memory>
#include <fstream>
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
	/*
	bitset<blockSize> k(string("1110010010111000101001011110010010111000101001011110010010111000"));  // 4e25
	cout << k << endl;
	bitset<blockSize> key(string("1110010010111000101001011110010010111000101001011110010010111000"));  // 4e25
	bitset<subkeySize> subkeys[roundNum];
	keyGeneration(subkeys, key);
	k = encrypt(k, subkeys);
	cout << k << endl;
	k = decrypt(k, subkeys);
	cout << k << endl;
	*/
	
	ifstream f("001.dat",ios::binary);
	if(f == NULL)
		cout << "!" << endl;
	
	string key = "ÈÕ";
	cout << key.size() << endl;
return 0;
}


