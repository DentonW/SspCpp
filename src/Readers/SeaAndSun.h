#pragma once

#include <optional>
#include <string>
#include <vector>
#include "../Cast.h"

namespace ssp
{
    std::optional<SCast> ReadSeaAndSun(const std::string& fileName);
};
