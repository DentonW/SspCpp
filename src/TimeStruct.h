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
  * \file   TimeStruct.h
  * \brief  Functions for creating the time structure from its constituent parts.
  */

#pragma once

#include <ctime>
#include <string>
#include <date/date.h>


namespace ssp
{
    inline std::tm CreateTime(int year, int month, int day, int hour, int minute, int second)
    {
        std::tm time;

        time.tm_year = year - 1900;
        time.tm_mon = month - 1;  // 0-indexed
        time.tm_mday = day;
        time.tm_hour = hour;
        time.tm_min = minute;
        time.tm_sec = second;
        mktime(&time);  // Fills in the rest of the members (day of the week, etc.)

        return time;
    }

    inline bool CreateTime(std::string year, std::string month, std::string day, std::string hour, std::string minute, std::string second, std::tm& time)
    {
        try
        {
            int yeari = std::stoi(year);
            int monthi = std::stoi(month);
            int dayi = std::stoi(day);
            int houri = std::stoi(hour);
            int minutei = std::stoi(minute);
            int secondi = std::stoi(second);

            time = CreateTime(yeari, monthi, dayi, houri, minutei, secondi);
            return true;
        }
        catch(std::invalid_argument)
        {
            std::cout << "Date/time conversion failed\n";
            return false;
        }
    }

    inline bool CreateTime(const date::sys_seconds& tp, std::tm& time)
    {
        // Assuming that the input time is valid
        auto tp_days = floor<date::days>(tp);
        auto hms = date::hh_mm_ss<std::chrono::seconds>{ tp - tp_days };
        auto ymd = date::year_month_day{ tp_days };

        time = CreateTime(static_cast<int>(ymd.year()), static_cast<unsigned int>(ymd.month()), static_cast<unsigned int>(ymd.day()),
            hms.hours().count(), hms.minutes().count(), static_cast<unsigned int>(hms.seconds().count()));
        return true;
    }


};
