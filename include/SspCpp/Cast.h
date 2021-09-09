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
 * \file   Cast.h
 * \brief  Main structures for storing cast information (time, location, sound speed, etc.)
 */

#pragma once

#include <ctime>
#include <string>
#include <vector>

namespace ssp
{
    struct SCastEntry
    {
        SCastEntry() { depth = 0; c = 0; temp = 0; salinity = 0; pressure = 0; absorp = 0; }
        double depth;  //!< Depth in meters
        double c;      //!< Sound speed in meters/second
        double temp;   //!< Temperature in degrees Celsius
        double salinity;  //!< Salinity in parts per thousand (ppt)
        double pressure;  //!< Usually expressed as depth (bars???)
        double absorp;  //!< Absorption /// @todo: Do any casts typically have this?
    };

    struct SCast
    {
        SCast() { lat = 0; lon = 0; time.tm_hour = 0; time.tm_min = 0; time.tm_sec = 0; time.tm_year = 0; time.tm_mon = 0; time.tm_mday = 0; }
        std::string desc;  //!< Description of type of file read from
        std::string fileName;  //!< Filename of this cast
        std::vector<SCastEntry> entries;
        std::tm time;  //!< May use C++20 features from std::chrono in the future
        double lat;  //!< Latitude
        double lon;  //!< Longitude
    };
};
