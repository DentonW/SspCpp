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
  * \file   Hypack.cpp
  * \brief  
  */

#include "pch.h"
#include "Hypack.h"
#include <date/date.h>
#include <SspCpp/SoundSpeed.h>
#include "TimeStruct.h"


namespace ssp::hypack
{
    bool ParseHeader(std::string line, ssp::SCast& cast)
    {
        std::stringstream ss(line);

        // First 3 entries are "FTP NEW 3" in example files
        std::string pos1, pos2, pos3;
        ss >> pos1 >> pos2 >> pos3;
        if (!ss)
            return false;
        
        ss >> cast.lat >> cast.lon;
        if (!ss)
            return false;

        // The remainder of the string has the time and date
        std::string datetime;
        if (!std::getline(ss, datetime))
            return false;

        std::istringstream in(datetime);
        date::sys_seconds tp;
        // In format: hh:mm month/day/year. Note that there's a space at the beginning,
        //  since the string remaining will be of the form " 15:47 08/19/2019".
        in >> date::parse(" %H:%M %m/%d/%Y", tp);
        if (!in)
            return false;  // Error parsing date/time string

        if (!CreateTime(tp, cast.time))
            return false;

        return true;
    }

};  // End namespace ssp::hypack


std::optional<ssp::SCast> ssp::ReadHypack(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile)
    {
        std::cout << "Could not open file " << fileName << "\n";
        return {};
    }

    SCast cast;
    std::vector<SCastEntry>& entries = cast.entries;
    std::string line, headerStr;

    if (!std::getline(inFile, line))
    {
        std::cout << "Could not read " << fileName << "\n";
        return {};
    }

    if (!hypack::ParseHeader(line, cast))
    {
        std::cout << "Could not parse header for " << fileName << "\n";
        return {};
    }

    // Read in and parse the sound speed data
    while (!inFile.eof())
    {
        SCastEntry entry;

        inFile >> entry.depth >> entry.c;
        if (!inFile)
            break;

        entries.push_back(entry);
    }

    cast.desc = "Hypack (.vel)";
    cast.fileName = fileName;

    return cast;
}
