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
 * \file   Physical.cpp
 * \brief  Equations for calculating physical values (gravity, etc.)
 */

#include "pch.h"
#include "SoundSpeed.h"
#include <cmath>


namespace ssp
{

double Gravity(double latitudeDeg)
{
    double sinLat = sin(latitudeDeg * 0.017453292519943295);  // Convert to radians
    double sin2 = sinLat * sinLat;

    double g = 9.780318 * (1.0 + 5.2788e-3 * sin2 + 2.36e-5 * sin2*sin2);
    return g;
}


double Depth(double pressureDBar, double latitudeDeg)
{
    const double g = Gravity(latitudeDeg);

    double P = pressureDBar * 0.01;  // Decibar to MegaPascal
    double Z = 9.72659e2 * P - 2.2512e-1 * P*P + 2.279e-4 * P*P*P - 1.82e-7 * P*P*P*P;
    Z /= (g + 1.092e-4 * P);

    return Z;
}

};  // End namespace ssp
