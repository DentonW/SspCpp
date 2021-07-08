#include "pch.h"
#include "SeaBird.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <date/date.h>
#include <fmt/format.h>
#include "Cast.h"
#include "TimeStruct.h"


namespace ssp
{

std::optional<SCast> ReadSeaBirdCnv(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile)
    {
        std::cout << "Could not open file " << fileName << "\n";
        return {};
    }

    SCast cast;
    std::vector<SCastEntry>& entries = cast.entries;




    cast.desc = "SeaBird";
    cast.fileName = fileName;

    return cast;
}


bool ParseTsvHeader(std::string line, SCast& cast)
{
    // Header string format: "## DATE:yyyy-mm-ddThh:mm:ss\tLATITUDE:xx.xx\tLONGITUDE:xx.xx"
    std::regex rgx("## DATE:([0-9]+)-([0-9]+)-([0-9]+)T([0-9])+:([0-9])+:([0-9]+).*LATITUDE:(.?[0-9]+.[0-9]+).*LONGITUDE:(.?[0-9]+.[0-9]+)");
    std::smatch matches;
    std::regex_search(line, matches, rgx);

    if (matches.size() != 9)
    {
        std::cout << "Sea-Bird .tsv has incorrect header\n";
        return false;
    }

    cast.lat = std::stod(matches[7]);
    cast.lon = std::stod(matches[8]);

    std::cout << matches.size() << "\n";
    for (size_t n = 0; n < matches.size(); ++n)
        std::cout << matches[n].str() << "\n";

    if (!CreateTime(matches[1], matches[2], matches[3], matches[4], matches[5], matches[6], cast.time))
        return false;

    return true;
}


std::optional<SCast> ReadSeaBirdTsv(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile)
    {
        std::cout << "Could not open file " << fileName << "\n";
        return {};
    }

    SCast cast;
    std::vector<SCastEntry>& entries = cast.entries;
    std::string line;

    try
    {
        if (!std::getline(inFile, line))
            throw "Line read failure";

        if (!ParseTsvHeader(line, cast))
            throw "Could not parse header line";

        while (std::getline(inFile, line))
        {
            if (line.size() == 0)
                break;

            // The depth and sound speed are required fields...
            std::stringstream ss(line);
            SCastEntry entry;
            ss >> entry.depth >> entry.c;
            if (ss.fail())
                throw fmt::format("Incomplete entry for line #{}", entries.size() + 2);

            // Temperature and salinity are optional fields (may not be present in the file but have to be present together)
            ss >> entry.temp >> entry.salinity;
            if (ss.fail())
            {
                entry.temp = 0;
                entry.salinity = 0;
            }

            entry.absorp = 0;

            entries.push_back(entry);
        }
    }
    catch (std::string err)
    {
        std::cout << "Error reading Sea-Bird file (" << fileName << "): " << err << "\n";
        return {};
    }

    cast.lat = 0;
    cast.lon = 0;
    cast.desc = "Sea-Bird";
    cast.fileName = fileName;

    return cast;
}


std::optional<SCast> ReadSeaBird(const std::string& fileName)
{
    /// @todo: Add .csv format
    return ReadSeaBirdTsv(fileName);
}

};  // End namespace ssp
