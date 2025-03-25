#include "Encrypter.h"
#include "EncryptConstants.h"

using namespace std;
using namespace rtd;

void rtd::encryptDataAndSaveToFile(string filename, vector<char>& bytes)
{
	ofstream ofile(filename, ios::binary | ios::trunc | ios::out);

	string fileSizeStr = to_string(bytes.size() + size_string_lenght);

	for (int pos = 0; pos < size_string_lenght - fileSizeStr.size(); ++pos)
	{
		char zero = '0' + size_string_hash;
		ofile.write(reinterpret_cast<char*>(&zero), 1);
	}

	for (int pos = 0; pos < fileSizeStr.size(); ++pos)
	{
		char c = fileSizeStr[pos] + size_string_hash;
		ofile.write(
			reinterpret_cast<char*>(&c),
			1
		);
	}

	for (int pos = 0; pos < bytes.size(); ++pos)
	{
		bytes[pos] += enc_hash;
		ofile.write(reinterpret_cast<char*>(&bytes[pos]), sizeof(char));
	}
}