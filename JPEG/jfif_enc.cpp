/*
Suppose we have us here a 
normal sequencial baseline jpeg form the 20th century,
which has 3 components: YCrCb -_-
*/

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "bit_ops.cpp"
using namespace std;

const char MARKER = 0xFF;
const char HUFFMAN = 0xC4;
const char END = 0xD9;  // If found early, then this jpeg is probably 21st century 
const char AC = 0x10;  // Either DC or AC
const char BASELINE_DCT = 0xC0;
const char SOS = 0xDA;  // Start of scan
const char MAX_LEN = 16;  // 16 possible huffman code lengths

typedef struct huffmanS
{
	unsigned int code;
	int len;  // !!! Gotta know it(e.g. 0 and 00)
	char byte;
	huffmanS(unsigned int code_in, int len_in, char byte_in)
	{
		code = code_in;
		len = len_in;
		byte = byte_in;
	}
}huffman;

typedef struct globalS
{
	// Sampling parameters
	vector<int> y;
	vector<int> x;
	vector<int> h;
	vector<int> v;
	
	// Which huffman table are they using
	vector<int> ac_num;
	vector<int> dc_num;
}global;


char* read_file(string filename, int& len)
{
	// Read file to char*
	char* str;
	ifstream input_file(filename, ios::binary);
	if(input_file != NULL)
	{
		input_file.seekg(0, ios::end);
		int e = input_file.tellg();
		input_file.seekg(0, ios::beg);
		int s = input_file.tellg();
		len = e - s;
		str = (char*)malloc(sizeof(char)*(len));
		input_file.read(str, len);
		input_file.close();
	}
	
	return str;
}

void write_file(string filename, char* str, int len)
{
	// Write char* to file
	ofstream output_file(filename, ios::binary);
	output_file.seekp(0, ios::beg);
	output_file.write(str, len);
	output_file.close();
}

int parse_one_huffman_table(vector<huffman>& codes, char* input)
{
	// Find lengths
	int i = 0;
	vector<int> lens(MAX_LEN);
	for(; i < MAX_LEN; ++i)
		lens[i] = *(input + i);
		
	// Calculate codes
	unsigned int base = 0;
	for(int k = 0; k < MAX_LEN; ++k)
	{
		//cout << k << ": " << lens[k] << endl;
		while(lens[k]--)
		{
			char tc = *(input + i);
			huffman temp(base, k + 1, tc);
			codes.push_back(temp);
			++i;
			if(tc == MARKER)
				++i;
			++base;
		}
		
		base <<= 1;
	}
	
	// Offseted by i
	return i;
}

int parse_huffman_tables(vector<vector<huffman>>& huffmans_dc, vector<vector<huffman>>& huffmans_ac, char* input)
{
	// Say 4 tables(default)
	// And suppose table numbers in ascending order
	int i = 0;
	char temp;
	int count = 4;
	while(count)
	{
		if(*(input + i) == MARKER)  // Special judge
		{
			temp = *(input + i + 1);
			if(temp == END)
				break;
			else if(temp == HUFFMAN)
			{
				--count;
				temp = *(input + i + 4);
				vector<huffman> th;
				i += parse_one_huffman_table(th, (input + i + 5));
				if(temp & AC)  // AC
					huffmans_ac.push_back(th);
				else  // DC
					huffmans_dc.push_back(th);
			}
		}
		
		++i;
	}
	
	// Offseted by i
	return i;
}

int parse_global(char* input, global& globals)
{
	int i = 0;
	char temp;
	while(true)
	{
		if(*(input + i) == MARKER)  // Special judge
		{
			temp = *(input + i + 1);
			if(temp == END)
				break;
				
			else if(temp == BASELINE_DCT)
			{
				// Parameters
				unsigned int y = *(input + i + 5);
				y <<= 8;
				y += (unsigned char)*(input + i + 6);
				unsigned int x = *(input + i + 7);
				x <<= 8;
				x += (unsigned char)*(input + i + 8);
				int n = *(input + i + 9);
				
				i += 10;
				int v_max = 0, h_max = 0;
				for(int j = 0; j < n; ++j)
				{
					temp = *(input + i + 1);
					int h = (temp >> 4);
					int v = ((unsigned char)(temp << 4) >> 4);
					h_max = max(h, h_max);
					v_max = max(v, v_max);
					globals.h.push_back(h);
					globals.v.push_back(v);
					i += 3;
				}
				
				cout << "Resolution: "<< x << "x" << y << endl;
				
				// Calculate resolutions(floor & 8x)
				for(int j = 0; j < n; ++j)
				{
					int tx = x * globals.h[j] / h_max;
					if((x * globals.h[j]) % h_max)
						++tx;
					if(tx % 8)
						tx += (8 - tx % 8);
					globals.x.push_back(tx);	
					
					int ty = y * globals.v[j] / v_max;
					if((y * globals.v[j]) % v_max)
						++ty;
					if(ty % 8)
						ty += (8 - ty % 8);
					globals.y.push_back(ty);
				}
				
				break;
			}
		}
		
		++i;
	}
	
	return i;
}

char huffman_decode(char* input, int& bit_offset, vector<huffman> t)
{
	// Decode one 
	int len = 1;
	unsigned int temp;
	while(len <= 16)
	{
		temp = read_bits(input, bit_offset, len);
		for(int i = 0; i < t.size(); ++i)
		{
			if(t[i].len > len)
				break;
			else if(t[i].len == len && t[i].code == temp)
			{
				bit_offset += len;
				return t[i].byte;	
			}
		}
		
		++len;
	}
	
	cout << "Can't decode!" << endl;
	return 0xFF;
}

void process_dc(char* input, vector<vector<huffman>>& huffmans_dc, vector<vector<huffman>>& huffmans_ac, global& globals, char* key_stream)
{
	// Say only one scan(interleaved)
	// Say components are in ascending order
	// Say we yet don't consider 0xFF00, byte alignment and all that shits
	// It can still work with the sample image 
	int i = 0;
	char temp;
	while(true)
	{
		if(*(input + i) == MARKER)  // Special judge
		{
			temp = *(input + i + 1);
			if(temp == END)
				break;
			else if(temp == SOS)  // Scan parameters
			{
				int n = *(input + i + 4);
				i += 5;
				for(int j = 0; j < n; ++j)
				{
					temp = *(input + i + 1);
					int dc = (temp >> 4);
					int ac = ((unsigned char)(temp << 4) >> 4);
					globals.dc_num.push_back(dc);
					globals.ac_num.push_back(ac);
					i += 2;
				}
				
				i += 4;
				break;
			}
			
		}
		
		++i;
	}
	
	// Calc scan ranges
	int n_x = globals.x[0];
	int n_y = globals.y[0];
	for(int k = 1; k < globals.x.size(); ++k)
		n_x = min(n_x, globals.x[k]);
	for(int k = 1; k < globals.y.size(); ++k)
		n_y = min(n_y, globals.y[k]);
	n_x /= 8;
	n_y /= 8;
		
	// Start the scan
	input += i;
	int bits_i = 0;  // Bit-wise position
	int keys_i = 0;  // Bit-wise position
	char tc;
	unsigned int tu;
	for(int x = 0; x < n_x; ++x)  // rows
		for(int y = 0; y < n_y; ++y)  // columns
			for(int k = 0; k < globals.h.size(); ++k)  // components
				for(int i_h = 0; i_h < globals.h[k]; ++i_h)  // rows inside a mcu
					for(int i_v = 0; i_v < globals.v[k]; ++i_v)  // columns inside a mcu
					{
						// One 8x8
						// DC encrypt
						int offset = bits_i % 8;
						tc = huffman_decode(input + bits_i / 8, offset, huffmans_dc[globals.dc_num[k]]);
						bits_i += (offset - bits_i % 8);
						tu = read_bits(input + bits_i / 8, bits_i % 8, (int)tc);
						tu ^= read_bits(key_stream + keys_i / 8, keys_i % 8, (int)tc);
						write_bits(input + bits_i / 8, bits_i % 8, (int)tc, tu);
						bits_i += (int)tc;
						keys_i += (int)tc;
						
						// AC pass(Gotta count it to be sure)
						for(int ti = 0; ti < 63;)
						{
							offset = bits_i % 8;
							tc = huffman_decode(input + bits_i / 8, offset, huffmans_ac[globals.ac_num[k]]);
							bits_i += (offset - bits_i % 8);
							if(tc == 0)  // End of block(unit/8x8/whatever)
								break;
							bits_i += (tc & 0x0F);
	                        ti += ((tc >> 4) & 0x0F);  // Better than changing into unsigned...
						}
                        
					}

}

char* generate_key_stream(string key, int len)
{
	// Generate xor encryption key stream
	string key_stream;
	while(key_stream.size() < len)
		key_stream += key;
	
	char* str = (char*)malloc(sizeof(char)*(len + key.size()));
	strcpy(str, key_stream.c_str());
	return str;
}

char* generate_pseudo_random_stream(string key, int len)
{
	// Better randomness
	// Seed from first 32 bits of key
	char* str = (char*)malloc(sizeof(char)*key.size());
	strcpy(str, key.c_str());
	unsigned int seed = 0;
	for(int i = 0; i < 8 && i < key.size(); ++i)
	{
		if(i > 0)
			seed << 4;
		seed += str[i]; 
	}
	srand(seed);
	
	// Generate stream
	str = (char*)malloc(sizeof(char)*len);
	for(int i = 0; i < len; ++i)
		str[i] = (rand() % 256 - 128);
		
	return str;
}

void encrypt(string input_filename, string output_filename, string key)
{
	// Open file
	int len;
	char* input = read_file(input_filename, len);
	
	// Get key stream
	//char* key_stream = generate_key_stream(key, len);
	char* key_stream = generate_pseudo_random_stream(key, len); 
	
	// Parse global parameters before FFC4s
	global globals;
	int i = parse_global(input, globals);
	
	// Get huffman tables
	vector<vector<huffman>> dc;
	vector<vector<huffman>> ac;
	i = parse_huffman_tables(dc, ac, input + i);
	
	// Find DCs & Change them
	process_dc(input + i, dc, ac, globals, key_stream);
	
	// Write file
	write_file(output_filename, input, len);
}

int main()
{
	// Interface
	string input_filename;
	cout << "Input image name: ";
	cin >> input_filename;
	cout << "Encrypt(0) or decrypt(1)? ";
	bool choice;
	cin >> choice;
	string output_filename = input_filename.substr(0, input_filename.size() - 4);
	if(choice)
		output_filename += "_decrypted.jpg";
	else
		output_filename += "_encrypted.jpg";
	string key;
	cout << "Input key: ";
	cin >> key;
	
	encrypt(input_filename, output_filename, key);
	cout << "\nDone! Please check " + output_filename << endl;
	
	return 0;
} 
