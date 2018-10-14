#include <iostream>
#include <string>
#include <string.h>
#include <bitset>
#include <fstream>
#include <memory.h>
#include "md5_parameters.h"
#include "md5.cpp"
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

bool md5(string inputFilename, string outputFilename)
{
	bool status = false;
	ifstream inputFile(inputFilename, ios::binary);
	if(inputFile != NULL)
	{
		inputFile.seekg(0, ios::end);
		unsigned long long e = inputFile.tellg();
		inputFile.seekg(0, ios::beg);
		unsigned long long s = inputFile.tellg();
		unsigned long long len = e - s;
		unsigned long long numPad = blockBytes - (len + lenPadBytes) % blockBytes;
		unsigned long long paddedLen = len + lenPadBytes + numPad; 
		unsigned long long numPkt = paddedLen / blockBytes;
		cout << "numpad: " << numPad << " len:" << len << endl;
		char* in = (char*)malloc(sizeof(char)*(paddedLen));
		char* out = (char*)malloc(sizeof(char)*(outBytes));
		inputFile.read(in, len);
		cout << "in: " << in << endl;
		inputFile.close();
		
		// padding
		in[len] = 128;  // 10000000
		for(int i = 1; i < numPad; ++i)
		{
			in[len + i] = 0;  // 0000000...	
			cout << in[len + i] << endl;
		}
		bitset<lenPadBytes * 8> lenBits(len);  // length
		cout << "lenBits: " << lenBits << endl;
		reverseCopy<bitset<lenPadBytes * 8>*, char*>(&lenBits, in + paddedLen - lenPadBytes, lenPadBytes);
		cout << "in_padded: " << (int)in[50] << endl;
		
		// md5
		unsigned int vector[4] = {A, B, C, D};
		unsigned int pkt[16];
		for(int i = 0; i < numPkt; ++i)
		{
			for(int j = 0; j < (blockBytes * 8) / wordLen; ++j)
			{
				bitset<wordLen> temp;
				reverseCopy<char*, bitset<wordLen>*>(in + i * blockBytes + j * wordLen / 8, &temp, wordLen / 8);
				pkt[j] = temp.to_ulong();
			}
			transform(vector, pkt);
		}
		
		for(int i = 0; i < numParts; ++i)
		{
			bitset<wordLen> temp(vector[i]);
			reverseCopy<bitset<wordLen>*, char*>(&temp, out + i * wordLen / 8, wordLen / 8);
		}
		bitset<128> outB;
		reverseCopy<char*, bitset<128>*>(out, &outB, 16);
		cout << outB << endl;

		ofstream outputFile(outputFilename, ios::binary);
		outputFile.seekp(0,ios::beg);
		outputFile.write(out, outBytes);
		outputFile.close();
		status = true;
	}

	return status;
}
