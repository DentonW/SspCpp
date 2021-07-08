#pragma once

#include <optional>
#include <string>
#include "Cast.h"

namespace ssp
{
    enum class eCastType
    {
        Sonardyne,
        Unknown
    };

    std::optional<SCast> ReadCast(const std::string& fileName, eCastType type = eCastType::Unknown);
};
