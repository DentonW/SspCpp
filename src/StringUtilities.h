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
  * \file   StringUtilities.h
  * \brief  Utility function(s) for string modifications
  */

#pragma once

#include <algorithm>
#include <filesystem>
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

    /*
     * Case sensitive implementation of StartsWith()
     * It checks if the string 'mainStr' starts with given string 'toMatch'
     * Example from https://thispointer.com/c-check-if-a-string-starts-with-an-another-given-string/
     */
    inline bool StartsWith(std::string mainStr, std::string toMatch)
    {
        // std::string::find returns 0 if toMatch is found at starting
        if (mainStr.find(toMatch) == 0)
            return true;
        else
            return false;
    }

    /*
     * Case insensitive implementation of StartsWith()
     * It checks if the string 'mainStr' starts with given string 'toMatch'
     * Example from https://thispointer.com/c-check-if-a-string-starts-with-an-another-given-string/
     */
    inline bool StartsWithCaseInsensitive(std::string mainStr, std::string toMatch)
    {
        // Convert mainStr to lower case
        std::transform(mainStr.begin(), mainStr.end(), mainStr.begin(), ::tolower);
        // Convert toMatch to lower case
        std::transform(toMatch.begin(), toMatch.end(), toMatch.begin(), ::tolower);
        if (mainStr.find(toMatch) == 0)
            return true;
        else
            return false;
    }

    //! Gets the filename extension (with leading .)
    inline std::string GetExtension(std::string fileName)
    {
        return std::filesystem::path(fileName).extension().string();
    }

    //! Converts an ASCII string to its lowercase equivalent
    inline std::string Lowercase(std::string s)
    {
        // From https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case
        //  Will fail on UTF-8 data, since this operates per-byte.
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return s;
    }

    //
    // The following 3 functions are modified from https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
    //

    // Trim whitespace from start
    inline std::string ltrim(std::string s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        return s;
    }

    // Trim whitespace from end
    inline std::string rtrim(std::string s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
        return s;
    }

    // Trim whitespace from both ends
    inline std::string trim(std::string s)
    {
        return rtrim(ltrim(s));
    }
};
