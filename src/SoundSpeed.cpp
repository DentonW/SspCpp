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
#include "Readers/SonarDyne.h"
#include "src\SoundSpeed.h"


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
    auto sspSonardyne1 = ssp::ReadSonardyne("F:/Coding/hyo2_soundspeed/data/input/sonardyne/_Demo1.pro");
    if (!sspSonardyne1)
        return 1;
    PlotCast(*sspSonardyne1);

    auto sspSonardyne2 = ssp::ReadSonardyne("F:/Coding/hyo2_soundspeed/data/input/sonardyne/Demo2.pro");
    if (!sspSonardyne2)
        return 1;
    PlotCast(*sspSonardyne2);


    return 0;
}


std::optional<ssp::SCast> ssp::ReadCast(const std::string& fileName, eCastType type)
{
    switch (type)
    {
        case eCastType::Sonardyne:
            return ReadSonardyne(fileName);

        case eCastType::Unknown:  // Fallthrough
        default:
            std::cout << "No support for determining SSP file types\n";
            break;
    }

    return {};
}
