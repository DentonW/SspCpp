/*
 * MIT License
 *
 * Copyright (c) 2021 Denton Woods
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

 /*!
  * \file   Unb.cpp
  * \brief
  */

#include "../pch.h"
#include "Unb.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <date/date.h>
#include <fmt/format.h>
#include <SspCpp/Cast.h>
#include "../LatLong.h"
#include "../StringUtilities.h"
#include "../TimeStruct.h"


namespace ssp::unb
{
    // This version will throw exceptions!
    void ParseVersion(std::ifstream& inFile)
    {
        std::string line;
        if (!std::getline(inFile, line))  // Version line (usually with comments after #)
            throw "Line read failure";

        auto strings = SplitString(line);
        if (strings.size() == 0)
            throw "Invalid version in header";

        try
        {
            int ver = std::stoi(strings[0]);
            if (ver != 2)
                throw "Invalid version number (should be 2)";
        }
        catch (std::invalid_argument)
        {
            throw "Invalid version line";
        }

        return;
    }


    bool ParseDateTime(std::ifstream& inFile, SCast& cast)
    {
        std::string line;
        if (!std::getline(inFile, line))  // Date/time line (usually with comments after #)
            return false;

        std::istringstream in(line);
        date::sys_seconds tp;
        // In format: year julian-day hh:mm:ss
        in >> date::parse("%Y %j %T", tp);
        if (!in)
            return false;  // Error parsing date/time string

        if (!CreateTime(tp, cast.time))
            return false;

        // The next line has a date/time for logging, but the two examples we have are filled with zeros
        if (!std::getline(inFile, line))
            return false;

        return true;
    }


    bool ParseLatLon(std::ifstream& inFile, SCast& cast)
    {
        std::string line;
        if (!std::getline(inFile, line))  // Lat/lon line
            return false;

        std::stringstream ss(line);
        ss >> cast.lat;
        if (!ss)
            return false;
        ss >> cast.lon;
        if (!ss)
            return false;

        if (!CheckLatLon(cast.lat, cast.lon))
            return false;

        // The next line has a lat/lon for logging, but the two examples we have are filled with zeros
        if (!std::getline(inFile, line))
            return false;

        return true;
    }


    // Warning: This function can throw!
    bool ReadEntries(std::ifstream& inFile, SCast& cast) noexcept(false)
    {
        std::string line;

        if (!std::getline(inFile, line))
            throw "Could not read line";
        int num;
        std::stringstream ss(line);
        ss >> num;
        if (!ss || num < 1)
            throw "Could not read number of entries";

        cast.entries.reserve(num);

        // Skip the next 10 lines, which are for future use
        for (int m = 0; m < 10; ++m)
        {
            if (!std::getline(inFile, line))
                throw "Could not read line";
        }

        for (int n = 0; n < num; ++n)
        {
            if (!std::getline(inFile, line))
                throw fmt::format("Could not read line #{}", n + 16);

            auto strings = SplitString(line);
            if (strings.size() == 0 || strings.size() < 7)
                throw fmt::format("Invalid line #{}", n + 16);

            try
            {
                int entryNum = std::stoi(strings[0]);
                if (entryNum != n+1)  // 1-indexed
                    throw fmt::format("Invalid entry number on line #{}", n + 16);

                SCastEntry entry;
                entry.depth = std::stod(strings[1]);
                entry.c = std::stod(strings[2]);
                entry.temp = std::stod(strings[3]);
                entry.salinity = std::stod(strings[4]);

                cast.entries.push_back(entry);
            }
            catch (std::invalid_argument)
            {
                throw fmt::format("Line #{} could not be parsed", n + 16);
            }
        }

        return true;
    }

}  // End namespace ssp::unb


std::optional<ssp::SCast> ssp::ReadUnb(const std::string& fileName)
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

    try
    {
        unb::ParseVersion(inFile);

        if (!unb::ParseDateTime(inFile, cast))
            throw "Could not parse date/time";

        if (!unb::ParseLatLon(inFile, cast))
            throw "Could not parse latitude/longitude";

        if (!unb::ReadEntries(inFile, cast))
            throw "Could not read entries";
    }
    catch (std::string err)
    {
        //std::cout << "Error reading Unb file (" << fileName << "): " << err << "\n";
        fmt::print("Error reading Unb file ({}): {}\n", fileName, err);
        return {};
    }

    cast.desc = "University of New Brunswick";
    cast.fileName = fileName;

    return cast;
}
