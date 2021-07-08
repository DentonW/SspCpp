#pragma once

#include <optional>
#include <string>
#include "Cast.h"

namespace ssp
{
    enum class eCastType
    {
        SeaBird,
        Sonardyne,
        Unknown
    };

    std::optional<SCast> ReadCast(const std::string& fileName, eCastType type = eCastType::Unknown);
};
