/*
	-- Lzw.h --

	Performs Lzw decompression to stream of compressed 
	ASCII, and sends plaintext into specified output stream.

	Example usage:

		std::ifstream input("infile.z", std::ifstream::binary);
		const uint32_t initial_dict_size = 256;
		const uint32_t max_code_width = 12;

		Lzw lzw(initial_dict_size, max_code_width);
		lzw.decompress(input, std::cout);

	where 'input' and 'std::cout' can be replaced by any istream& 
	and ostream&, respectively.

*/

#ifndef LZW_H
#define LZW_H

#include <unordered_map>
#include <cstdint>
#include <string>
#include <iostream>
#include <array>


class Lzw
{
public:
	/* Public Methods */
	Lzw(uint32_t _initial_dict_size, uint32_t _max_code_width);
	void decompress(std::istream& input, std::ostream& output);
	
private:
	/* Private Methods */
	bool read_two_codes(std::istream& input, std::array<uint32_t, 2>& codes) const;
	const std::string decode(uint32_t code, std::unordered_map<uint32_t, std::string>& dict, uint32_t & index, std::string prev) const;
	void insert_into_dict(std::string symbol, uint32_t& index);

	/* Private Attributes */
	std::unordered_map<uint32_t, std::string> dict;
	uint32_t initial_dict_size;
	uint32_t max_code_width;

};

#endif // !LZW_H