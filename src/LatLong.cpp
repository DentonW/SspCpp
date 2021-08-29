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
  * \file   LatLong.cpp
  * \brief  Structures and functions for handling latitude and longitude
  */

#include "pch.h"
#include "LatLong.h"


namespace ssp
{

// Sign function from https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}


double LatLonToDecimal(int degrees, unsigned int minutes, double seconds)
{
    double sig = sgn(degrees);
    double dec = abs(degrees) + minutes / 60.0 + seconds / 3600.0;
    dec *= sig;
    return dec;
}


bool LatLonFromDecimal(double decimal, double& degrees, double& minutes, double& seconds)
{
    double sig = sgn(decimal);
    decimal = abs(decimal);

    degrees = std::floor(decimal);

    decimal -= degrees;
    decimal *= 60;
    minutes = std::floor(decimal);

    decimal -= minutes;
    seconds = decimal * 60;

    degrees = sig * degrees;

    return true;
}

};  // End namespace ssp
