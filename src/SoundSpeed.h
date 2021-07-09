#pragma once

#include <optional>
#include <string>
#include "Cast.h"

namespace ssp
{
    enum class eCastType
    {
        SeaBirdCnv,
        SeaBirdTsv,
        Sonardyne,
        Unknown
    };

    std::optional<SCast> ReadCast(const std::string& fileName, eCastType type = eCastType::Unknown);

    bool PlotCast(const ssp::SCast& cast);

    double SoundSpeedWongZhu(double temp, double salin, double pressure);
};
