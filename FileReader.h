#pragma once
#include <string>
#include <fstream>
#include <json/json.h>

namespace rtd {

    class FileReader
    {
    protected:
        Json::Value m_json;
        std::ifstream m_file;
        std::string m_filename;

    public:
        FileReader(std::string filename);
        void closeFile();
        bool readFile();
        virtual void processData() = 0;
    };

}