#include <stdio.h>
#include <math.h>


void generateT()
{
	double bigInt = 4294967296;
	for(int i = 1; i <= 64; ++i)
	{
		if(i%16 == 1)
			printf("\n");
		unsigned int temp = (unsigned int)fabs(sin(i) * bigInt);
		printf("0x%x, ", temp);
	}
}

int main()
{
	generateT();

return 0;
}
