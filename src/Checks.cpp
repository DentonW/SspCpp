#include "pch.h"
#include "Checks.h"
#include <algorithm>


void ssp::Reorder(SCast& cast)
{
    std::sort(begin(cast.entries), end(cast.entries), [](SCastEntry& left, SCastEntry& right) { return left.depth < right.depth; });
    return;
}


void ssp::RemoveNegativeDepths(SCast& cast)
{
    auto removeIter = std::remove_if(begin(cast.entries), end(cast.entries), [](SCastEntry& entry) { return entry.depth < 0; });
    cast.entries.erase(removeIter, end(cast.entries));
    return;
}


bool ssp::CheckLatLon(SCast& cast)
{
    if (cast.lat < -90 || cast.lat > 90)
        return false;
    if (cast.lon < -180 || cast.lat > 180)
        return false;

    return true;
}
