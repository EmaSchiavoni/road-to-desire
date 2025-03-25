#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "EncryptConstants.h"

namespace rtd {

	inline std::vector<char> getDecryptedFileBytes(std::string filename)
	{
		std::vector<char> buffer;
		std::ifstream ifile(filename, std::ios::binary | std::ios::ate | std::ios::in);
		int file_size = ifile.tellg();
		ifile.seekg(0);

		std::string sizeStr = "";

		for (int pos = 0; pos < rtd::size_string_lenght; ++pos)
		{
			char c;
			ifile.read(reinterpret_cast<char*>(&c), 1);
			sizeStr += (c - rtd::size_string_hash);
		}

		if (std::stoi(sizeStr) != file_size)
			throw std::runtime_error("ERROR: El archivo \"" + filename + "\" está corrupto.");

		for (int pos = 0; pos < file_size - rtd::size_string_lenght; ++pos)
		{
			buffer.push_back(' ');
			ifile.read(reinterpret_cast<char*>(&buffer[pos]), 1);
			buffer[pos] -= rtd::enc_hash;
		}

		return buffer;
	}

}