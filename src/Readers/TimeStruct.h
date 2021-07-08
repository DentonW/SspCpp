#pragma once

#include <ctime>

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
};
