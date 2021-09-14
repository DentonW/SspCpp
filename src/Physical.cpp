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
#include <SspCpp/SoundSpeed.h>
#include <cmath>


template <class T>
T deg2rad(T deg)
{
    return deg * 0.017453292519943295;
}

namespace ssp
{

double Gravity(double latitudeDeg)
{
    double sinLat = sin(deg2rad(latitudeDeg));
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


double DepthToPressure(double depth, double latitudeDeg)
{
    double z = depth;

    double sinphi = sin(deg2rad(latitudeDeg));  // Convert to radians
    //double g = 9.7803 * (1.0 + 5.3e-3 * sinphi*sinphi);  // Their paper appears to have an error with 0.7803 instead
    double g = Gravity(latitudeDeg);
    double hZ45 = 1.00818e-2 * z + 2.465e-8 * z*z - 1.25e-13 * z*z*z + 2.8e-19 * z*z*z*z;
    double k = (g - 2e-5 * z) / (9.80612 - 2e-5 * z);

    double p = hZ45 * k;  // In MegaPascals
    p *= 100.0;  // Convert using 1 Mpa = 100 decibars
    return p;
}


double PascalToBar(double pascals)
{
    return 1e-5 * pascals;
}


/*! \brief Converts the conductivity ratio to salinity (not to be used directly by users).
 * 
 * Same paper as ConductivityToSalinity. T is in Celsius. Rt is dimensionless.
 */

double CondRatioToSalinity(double Rt, double T)
{
    constexpr double a0 =  0.0080;
    constexpr double a1 = -0.1692;
    constexpr double a2 =  25.3851;
    constexpr double a3 =  14.0941;
    constexpr double a4 = -7.0261;
    constexpr double a5 =  2.7081;
    constexpr double b0 =  0.0005;
    constexpr double b1 = -0.0056;
    constexpr double b2 = -0.0066;
    constexpr double b3 = -0.0375;
    constexpr double b4 =  0.0636;
    constexpr double b5 = -0.0144;
    constexpr double k  =  0.0162;

    double rtRt = sqrt(Rt);

    // Eq.(2)
    double dS = b0 + (b1 + (b2 + (b3 + (b4 + b5 * rtRt) * rtRt) * rtRt) * rtRt) * rtRt;
    dS *= (T - 15.0) / (1.0 + k * (T - 15.0));

    // Eq.(1)
    double S = a0 + (a1 + (a2 + (a3 + (a4 + a5 * rtRt) * rtRt) * rtRt) * rtRt) * rtRt + dS;

    return S;
}


double ConductivityToSalinity(double conductivitySm, double pressureDbar, double tempC)
{
    double p = pressureDbar;
    double t = tempC;

    constexpr double ref = 4.2914;  // Siemens / m for reference C(35,15,0)

    constexpr double c0 =  0.6766097;
    constexpr double c1 =  2.00564e-2;
    constexpr double c2 =  1.104259e-4;
    constexpr double c3 = -6.9698e-7;
    constexpr double c4 =  1.0031e-9;
    constexpr double e1 =  2.070e-5;
    constexpr double e2 = -6.370e-10;
    constexpr double e3 =  3.989e-15;
    constexpr double d1 =  3.426e-2;
    constexpr double d2 =  4.464e-4;
    constexpr double d3 =  4.215e-1;
    constexpr double d4 = -3.107e-3;

    double R = conductivitySm / ref;  // First equation in their paper on p.6

    double rt = c0 + (c1 + (c2 + (c3 + c4 * t) * t) * t) * t;  // Eq.(3)

    double RpNum = p * (e1 + (e2 + e3 * p) * p);
    //double RpDen = 1.0 + d1 * t + d2 * t * t + (d3 + d4 * t) * R;
    double RpDen = 1 + (d1 + d2 * t) * t + (d3 + d4 * t) * R;
    double Rp = 1.0 + RpNum / RpDen;  // Eq.(4)

    double Rt = R / (Rp * rt);  // Equation at the end of p.8

    double S = CondRatioToSalinity(Rt, t);

    return S;
}

};  // End namespace ssp
