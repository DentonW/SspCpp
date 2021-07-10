#pragma once

#include <optional>
#include <string>
#include "Cast.h"

namespace ssp
{
    enum class eCastType
    {
        Asvp,  // Kongsberg Maritime (.asvp)
        SeaBirdCnv,
        SeaBirdTsv,
        Sonardyne,
        Unknown
    };

    std::optional<SCast> ReadCast(const std::string& fileName, eCastType type = eCastType::Unknown);

    bool PlotCast(const ssp::SCast& cast);

    double WongZhu(double temp, double salin, double pressure);
};
