#include <stdexcept>
#include "FileReader.h"

using rtd::FileReader;
using std::runtime_error;
using std::string;

FileReader::FileReader(string filename)
{
    m_filename = filename;
    m_file.open(m_filename);

    if(!m_file.is_open())
        throw runtime_error("Ocurrió un error al intentar leer el archivo '" + m_filename + "'");
}

bool FileReader::readFile()
{
    Json::Reader reader;
    return reader.parse(m_file, m_json);
}

void FileReader::closeFile()
{
    m_file.close();
}