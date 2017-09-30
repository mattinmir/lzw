#include <iostream>
#include <fstream>
#include <cstdint>
#include "Lzw.h"

int main(int argc, const char* argv[])
{

	if (argc < 2) 
	{
		std::cerr << "Usage: " << argv[0] << "<input_file>\n";
		return EXIT_FAILURE;
	}

	std::ifstream input(argv[1], std::ifstream::binary);
	if (!input.is_open())
	{
		std::cerr << "Could not open file\n";
		return EXIT_FAILURE;
	}

	const uint32_t initial_dict_size = 256;
	const uint32_t max_code_width = 12;

	Lzw lzw(initial_dict_size, max_code_width);
	lzw.decompress(input, std::cout);
}