#pragma once
#include <vector>
#include <string>
#include <fstream>

namespace rtd {

	void encryptDataAndSaveToFile(std::string filename, std::vector<char>& bytes);

}