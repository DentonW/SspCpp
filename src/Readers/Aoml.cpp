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
  * \file   Asvp.cpp
  * \brief  
  */

#include "pch.h"
#include "Aoml.h"
#include <fstream>
#include <regex>
#include <fmt/format.h>
#include <SspCpp/SoundSpeed.h>
#include "StringUtilities.h"
#include "TimeStruct.h"


namespace ssp::aoml
{
    //! Gets everything past the vertical bar character on each header line. Usually has one space after the bar.
    std::string GetLineValue(std::string line)
    {
        std::regex rgx("\\|\\s*(.*)");
        std::smatch match;
        std::regex_search(line, match, rgx);
        if (match.size() != 2)
        {
            std::cout << "Could not parse line\n";
            return "";
        }

        return match[1];
    }

    bool ParseLatitude(std::string line, ssp::SCast& cast)
    {
        auto strings = SplitString(line);
        if (strings.size() != 3)
        {
            std::cout << "Latitude string incorrect\n";
            return false;
        }

        try
        {
            double deg = std::stod(strings[0]);
            double minSec = std::stod(strings[1]);  // Minutes plus fractional minutes (e.g., 11.01)
            auto dir = strings[2];  // N or S
            cast.lat = deg + minSec / 60;
            if (dir != "N" && dir != "S")
            {
                std::cout << "Invalid latitude direction\n";
                return false;
            }
            if (dir == "S")
                cast.lat = -cast.lat;
        }
        catch (std::invalid_argument)
        {
            std::cout << "Invalid latitude string\n";
            return false;
        }

        return true;
    }

    bool ParseLongitude(std::string line, ssp::SCast& cast)
    {
        auto strings = SplitString(line);
        if (strings.size() != 3)
        {
            std::cout << "Longitude string incorrect\n";
            return false;
        }

        try
        {
            double deg = std::stod(strings[0]);
            double minSec = std::stod(strings[1]);  // Minutes plus fractional minutes (e.g., 11.01)
            auto dir = strings[2];  // W or E
            cast.lon = deg + minSec / 60;
            if (dir != "W" && dir != "E")
            {
                std::cout << "Invalid longitude direction\n";
                return false;
            }
            if (dir == "W")
                cast.lon = -cast.lon;
        }
        catch (std::invalid_argument)
        {
            std::cout << "Invalid longitude string\n";
            return false;
        }

        return true;
    }

    void SetDate(std::string dateStr, ssp::SCast& cast)
    {

        return;
    }
}

using namespace ssp::aoml;


std::optional<ssp::SCast> ssp::ReadAoml(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile)
    {
        std::cout << "Could not open file " << fileName << "\n";
        return {};
    }

    SCast cast;
    std::vector<SCastEntry>& entries = cast.entries;
    std::string line, dateStr;
    bool bLatSet = false, bLongSet = false;

    while (!inFile.eof())
    {
        if (!std::getline(inFile, line))  // Header
            break;
        if (line.size() == 0)
            break;

        if (StartsWith(line, "Latitude"))
        {
            auto latVal = GetLineValue(line);
            if (latVal.size() == 0)
            {
                std::cout << fmt::format("Could not parse latitude line for {}\n", fileName);
                return {};
            }
            if (!ParseLatitude(latVal, cast))
            {
                return {};
            }
            bLatSet = true;
        }
        else if (StartsWith(line, "Longitude"))
        {
            auto lonVal = GetLineValue(line);
            if (lonVal.size() == 0)
            {
                std::cout << fmt::format("Could not parse longitude line for {}\n", fileName);
                return {};
            }
            if (!ParseLongitude(lonVal, cast))
            {
                return {};
            }
            bLongSet = true;
        }
        // The following date/time fields look to always be in this order
        else if (StartsWith(line, "Year"))
        {
            dateStr += GetLineValue(line) + " ";
        }
        else if (StartsWith(line, "Month"))
        {
            dateStr += GetLineValue(line) + " ";
        }
        else if (StartsWith(line, "Day"))
        {
            dateStr += GetLineValue(line) + " ";
        }
        else if (StartsWith(line, "Hour"))
        {
            dateStr += GetLineValue(line) + " ";
        }
        else if (StartsWith(line, "Minute"))
        {
            dateStr += GetLineValue(line);
        }

        else if (StartsWith(line, "===="))
        {
            break;  // End of header
        }
    }

    if (!bLatSet || !bLongSet)
        std::cout << fmt::format("Warning: Missing lat/lon data in {}\n", fileName);
    SetDate(dateStr, cast);

    if (!std::getline(inFile, line))  // Unused
        return {};
    if (!std::getline(inFile, line))
        return {};
    auto desc = SplitString(line);
    // The example files I have only have depth and temperature
    if (desc.size() != 2 || desc[0] != "Depth" || desc[1] != "Temperature")
    {
        std::cout << fmt::format("Invalid data types for {}\n", fileName);
        return {};
    }

    // Now read in the data
    while (!inFile.eof())
    {
        double salinity = 35;  // Assuming 35 ppt salinity, since not measured
        if (!std::getline(inFile, line))
            break;

        auto data = SplitString(line);
        if (data.size() != 2)
            break;

        try
        {
            SCastEntry entry;
            entry.depth = std::stod(data[0]);
            entry.temp = std::stod(data[1]);

            entry.pressure = DepthToPressure(entry.depth, cast.lat);
            entry.c = WongZhu(entry.temp, salinity, entry.pressure);

            cast.entries.push_back(entry);
        }
        catch (std::invalid_argument)
        {
            break;
        }
    }


    cast.desc = "AOML AMVER-SEAS XBT (.txt)";
    cast.fileName = fileName;

    return cast;
}
