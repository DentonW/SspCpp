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
  * \file   Sonardyne.cpp
  * \brief
  */

#include "pch.h"
#include "Sonardyne.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fmt/format.h>
#include "Cast.h"
#include "TimeStruct.h"


std::optional<ssp::SCast> ssp::ReadSonardyne(const std::string& fileName)
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
        if (!std::getline(inFile, line))  // Title
            throw "Line read failure";
        if (!std::getline(inFile, dateLine))
            throw "Line read failure";
        if (!std::getline(inFile, timeLine))
            throw "Line read failure";
        if (!std::getline(inFile, line))  // Probe name?
            throw "Line read failure";
        if (!std::getline(inFile, line))  // Comments line
            throw "Line read failure";

        // Parse the date string
        std::replace(dateLine.begin(), dateLine.end(), '/', ' ');
        std::stringstream dateStream(dateLine);
        int month, day, year;
        dateStream >> month >> day >> year;
        if (dateStream.fail())
            throw "Could not parse date";

        // Parse the time string
        std::replace(timeLine.begin(), timeLine.end(), ':', ' ');
        std::stringstream timeStream(timeLine);
        int hour, minute, second;
        timeStream >> hour >> minute >> second;
        if (timeStream.fail())
            throw "Could not parse time";

        cast.time = CreateTime(year, month, day, hour, minute, second);

        while (std::getline(inFile, line))
        {
            if (line.size() == 0)
                break;

            // The depth and sound speed are required fields...
            std::stringstream ss(line);
            SCastEntry entry;
            ss >> entry.depth >> entry.c;
            if (ss.fail())
                throw fmt::format("Incomplete entry for line #{}", entries.size() + 6);

            // Salinity and temperature are optional fields (may not be present in the file)
            ss >> entry.salinity;
            if (ss.fail())
            {
                entry.salinity = 0;
                entry.temp = 0;
            }
            else
            {
                ss >> entry.temp;
                if (ss.fail())
                {
                    entry.temp = 0;
                }
            }

            entries.push_back(entry);
        }
    }
    catch (std::string err)
    {
        std::cout << "Error reading Sonardyne file (" << fileName << "): " << err << "\n";
        return {};
    }

    cast.lat = 0;
    cast.lon = 0;
    cast.desc = "Sonardyne";
    cast.fileName = fileName;

    return cast;
}
