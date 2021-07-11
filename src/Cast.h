#pragma once

#include <ctime>
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
        std::string desc;  // Description of type of file read from
        std::string fileName;  // Filename of this cast
        std::vector<SCastEntry> entries;
        std::tm time;  //!< May use C++20 features from std::chrono in the future
        double lat;  //!< Latitude
        double lon;  //!< Longitude
    };
};
