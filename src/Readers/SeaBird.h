#pragma once

#include <optional>
#include <string>
#include <vector>
#include "../Cast.h"

namespace ssp
{
    std::optional<SCast> ReadSeaBirdCnv(const std::string& fileName);
    std::optional<SCast> ReadSeaBirdTsv(const std::string& fileName);
    std::optional<SCast> ReadSeaBird(const std::string& fileName);
};
