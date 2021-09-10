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
  * \file   Simple.cpp
  * \brief  
  */

#include "pch.h"
#include "Simple.h"
#include <array>
#include <fstream>
#include <regex>
#include <fmt/format.h>
#include <SspCpp/SoundSpeed.h>



std::optional<ssp::SCast> ssp::ReadSimple(const std::string& fileName)
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

    while (!inFile.eof())
    {
        ++lineNum;

        if (!std::getline(inFile, line))  // Header
            break;
        if (line.size() == 0)
            break;

        // Can be either an integer or a decimal number. The "(?:[+-]?[0-9]*[.])?" is a non-capture group,
        //  with the last ? indicating that it is optional. The "[0-9]+" at the end indicates that this
        //  must have at least a set of digits.
        std::regex rgx("([+-]?(?:[0-9]*[.])?[0-9]+)");
        std::smatch match;

        std::array<double,2> vals;
        for (int n = 0; n < 2 && std::regex_search(line, match, rgx); ++n)
        {
            if (match.size() < 2)
            {
                fmt::print("Could not parse line #{}\n", lineNum);
                return {};
            }

            try
            {
                vals[n] = std::stod(match[1]);
            }
            catch (std::invalid_argument)
            {
                fmt::print("Could not parse line #{}\n", lineNum);
                return {};
            }

            // Move past the found string to continue searching
            line = match.suffix();
        }

        SCastEntry entry;
        entry.depth = vals[0];
        entry.c = vals[1];
        entries.push_back(entry);
    }

    cast.desc = "Simple text-based SSP";
    cast.fileName = fileName;

    return cast;
}
