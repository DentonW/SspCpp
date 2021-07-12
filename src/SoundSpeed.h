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
  * \file   SoundSpeed.h
  * \brief  Main interface header to be used in the library
  */

#pragma once

#include <optional>
#include <string>
#include "Cast.h"

#define SSP_WORLD_VERSION 0
#define SSP_MAJOR_VERSION 0
#define SSP_MINOR_VERSION 1
#define SSP_STRING_VERSION "0.0.1"

namespace ssp
{
    //! 
    enum class eCastType
    {
        Asvp,  //!< Kongsberg Maritime (.asvp)
        SeaAndSun,  //!< .tob
        SeaBirdCnv,  //!< 
        SeaBirdTsv,  //!< 
        Sonardyne,  //!< 
        Unknown  //!< 
    };

    std::optional<SCast> ReadCast(const std::string& fileName, eCastType type = eCastType::Unknown);

    bool PlotCast(const ssp::SCast& cast);



    double WongZhu(double temp, double salin, double pressure);

    double Gravity(double latitudeDeg);

    /*! From Leroy and Parthiot, "Depth-pressure relationships in the oceans and seas"
     *   https://doi.org/10.1121/1.421275
     */
    double Depth(double pressureDBar, double latitudeDeg);
};
