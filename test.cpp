#include <iostream>
#include <cstring>
#include "des.cpp"
#include "des_parameters.h"
using namespace std;


int main()
{
	bitset<48> k(string("10101010101010101010101010101010101010101010101"));
	cout << k << endl;
	bitset<32> kk = substitution<48, 32, 4, 16, 8>(k, S_BOX);
	cout << kk << endl;
	
	
return 0;
}


