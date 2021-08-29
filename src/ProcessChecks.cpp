/*
 * MIT License
 *
 * Copyright (c) 2021 Denton Woods
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

 /*!
  * \file   ProcessChecks.cpp
  * \brief  Checks and cleanup for running on cast data.
  *
  * Sometimes samples can have spurious data (negative sound speeds, etc.), and this file provides
  * a set of routines that can be run to check for and remove those entries.
  */

#include "pch.h"
#include <SspCpp/LatLong.h>
#include <SspCpp/ProcessChecks.h>
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


bool CheckLatLon(double lat, double lon)
{
    if (lat < -90 || lat > 90)
        return false;
    if (lon < -180 || lat > 180)
        return false;
    return true;
}

bool CheckLatLon(const SLatLong& latlon)
{
    return CheckLatLon(latlon.lat, latlon.lon);
}

bool CheckLatLon(SCast& cast)
{
    return CheckLatLon(cast.lat, cast.lon);
}


bool CheckSoundSpeed(double c)
{
    if (c <= 0.0)
        return false;
    // This is the theoretical highest sound speed possible, in solid hydrogen.
    //  From "Speed of sound from fundamental physical constants" by K. Trachenko et. al.,
    //  doi: 10.1126/sciadv.abc8662
    if (c > 36100.0)
        return false;

    return true;
}


bool CheckDepth(double depth)
{
    if (depth < 0.0)
        return false;
    // Depth of the Challenger Deep (deepest part of the ocean) as measured in a 2020 survey
    //  (https://asa.scitation.org/doi/abs/10.1121/1.5146741)
    if (depth > 10991)
        return false;
    return true;
}


bool CheckDepthIncreasing(const std::vector<SCastEntry>& cast)
{
    if (cast.size() == 0)
        return false;

    for (size_t n = 1; n < cast.size(); ++n)
    {
        if (cast[n].c <= cast[n - 1].c)
            return false;
    }

    return true;
}


bool CheckLimits(const SCast& cast)
{
    for (const auto& entry : cast.entries)
    {
        if (!CheckLimits(entry))
            return false;
    }
    return true;
}


bool CheckLimits(const SCastEntry& entry)
{
    if (!CheckSoundSpeed(entry.c))
        return false;
    if (!CheckDepth(entry.depth))
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
