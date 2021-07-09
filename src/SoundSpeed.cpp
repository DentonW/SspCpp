#include "pch.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fmt/format.h>

#ifdef _DEBUG
  #undef _DEBUG
    #include <matplotlibcpp.h>
  #define _DEBUG
#else
  #include <matplotlibcpp.h>
#endif

#include "Cast.h"
#include "ProcessChecks.h"
#include "SoundSpeed.h"

#include "Readers/SeaBird.h"
#include "Readers/Sonardyne.h"


namespace ssp
{

#ifdef MATPLOTLIB_CPP_SUPPORT
bool ssp::PlotCast(const ssp::SCast& cast)
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
    plt::ylim(depth.back(), depth.front());
    plt::show();

    return true;
}
#else
bool ssp::PlotData(const SCast& cast)
{
    return true;
}
#endif



std::optional<ssp::SCast> ssp::ReadCast(const std::string& fileName, eCastType type)
{
    switch (type)
    {
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

double ssp::SoundSpeedWongZhu(double temp, double salin, double pressure)
{

    return 0.0;
}


};  // End namespace ssp
