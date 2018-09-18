#include <iostream>
#include <bitset>
#include "des_parameters.h"
using namespace std;


int main()
{
	bitset<10> k;
	bitset<>* p = k;
	cout << p[2] << endl;
	
return 0;
}


