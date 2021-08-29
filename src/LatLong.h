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
  * \file   LatLong.h
  * \brief  Structures and functions for handling latitude and longitude
  */

#pragma once


//! Structure holding latitude and longitude in decimal degrees
struct SLatLong
{
    union
    {
        double latitude;
        double lat;
    };
    union
    {
        double longitude;
        double lon;
    };
};


/*!
 * \brief Converts latitude or longitude to its decimal values from deg:min:sec format
 * 
 * This does no bounds checking. All values other than degrees must be positive. This follows this typical
 * format, where west and south entries are negative.
 */
double LatLonToDecimal(int degrees, unsigned int minutes, double seconds);

/*!
 * \brief Converts latitude or longitude from its decimal values to deg:min:sec format
 *
 * This does no bounds checking. This follows this typical format, where west and south entries are negative.
 */
bool LatLonFromDecimal(double decimal, double& degrees, double& minutes, double& seconds);

//! Checks whether a given lat/lon is within valid ranges
bool CheckLatLon(const SLatLong& latlon);
//! Checks whether a given lat/lon is within valid ranges
bool CheckLatLon(double lat, double lon);
