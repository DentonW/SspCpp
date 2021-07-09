#pragma once

#include "Cast.h"

namespace ssp
{
    void Reorder(SCast& cast);
    void RemoveNegativeDepths(SCast& cast);

    //! Checks whether latitude/longitude values are with valid ranges
    bool CheckLatLon(SCast& cast);
};
