/*
 * MIT License
 *
 * Copyright (c) 2023 Denton Woods
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
  * \file   ProcessChecks.h
  * \brief  Checks and cleanup for running on cast data.
  * 
  * Sometimes samples can have spurious data (negative sound speeds, etc.), and this file provides
  * a set of routines that can be run to check for and remove those entries.
  */

#pragma once

#include "LatLong.h"
#include "Cast.h"
#include "sspcpp_export.h"


namespace ssp
{
    SSPCPP_EXPORT void Reorder(SCast& cast);
    SSPCPP_EXPORT void RemoveNegativeDepths(SCast& cast);
    SSPCPP_EXPORT void RemoveNegativeSpeeds(SCast& cast);

    /*! Removes entries with the same depth
     *
     * This is relatively dumb, as it does strict comparison. We may want to switch to a percent difference threshold test.
     */
    SSPCPP_EXPORT void RemoveDuplicateDepths(SCast& cast);

    //! Checks whether latitude/longitude values are within valid ranges
    SSPCPP_EXPORT bool CheckLatLon(double lat, double lon);
    //! Checks whether latitude/longitude values are within valid ranges
    SSPCPP_EXPORT bool CheckLatLon(const SLatLong& latlon);
    //! Checks whether latitude/longitude values are within valid ranges
    SSPCPP_EXPORT bool CheckLatLon(SCast& cast);

    //! Checks whether the sound speed is within physically valid ranges
    SSPCPP_EXPORT bool CheckSoundSpeed(double c);

    //! Checks whether the depth fits within the ocean depth limits
    SSPCPP_EXPORT bool CheckDepth(double depth);

    //! Checks whether the depth is monotonically increasing
    SSPCPP_EXPORT bool CheckDepthIncreasing(const std::vector<SCastEntry>& cast);

    //! Checks whether cast values are within proper limits (no negative values and values physically reasonable)
    SSPCPP_EXPORT bool CheckLimits(const SCast& cast);

    //! Checks whether single cast sample is within proper limits (no negative values and values physically reasonable)
    SSPCPP_EXPORT bool CheckLimits(const SCastEntry& entry);

    SSPCPP_EXPORT bool Cleanup(SCast& cast);
};
