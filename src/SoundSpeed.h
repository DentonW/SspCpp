#pragma once

#include <optional>
#include <string>
#include "Cast.h"

namespace ssp
{
    enum class eCastType
    {
        Asvp,  // Kongsberg Maritime (.asvp)
        SeaAndSun,  // .tob
        SeaBirdCnv,
        SeaBirdTsv,
        Sonardyne,
        Unknown
    };

    std::optional<SCast> ReadCast(const std::string& fileName, eCastType type = eCastType::Unknown);

    bool PlotCast(const ssp::SCast& cast);



    double WongZhu(double temp, double salin, double pressure);

    double Gravity(double latitudeDeg);

    /*! From Leroyand Parthiot, "Depth-pressure relationships in the oceans and seas"
     *   https://doi.org/10.1121/1.421275
     */
    double Depth(double pressureDBar, double latitudeDeg);
};
