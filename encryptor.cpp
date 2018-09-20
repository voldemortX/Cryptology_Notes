#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include "des.cpp"
#include "des_parameters.h"
using namespace std;


bool des(string inputFilename, string outputFilename, string key, string mode, string op="encrypt", string IV = "")
{
	bool status = false;
	ifstream inputFile(inputFilename, ios::binary);
	if((inputFile != NULL) && (key.size() >= 8))
	{
		
	}
	
	return status;
}


