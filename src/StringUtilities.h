#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace ssp
{
    inline std::vector<std::string> SplitString(std::string str)
    {
        // Idea from https://stackoverflow.com/questions/5607589/right-way-to-split-an-stdstring-into-a-vectorstring
        std::stringstream ss(str);
        std::istream_iterator<std::string> beginStream(ss);
        std::istream_iterator<std::string> endStream;
        std::vector<std::string> entries(beginStream, endStream);

        return entries;
    }
};
