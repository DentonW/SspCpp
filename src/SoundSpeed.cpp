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
#include "SoundSpeed.h"
#include "Checks.h"

#include "Readers/SeaBird.h"
#include "Readers/Sonardyne.h"


std::vector<std::string> SplitString(std::string str)
{
    // Idea from https://stackoverflow.com/questions/5607589/right-way-to-split-an-stdstring-into-a-vectorstring
    std::stringstream ss(str);
    std::istream_iterator<std::string> beginStream(ss);
    std::istream_iterator<std::string> endStream;
    std::vector<std::string> entries(beginStream, endStream);

    return entries;
}


#ifdef MATPLOTLIB_CPP_SUPPORT
bool PlotCast(const ssp::SCast& cast)
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
bool PlotData(const SCast& cast)
{
    return true;
}
#endif





int main()
{
    auto sspSonardyne1 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/sonardyne/_Demo1.pro", ssp::eCastType::Sonardyne);
    if (!sspSonardyne1)
        return 1;
    //PlotCast(*sspSonardyne1);

    auto sspSonardyne2 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/sonardyne/Demo2.pro", ssp::eCastType::Sonardyne);
    if (!sspSonardyne2)
        return 1;
    //PlotCast(*sspSonardyne2);

    auto sspSeaBird1 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/_H1662.SVP.sampled.ascent.tsv", ssp::eCastType::SeaBirdTsv);
    if (!sspSeaBird1)
        return 1;
    //PlotCast(*sspSeaBird1);

    auto sspSeaBird2 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/_H1662.SVP.sampled.ascent.with_temp_and_sal.tsv", ssp::eCastType::SeaBirdTsv);
    if (!sspSeaBird2)
        return 1;
    //PlotCast(*sspSeaBird2);

    //auto sspSeaBird3 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/_2013_192_124923test.cnv", ssp::eCastType::SeaBirdCnv);
    //if (!sspSeaBird3)
    //    return 1;
    ////PlotCast(*sspSeaBird3);
    //Reorder(*sspSeaBird3);
    //RemoveNegativeDepths(*sspSeaBird3);

    auto sspSeaBird4 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/FK001_CTD02.cnv", ssp::eCastType::SeaBirdCnv);
    if (!sspSeaBird4)
        return 1;
    
    auto sspSeaBird5 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/EX1811_DIVE01_20181031_ROVCTD.cnv", ssp::eCastType::SeaBirdCnv);
    if (!sspSeaBird5)
        return 1;


    return 0;
}


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
