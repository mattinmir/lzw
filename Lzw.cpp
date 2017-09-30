#include "Lzw.h"
#include <unordered_map>
#include <cstdint>
#include <string>
#include <iostream>
#include <array>
#include <bitset>

Lzw::Lzw(uint32_t _initial_dict_size, uint32_t _max_code_width)
:	initial_dict_size(_initial_dict_size),
	max_code_width(_max_code_width)
{}


void Lzw::decompress(std::istream & input, std::ostream & output)
{
	std::array<uint32_t, 2> codes;
	bool end_of_file;
	uint32_t dict_index = this->initial_dict_size;
	std::string curr;
	std::string prev;
	std::string new_symbol;

	// If file is empty, do nothing
	if (input.peek() && input.eof())
	{
		return;
	}

	// If there were only two chars to read, eof flag is set true
	end_of_file = this->read_two_codes(input, codes);
	
	// If file has only one code, output it and return
	if (end_of_file)
	{
		output << std::string(1, codes[0]);
		return;
	}

	// If file has more than one code, begin decompression
	else
	{
	
		prev = std::string(1, codes[0]);
		std::cout << prev;

		curr = this->decode(codes[1], this->dict, dict_index, prev);
		output << curr;

		new_symbol = prev + curr[0];
		this->insert_into_dict(new_symbol, dict_index);

		prev = curr;

		do
		{
			// If there are zero 8-bit characters to read
			if (input.peek() && input.eof())
			{
				end_of_file = true;
			}

			// If there are (at least) two or three more 8-bit characters to read
			// (One 8-bit character is impossible under specifications)
			else
			{
				end_of_file = this->read_two_codes(input, codes);

				curr = this->decode(codes[0], this->dict, dict_index, prev);

				output << curr;

				new_symbol = prev + curr[0];
				this->insert_into_dict(new_symbol, dict_index);

				prev = curr;

				// If the end of the file was reached with only one code left to read,
				// eof will be set to prevent us processing codes[1]
				if (!end_of_file)
				{
					curr = this->decode(codes[1], this->dict, dict_index, prev);
					output << curr;

					new_symbol = prev + curr[0];
					this->insert_into_dict(new_symbol, dict_index);

					prev = curr;
				}
			}
		} while (!end_of_file);
	}
}

bool Lzw::read_two_codes(std::istream& input, std::array<uint32_t, 2>& codes) const
{
	// Read three 8-bit chars
	char chars[3];
	for (uint32_t i = 0; i < 3; i++)
	{
		input.get(chars[i]);
	}

	// Split the three 8-bit chars into two 12-bit values
	std::string codes_str[2];
	codes_str[0] = std::bitset<8>(chars[0]).to_string() + std::bitset<8>(chars[1]).to_string().substr(0, 4);
	codes_str[1] = std::bitset<8>(chars[1]).to_string().substr(4, 4) + std::bitset<8>(chars[2]).to_string();

	codes[0] = std::stoul(codes_str[0], nullptr, 2);
	codes[1] = std::stoul(codes_str[1], nullptr, 2);
	
	// If eof not set, then full 24 bits were read
	// If eof set, only 16 bits were read, and codes[1] should contain no info
	return input.eof();
}

const std::string Lzw::decode(uint32_t code, std::unordered_map<uint32_t, std::string>& dict, uint32_t & index, std::string prev) const
{
	// If code is an ascii char
	if (code < 256)
	{
		return std::string(1, code);
	}
	// If the code is in the dictionary
	else if (dict.find(code) != dict.end())
	{
		return dict.at(code);
	}
	// If code not in dictionary
	else
	{
		return prev;
	}
}

void Lzw::insert_into_dict(std::string symbol, uint32_t & index)
{
	// If dict not full
	if (index < std::pow(2, this->max_code_width))
	{
		this->dict[index++] = symbol;
	}
	// If dict full
	else
	{
		index = this->initial_dict_size;
		this->dict = std::unordered_map<uint32_t, std::string>();

		this->dict[index++] = symbol;
	}
}
