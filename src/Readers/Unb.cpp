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
#include <fmt/format.h>
#include <SspCpp/Cast.h>
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
                throw "Invalid version number";
        }
        catch (std::invalid_argument)
        {
            throw "Invalid version line";
        }

        return;
    }


    bool ParseJulianDay(const std::string& yearStr, const std::string& dayJulianStr, std::string& month, std::string& day)
    {
        int dayJulian, year;

        try
        {
            dayJulian = std::stoi(dayJulianStr);
            if (dayJulian < 0 || dayJulian > 366)
            {
                std::cout << "Invalid Julian date of " << dayJulianStr << "\n";
                return false;
            }

            year = std::stoi(yearStr);
            if (year < 0)
            {
                std::cout << "Invalid year of " << yearStr << "\n";
                return false;
            }

        }
        catch (std::invalid_argument)
        {
            std::cout << "Could not parse Julian date\n";
            return false;
        }

        return true;
    }

    bool ParseDateTime(const std::string& line, SCast& cast)
    {
        date::parse()


        // Example date/time line: "Freitag, 20. Juli 2018 16:54:38"
        //std::regex rgxDateTime("[a-zA-Z]+[,] ([0-9]+)[.] ([a-zA-Z]+) ([0-9]+) ([0-9]+):([0-9]+):([0-9]+)");
        std::regex rgxDateTime("([0-9]+) ([0-9]+) ([0-9]+):([0-9]+):([0-9]+)");
        std::smatch match;
        std::regex_search(line, match, rgxDateTime);
        if (match.size() != 6)
            return false;

        auto year = match[1];
        auto dayJulian = match[2];
        auto hour = match[3];
        auto min = match[4];
        auto sec = match[5];

        if (!ParseJulianDay)
            return false;

        if (!CreateTime(year, month, day, hour, min, sec, cast.time))
            return false;

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

        if (!std::getline(inFile, line))  // Version line (usually with comments after #)
            throw "Line read failure";

        if (!unb::ParseDateTime(line))
    }
    catch (std::string err)
    {
        std::cout << "Error reading Unb file (" << fileName << "): " << err << "\n";
        return {};
    }

    //cast.lat = 0;
    //cast.lon = 0;
    cast.desc = "University of New Brunswick";
    cast.fileName = fileName;

    return cast;
}
