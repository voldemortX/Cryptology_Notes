void reverse(char *s, int n)
{
	// wtf
	int i;
	char t;
	for(i=0; i<n/2; i++)
	{
		t=*(s+i);
		*(s+i)=*(s+n-i-1);
		*(s+n-i-1)=t;
	}
} 

template<typename t1, typename t2>
void reverse_copy(t1 in, t2 out, int n)
{
	char* temp = (char*)malloc(n * sizeof(char));
	memcpy(temp, in, n);
	reverse(temp, n);
	memcpy(out, temp, n);
}

// Read&Write bits as uint32 --> len <= 24
// Start = bits(8x) + offset(0-7)
unsigned int read_bits(char* bits, int offset, int len)
{
	unsigned int res = 0;
	if(len == 0)
		return res;
		
	res = *((unsigned int*)bits);  // Copy 32 bits
	reverse_copy<unsigned int*, unsigned int*>(&res, &res, 4);  // Reverse byte-wise
	res <<= offset;  // Offsetted by 'offset' bits
	res >>= (32 - len);  // Get only 'len' bits
	return res;
}

void write_bits(char* bits, int offset, int len, unsigned int content)
{
	if(len == 0)
		return;
		
	// Clear the target
	unsigned int* target = (unsigned int*)bits;
	reverse_copy<unsigned int*, unsigned int*>(target, target, 4);
	unsigned int cleaner = 0xFFFFFFFF;
	cleaner <<= (32 - len);
	cleaner >>= offset;
	cleaner = ~cleaner;
	(*target) &= cleaner;
	
	// Then it's like the reverse of read_bits()
	content <<= (32 - len);
	content >>= offset;
	(*target) |= content;
	
	// Reverse back
	reverse_copy<unsigned int*, unsigned int*>(target, target, 4);
}

