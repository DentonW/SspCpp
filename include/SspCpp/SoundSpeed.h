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
#include "ProcessChecks.h"

#define SSP_WORLD_VERSION 1
#define SSP_MAJOR_VERSION 0
#define SSP_MINOR_VERSION 0
#define SSP_STRING_VERSION "1.0.0"

namespace ssp
{
    //! 
    enum class eCastType
    {
        Aoml,        //!< AOML AMVER-SEAS XBT (.txt)
        Asvp,        //!< Kongsberg Maritime (.asvp)
        SeaAndSun,   //!< Sea&Sun (.tob)
        SeaBirdCnv,  //!< Sea-Bird (.cnv)
        SeaBirdTsv,  //!< Sea-Bird (.tsv)
        Sonardyne,   //!< SonarDyne (.pro)
        Unknown      //!< Does nothing currently - will try to determine file format in the future
    };

    std::optional<SCast> ReadCast(const std::string& fileName, eCastType type = eCastType::Unknown);

    bool PlotCast(const ssp::SCast& cast);


    /*!
     * Wong-Zhu equation for calculating sound speed
     * 
     * Valid ranges:
     *  Temp: [0, 40] Celsius
     *  Salinity: [0, 40] parts per thousand (ppt)
     *  Pressure: [0, 1000] bar
     */
    double WongZhu(double temp, double salin, double pressure);

    double Gravity(double latitudeDeg);

    /*! From Leroy and Parthiot, "Depth-pressure relationships in the oceans and seas"
     *   https://doi.org/10.1121/1.421275
     */
    double Depth(double pressureDBar, double latitudeDeg);

    /*! From Leroy and Parthiot, "Depth-pressure relationships in the oceans and seas"
     *   https://doi.org/10.1121/1.421275. Returns pressure in decibars (instead of MegaPascals as in the paper).
     */
    double DepthToPressure(double depth, double latitudeDeg);

    double PascalToBar(double pascals);
};
