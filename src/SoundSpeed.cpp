#include "pch.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fmt/format.h>

#ifdef MATPLOTLIB_CPP_SUPPORT
    #ifdef _DEBUG
      #undef _DEBUG
        #include <matplotlibcpp.h>
      #define _DEBUG
    #else
      #include <matplotlibcpp.h>
    #endif
#endif

#include "Cast.h"
#include "ProcessChecks.h"
#include "SoundSpeed.h"

#include "Readers/Asvp.h"
#include "Readers/SeaAndSun.h"
#include "Readers/SeaBird.h"
#include "Readers/Sonardyne.h"


namespace ssp
{

template <class T>
T min_vec(const std::vector<T>& v)
{
    return *std::min_element(begin(v), end(v));
}

template <class T>
T max_vec(const std::vector<T>& v)
{
    return *std::max_element(begin(v), end(v));
}



#ifdef MATPLOTLIB_CPP_SUPPORT
bool PlotCast(const SCast& cast)
{
    namespace plt = matplotlibcpp;

    std::vector<double> depth, c;
    depth.reserve(cast.entries.size());
    c.reserve(cast.entries.size());

    for (const auto& entry : cast.entries)
    {
        depth.push_back(entry.depth);
        c.push_back(entry.c);
    }

    plt::plot(c, depth);
    plt::xlabel("Sound speed (m/s)");
    plt::ylabel("Depth (m)");
    plt::ylim(max_vec(depth), min_vec(depth));
    plt::show();

    return true;
}
#else
bool PlotData(const SCast& cast)
{
    return true;
}
#endif



std::optional<SCast> ReadCast(const std::string& fileName, eCastType type)
{
    switch (type)
    {
        case eCastType::Asvp:
            return ReadAsvp(fileName);

        case eCastType::SeaAndSun:
            return ReadSeaAndSun(fileName);

        case eCastType::SeaBirdCnv:
            return ReadSeaBirdCnv(fileName);

        case eCastType::SeaBirdTsv:
            return ReadSeaBirdTsv(fileName);

        case eCastType::Sonardyne:
            return ReadSonardyne(fileName);

        case eCastType::Unknown:  // Fallthrough
        default:
            std::cout << "No support for determining SSP file types\n";
            break;
    }

    return {};
}


//
// Sound speed computation functions
//

double WongZhuCw(double T, double P)
{
    const double C00 = 1402.388;
    const double C01 = 5.03830;
    const double C02 = -5.81090e-2;
    const double C03 = 3.3432e-4;
    const double C04 = -1.47797e-6;
    const double C05 = 3.1419e-9;
    const double C10 = 0.153563;
    const double C11 = 6.8999e-4;
    const double C12 = -8.1829e-6;
    const double C13 = 1.3632e-7;
    const double C14 = -6.1260e-10;
    const double C20 = 3.1260e-5;
    const double C21 = -1.7111e-6;
    const double C22 = 2.5986e-8;
    const double C23 = -2.5353e-10;
    const double C24 = 1.0415e-12;
    const double C30 = -9.7729e-9;
    const double C31 = 3.8513e-10;
    const double C32 = -2.3654e-12;

    double C0 = C00 + T * (C01 + T * (C02 + T * (C03 + T * (C04 + T * C05))));
    double C1 = (C10 + T * (C11 + T * (C12 + T * (C13 + T * C14))));
    double C2 = (C20 + T * (C21 + T * (C22 + T * (C23 + T * C24))));
    double C3 = (C30 + T * (C31 + T * C32));

    double Cw = C0 + C1 * P + C2 * P*P + C3 * P*P*P;
    return Cw;
}

double WongZhuA(double T, double P)
{
    const double A00 = 1.389;
    const double A01 = -1.262e-2;
    const double A02 = 7.166e-5;
    const double A03 = 2.008e-6;
    const double A04 = -3.21e-8;
    const double A10 = 9.4742e-5;
    const double A11 = -1.2583e-5;
    const double A12 = -6.4928e-8;
    const double A13 = 1.0515e-8;
    const double A14 = -2.0142e-10;
    const double A20 = -3.9064e-7;
    const double A21 = 9.1061e-9;
    const double A22 = -1.6009e-10;
    const double A23 = 7.994e-12;
    const double A30 = 1.100e-10;
    const double A31 = 6.651e-12;
    const double A32 = -3.391e-13;

    double A0 = A00 + T * (A01 + T * (A02 + T * (A03 + T * A04)));
    double A1 = (A10 + T * (A11 + T * (A12 + T * (A13 + T * A14))));
    double A2 = (A20 + T * (A21 + T * (A22 + T * A23)));
    double A3 = (A30 + T * (A31 + T * A32));

    double A = A0 + A1 * P + A2 * P*P + A3 * P*P*P;
    return A;
}

double WongZhuB(double T, double P)
{
    const double B00 = -1.922e-2;
    const double B01 = -4.42e-5;
    const double B10 = 7.3637e-5;
    const double B11 = 1.7950e-7;

    double B = B00 + B01*T + (B10 + B11*T) * P;
    return B;
}

double WongZhuD(double T, double P)
{
    const double D00 = 1.727e-3;
    const double D10 = -7.9836e-6;

    double D = D00 + D10 * P;
    return D;
}

double WongZhu(double temp, double salin, double pressure)
{
    // Temp: [0, 40] Celsius
    // Salinity: [0, 40] parts per thousand (ppt)
    // Pressure: [0, 1000] bar

    double T = temp;
    double S = salin;
    double P = pressure;

    double Cw = WongZhuCw(temp, pressure);
    double A  = WongZhuA(temp, pressure);
    double B  = WongZhuB(temp, pressure);
    double D  = WongZhuD(temp, pressure);

    double c = Cw + A*S + B*pow(S,3.0/2.0) + D*S*S;

    return c;
}


};  // End namespace ssp
