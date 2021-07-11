#include "pch.h"
#include "ProcessChecks.h"
#include <algorithm>


namespace ssp
{

void Reorder(SCast& cast)
{
    std::sort(begin(cast.entries), end(cast.entries), [](SCastEntry& left, SCastEntry& right) { return left.depth < right.depth; });
    return;
}


void RemoveNegativeDepths(SCast& cast)
{
    auto removeIter = std::remove_if(begin(cast.entries), end(cast.entries), [](SCastEntry& entry) { return entry.depth < 0; });
    cast.entries.erase(removeIter, end(cast.entries));
    return;
}


void RemoveNegativeSpeeds(SCast& cast)
{
    auto removeIter = std::remove_if(begin(cast.entries), end(cast.entries), [](SCastEntry& entry) { return entry.c < 0; });
    cast.entries.erase(removeIter, end(cast.entries));
    return;
}


void RemoveDuplicateDepths(SCast& cast)
{
    Reorder(cast);  // Data must be sorted first!
    auto iter = std::unique(begin(cast.entries), end(cast.entries), [](SCastEntry& left, SCastEntry& right) { return left.depth == right.depth; });
    cast.entries.erase(iter, end(cast.entries));
    return;
}


bool CheckLatLon(SCast& cast)
{
    if (cast.lat < -90 || cast.lat > 90)
        return false;
    if (cast.lon < -180 || cast.lat > 180)
        return false;

    return true;
}


bool CheckLimits(const SCast& cast)
{
    return false;
}


bool CheckLimits(const SCastEntry& entry)
{
    if (entry.c <= 0)
        return false;
    if (entry.depth <= 0)
        return false;
    // The next values are optional, so they can be 0
    if (entry.pressure < 0)
        return false;
    if (entry.salinity < 0)
        return false;
    // Just something way out of the realm of possibility. If there is ice, maybe this could be 0 or less if the probe touches it?
    if (entry.temp < -100)
        return false;

    return true;
}


bool Cleanup(SCast& cast)
{
    // Remove obviously bad entries
    auto removeIter = std::remove_if(begin(cast.entries), end(cast.entries), [](SCastEntry& entry) { return !CheckLimits(entry); });
    cast.entries.erase(removeIter, end(cast.entries));

    Reorder(cast);
    RemoveDuplicateDepths(cast);

    if (!CheckLatLon(cast))
    {
        cast.lat = 0;
        cast.lon = 0;
    }

    if (cast.entries.size() == 0)
        return false;  // There were no valid entries!
    return true;
}

};  // End namespace ssp
