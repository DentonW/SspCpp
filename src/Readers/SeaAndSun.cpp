#include "pch.h"
#include "SeaAndSun.h"
#include <fstream>
#include "StringUtilities.h"
#include "TimeStruct.h"


std::optional<ssp::SCast> ssp::ReadSeaAndSun(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile)
    {
        std::cout << "Could not open file " << fileName << "\n";
        return {};
    }

    SCast cast;
    std::vector<SCastEntry>& entries = cast.entries;
    std::string line, dateLine, timeLine;

    int lineNum = 1;
    int numDataLines;

    try
    {
        std::string line;
        bool bFound = false;

        while (std::getline(inFile, line))
        {
            lineNum++;

            if (line.compare(0, 7, "Lines :") == 0)  // Line starts with "Lines :"
            {
                std::stringstream ss(line);
                std::string s1, s2, numLinesStr;
                ss >> s1 >> s2 >> numLinesStr;

                try
                {
                    numDataLines = std::stoi(numLinesStr);
                }
                catch (std::invalid_argument)
                {
                    throw "Invalid number of entries string";
                }

                bFound = true;
                break;
            }
        }
    }
    catch (std::string err)
    {
        std::cout << "Error reading Sea & Sun file (" << fileName << "): " << err << "\n";
        return {};
    }

    cast.lat = 0;
    cast.lon = 0;
    cast.desc = "Sea & Sun (.tob)";
    cast.fileName = fileName;

    return cast;
}
