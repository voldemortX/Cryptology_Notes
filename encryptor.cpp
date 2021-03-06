/*
Knowledge I pursue, not efficiency. 
#environment#
CPU:    Intel i5-6300HQ(2.30GHz)
Memory: 4G*2 
*/
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
	// looks like the best one I found
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

bool des_cfb64(char* in, char* out, long long numPkt, string op, bitset<subkeySize>* subkeys, string IV)
{
	// about 80KBps
	if(IV.size() < blockBytes)
		return false;
		
	bitset<blockSize> salt;  // feed
	reverseCopy<const char*, bitset<blockSize>*>(IV.c_str(), &salt, blockBytes);
	for(long long i = 0; i < numPkt; ++i)
	{
		if(i % 128 == 0)
		{
			cout << (double)(i+1) / numPkt * 100 << "%" << endl;
		}
		bitset<blockSize> pkt;
		reverseCopy<char*, bitset<blockSize>*>(in + i * blockBytes, &pkt, blockBytes);
		if(op == "encrypt")
		{
			salt = encrypt(salt, subkeys);;
			pkt ^= salt; 
			salt = pkt;
			//cout << "en: " << pkt << endl;
		}
		else if(op == "decrypt")
		{
			bitset<blockSize> temp = pkt;
			salt = encrypt(salt, subkeys);
			pkt ^= salt;
			salt = temp;
			//cout << "de: " << pkt << endl;
		}
		else
			return false;
		reverseCopy<bitset<blockSize>*, char*>(&pkt, out + i * blockBytes, blockBytes);
	
	}
	return true;
}

bool des_cbc(char* in, char* out, long long numPkt, string op, bitset<subkeySize>* subkeys, string IV)
{
	// about 76KBps
	if(IV.size() < blockBytes)
		return false;
	bitset<blockSize> salt;  // feed
	reverseCopy<const char*, bitset<blockSize>*>(IV.c_str(), &salt, blockBytes);
	for(long long i = 0; i < numPkt; ++i)
	{
		if(i % 128 == 0)
		{
			cout << (double)(i+1) / numPkt * 100 << "%" << endl;
		}
		bitset<blockSize> pkt;
		reverseCopy<char*, bitset<blockSize>*>(in + i * blockBytes, &pkt, blockBytes);
		if(op == "encrypt")
		{
			pkt ^= salt;
			pkt = encrypt(pkt, subkeys);
			salt = pkt;
			//cout << "en: " << pkt << endl;
		}
		else if(op == "decrypt")
		{
			bitset<blockSize> temp = pkt;
			pkt = decrypt(pkt, subkeys);
			pkt ^= salt;
			salt = temp;
			//cout << "de: " << pkt << endl;
		}
		else
			return false;
		reverseCopy<bitset<blockSize>*, char*>(&pkt, out + i * blockBytes, blockBytes);
	
	}
	return true;
}

bool des_ecb(char* in, char* out, long long numPkt, string op, bitset<subkeySize>* subkeys)
{
	// about 84KBps
	for(long long i = 0; i < numPkt; ++i)
	{
		if(i % 128 == 0)
		{
			cout << (double)(i+1) / numPkt * 100 << "%" << endl;
		}
		bitset<blockSize> pkt;
		reverseCopy<char*, bitset<blockSize>*>(in + i * blockBytes, &pkt, blockBytes);
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
		reverseCopy<bitset<blockSize>*, char*>(&pkt, out + i * blockBytes, blockBytes);
	
	}
	return true;
}

bool des(string inputFilename, string outputFilename, string key, string mode, string op, string IV = "")
{
	bool status = false;
	ifstream inputFile(inputFilename, ios::binary);
	if((inputFile != NULL) && (key.size() >= blockBytes))
	{
		inputFile.seekg(0, ios::end);
		long long e = inputFile.tellg();
		inputFile.seekg(0, ios::beg);
		long long s = inputFile.tellg();
		long long len = e - s;
		long long numPkt = len / blockBytes;
		long long numPad = blockBytes - len % blockBytes;
		cout << "numpad: " << numPad << " len:" << len << endl;
		char* in = (char*)malloc(sizeof(char)*(len + blockBytes * 2));
		char* out = (char*)malloc(sizeof(char)*(len + blockBytes * 2));
		inputFile.read(in, len);
		inputFile.close();
		bitset<blockSize> rawkey;
		reverseCopy<const char*, bitset<blockSize>*>(key.c_str(), &rawkey, blockBytes);
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
		else if(mode == "cbc" || mode == "CBC")
			status = des_cbc(in, out, numPkt, op, subkeys, IV);
		else if(mode == "cfb" || mode == "CFB")
			status = des_cfb64(in, out, numPkt, op, subkeys, IV);  // just CFB-64
		else
			return false;
		
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
