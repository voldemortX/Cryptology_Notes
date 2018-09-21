#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include <memory.h>
#include "des.cpp"
#include "des_parameters.h"
using namespace std;

void reverse(char *s, int n)
{
	int i;
	char t;
	for(i=0; i<=n/2; i++)
	{
		t=*(s+i);
		*(s+i)=*(s+n-i-1);
		*(s+n-i-1)=t;
	}
} 

template<typename t1, typename t2>
void reverseCopy(t1 in, t2 out, int n)
{
	char* temp = (char*)malloc(n * sizeof(char));
	memcpy(temp, in, n);
	reverse(temp, n);
	memcpy(out, temp, n);
}

bool des_cbc(char* in, char* out, long long numPkt, string op, bitset<subkeySize>* subkeys)
{
	
}

bool des_ecb(char* in, char* out, long long numPkt, string op, bitset<subkeySize>* subkeys)
{
	// about 84KBps
	for(long long i = 0; i < numPkt; ++i)
	{
		if(i % 128 == 0)
		{
			cout << (double)i / numPkt * 100 << "%" << endl;
		}
		bitset<blockSize> pkt;
		reverseCopy<char*, bitset<blockSize>*>(in + i * 8, &pkt, 8);
		if(op == "encrypt")
		{
			 pkt = encrypt(pkt, subkeys);
			 //cout << "en: " << pkt << endl;
		}
		else if(op == "decrypt")
		{
			 pkt = decrypt(pkt, subkeys);
			 //cout << "de: " << pkt << endl;
		}
		else
			return false;
		reverseCopy<bitset<blockSize>*, char*>(&pkt, out + i * 8, 8);
	
	}
	return true;
}

bool des(string inputFilename, string outputFilename, string key, string mode, string op, string IV = "")
{
	bool status = false;
	ifstream inputFile(inputFilename, ios::binary);
	if((inputFile != NULL) && (key.size() >= 8))
	{
		inputFile.seekg(0, ios::end);
		long long e = inputFile.tellg();
		inputFile.seekg(0, ios::beg);
		long long s = inputFile.tellg();
		long long len = e - s;
		long long numPkt = len / 8;
		long long numPad = 8 - len % 8;
		cout << "numpad: " << numPad << " len:" << len << endl;
		char* in = (char*)malloc(sizeof(char)*(len + 16));
		char* out = (char*)malloc(sizeof(char)*(len + 16));
		inputFile.read(in, len);
		inputFile.close();
		bitset<blockSize> rawkey;
		reverseCopy<const char*, bitset<blockSize>*>(key.c_str(), &rawkey, 8);
		bitset<subkeySize> subkeys[roundNum];
		keyGeneration(subkeys, rawkey);
		
		// pkcs5 padding
		long long writeLen = len;
		if(op == "encrypt")
		{
			writeLen += numPad;
			for(long long i = len; i < writeLen; ++i)
				in[i] = numPad;
			
			++numPkt;
			//cout << "pkt: " << in << endl; 
		}
		
		if(mode == "ecb" || mode == "ECB")	
			status = des_ecb(in, out, numPkt, op, subkeys);
		
		// pkcs5 padding
		if(op == "decrypt")
		{
			numPad = out[len - 1];
			writeLen -= numPad;
		}

		ofstream outputFile(outputFilename, ios::binary);
		outputFile.seekp(0,ios::beg);
		outputFile.write(out, writeLen);
		outputFile.close();
		status = true;
	}
	
	return status;
}
