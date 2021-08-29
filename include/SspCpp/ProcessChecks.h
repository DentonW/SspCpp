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
  * \file   ProcessChecks.h
  * \brief  Checks and cleanup for running on cast data.
  * 
  * Sometimes samples can have spurious data (negative sound speeds, etc.), and this file provides
  * a set of routines that can be run to check for and remove those entries.
  */

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

    //! Checks whether latitude/longitude values are within valid ranges
    bool CheckLatLon(SCast& cast);
    //! Checks whether latitude/longitude values are within valid ranges
    bool CheckLatLon(double lat, double lon);

    //! Checks whether the sound speed is within physically valid ranges
    bool CheckSoundSpeed(double c);

    //! Checks whether the depth is monotonically increasing
    bool CheckDepthIncreasing(const std::vector<SCastEntry>& cast);

    //! Checks whether cast values are within proper limits (no negative values and values physically reasonable)
    bool CheckLimits(const SCast& cast);

    //! Checks whether single cast sample is within proper limits (no negative values and values physically reasonable)
    bool CheckLimits(const SCastEntry& entry);

    bool Cleanup(SCast& cast);
};
