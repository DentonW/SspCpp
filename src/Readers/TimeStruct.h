#pragma once

#include <ctime>
#include <string>


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
        catch(std::invalid_argument& err)
        {
            err;
            std::cout << "Date/time conversion failed\n";
            return false;
        }
    }
};
