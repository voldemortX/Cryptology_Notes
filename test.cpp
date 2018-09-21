#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include "encryptor.cpp"
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
	bitset<blockSize> k(string("0110000101100010011000110110010101100100011001100110011101101000"));  // 4e25
	cout << k << endl;
	bitset<blockSize> key(string("0110000101100010011000110110010101100100011001100110011101101000"));  // 4e25
	bitset<subkeySize> subkeys[roundNum];
	keyGeneration(subkeys, key);
	k = encrypt(k, subkeys);
	cout << k << endl;
	k = decrypt(k, subkeys);
	cout << k << endl;

	
	ifstream f("001.dat",ios::binary);
	if(f == NULL)
		cout << "!" << endl;
	
	string key = "ÈÕ";
	cout << key.size() << endl;
	
	char x[10] = {'a', 'b','a', 'b','a', 'b','a', 'b','a', 'b'};
	unsigned long long int a = 0;
	bitset<64> k;
	//char* t = &k;
	memcpy(&k, x, 8);
	bitset<8> kk('a');
	cout << k << endl;
	cout << kk << endl; 
	*/	
	bool x = des("1.txt", "111.txt", "abcdefgh", "cbc", "encrypt", "Junoscock");
	if(x)
		cout << "En done!" << endl;
	x = des("111.txt", "1111.txt", "abcdefgh", "cbc", "decrypt", "Junoscock");
	if(x)
		cout << "De done!" << endl;
	
	
return 0;
}


