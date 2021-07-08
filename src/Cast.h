#pragma once

#include <ctime>
#include <vector>

namespace ssp
{
    struct SCastEntry
    {
        double depth;  //!< Depth in meters
        double c;      //!< Sound speed in meters/second
        double temp;   //!< Temperature in degrees Celsius
        double salinity;  //!< Salinity in parts per thousand (ppt)
        double absorp;  //!< Absorption /// @todo: Do any casts typically have this?
    };

    struct SCast
    {
        std::vector<SCastEntry> entries;
        std::tm time;  //!< May use C++20 features from std::chrono in the future
        double lat;  //!< Latitude
        double lon;  //!< Longitude
    };
};
