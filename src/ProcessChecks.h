#pragma once

#include "Cast.h"

namespace ssp
{
    void Reorder(SCast& cast);
    void RemoveNegativeDepths(SCast& cast);
    void RemoveNegativeSpeeds(SCast& cast);

    /*! Removes entries with the same depth
     *
     * This is relatively dumb, as it does strict comparison. We may want to switch to a percent difference threshold test.
     */
    void RemoveDuplicateDepths(SCast& cast);

    //! Checks whether latitude/longitude values are with valid ranges
    bool CheckLatLon(SCast& cast);

    //! Checks whether cast values are within proper limits (no negative values and values physically reasonable)
    bool CheckLimits(const SCast& cast);
};
