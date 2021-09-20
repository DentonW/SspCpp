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
  * \file   Oceanscience.cpp
  * \brief  
  */

#include "pch.h"
#include "Oceanscience.h"
#include <date/date.h>
#include <SspCpp/SoundSpeed.h>
#include <SspCpp/LatLong.h>
#include "TimeStruct.h"


std::optional<ssp::SCast> ssp::ReadOceanscience(const std::string& fileName)
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
    int lineNum = 0;

    std::optional<double> lat, lon;

    // Read in and parse the sound speed data
    while (!inFile.eof() && std::getline(inFile, line))
    {
        ++lineNum;

        // Blank lines and comment lines (starting with *) currently just ignored
        if (line.size() == 0)
            continue;
        if (line[0] == '*')
        {
            std::cout << "Need to check for date/time and lat/lon\n";
            continue;
        }


        if (!lat || !lon)
        {
            lat = 0;  lon = 0;  // Assuming latitude/longitude of 0/0 if not specified
        }

        std::stringstream ss(line);
        int n;  // Entry number 
        double cond, temp, pres;  // Conductivity, temperature, pressure
        ss >> n >> cond >> temp >> pres;
        if (!ss)
        {
            fmt::print("Issue reading line {} of {}", lineNum, fileName);
            return {};
        }

        if (cond < 0 || temp < -2 || pres < 0)
        {
            fmt::print("Invalid parameter on line {} of {}", lineNum, fileName);
            return {};
        }

        double salinity = ConductivityToSalinity(cond, pres, temp);
        if (salinity < 0)
        {
            fmt::print("Invalid conductivity on line {} of {}", lineNum, fileName);
            return {};
        }

        pres /= 10;  // Convert to bars
        double c = WongZhu(temp, salinity, pres);

        SCastEntry entry;
        entry.c = c;
        entry.depth = Depth(pres, *lat);
        entry.pressure = pres;

        entries.push_back(entry);
    }

    cast.desc = "Oceanscience (.asc)";
    cast.fileName = fileName;

    return cast;
}
