#include "pch.h"
#include "SeaBird.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <date/date.h>
#include <fmt/format.h>
#include "Cast.h"
#include "StringUtilities.h"
#include "TimeStruct.h"


namespace ssp
{

bool ParseCnvTime(std::string header, SCast& cast)
{
    std::regex rgxTime("# start_time = ([a-zA-Z]+ [0-9]+ [0-9]+ [0-9]+:[0-9]+:[0-9]+)");
    std::smatch match;
    std::regex_search(header, match, rgxTime);
    if (match.size() != 2)
    {
        std::cout << "Could not parse time\n";
        return false;
    }

    std::istringstream in(match[1]);
    date::sys_seconds tp;
    in >> date::parse("%b %d %Y %T", tp);
    auto tp_days = floor<date::days>(tp);
    auto hms = date::hh_mm_ss<std::chrono::seconds>{ tp - tp_days };
    auto ymd = date::year_month_day{ tp_days };
    
    cast.time = CreateTime(static_cast<int>(ymd.year()), static_cast<unsigned int>(ymd.month()), static_cast<unsigned int>(ymd.day()),
        hms.hours().count(), hms.minutes().count(), static_cast<unsigned int>(hms.seconds().count()));

    // To verify that CreateTime worked correctly.
    //std::time_t tt = std::mktime(&cast.time);
    //std::cout << std::put_time(std::localtime(&tt), "%Y-%m-%d %X") << "\n";

    return true;
}


bool ParseLatLon(std::string header, SCast& cast)
{
    std::smatch match;

    // From a couple of Sea-Bird's documents (e.g., https://www.seabird.com/asset-get.download.jsa?id=54952866001):
    //  "2 fixed digits of degrees, 2 fixed digits of minutes, and variable number of digits for decimal-fraction of
    //   minutes. Leading zeros always included for degrees and minutes to maintain fixed length. Decimal point and
    //   associated decimal-fraction are optional if full resolution not required."

    // Parse latitude string
    std::regex rgxLat("NMEA Latitude = ([0-9]+) ([0-9]+[.][0-9]+) ([NS])");
    std::regex_search(header, match, rgxLat);
    if (match.size() != 4)
        return false;

    std::string latDegStr = match[1];
    std::string latMinStr = match[2];
    std::string NorthSouth = match[3];

    int latDeg = std::stoi(latDegStr);
    double latMin = std::stod(latMinStr);
    double lat = latDeg + latMin / 60.0;

    if (NorthSouth == "S")
        lat = -lat;
    cast.lat = lat;

    // Parse longitude string
    std::regex rgxLon("NMEA Longitude = ([0-9]+) ([0-9]+[.][0-9]+) ([EW])");
    std::regex_search(header, match, rgxLon);
    if (match.size() != 4)
        return false;

    std::string lonDegStr = match[1];
    std::string lonMinStr = match[2];
    std::string EastWest = match[3];

    int lonDeg = std::stoi(lonDegStr);
    double lonMin = std::stod(lonMinStr);
    double lon = lonDeg + lonMin / 60.0;

    if (EastWest == "W")
        lon = -lon;
    cast.lon = lon;

    return true;
}


std::optional<SCast> ReadSeaBirdCnv(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile)
    {
        std::cout << "Could not open file " << fileName << "\n";
        return {};
    }

    SCast cast;
    std::vector<SCastEntry>& entries = cast.entries;

    // Reads all of the header up through the "*END*" line
    std::string line, header;
    header.reserve(7000);
    bool bFoundEnd = false;

    while (std::getline(inFile, line))
    {
        if (line == "*END*")
        {
            bFoundEnd = true;
            break;
        }

        header += line + "\n";
    }
    if (!bFoundEnd)
    {
        std::cout << "Sea-Bird file has malformed header\n";
        return {};
    }

    // This can have different sensors, and they can likely be on any channel. Here we have to determine from
    //  the header where the information we are interested in is.
    int depthPos = -1, speedPos = -1, salinPos = -1, tempPos = -1;
    std::regex rgx("# name ([0-9]+) = ([a-zA-Z0-9]+): ([a-zA-Z]+)");
    std::smatch match;
    std::string headSub = header;

    while (std::regex_search(headSub, match, rgx))
    {
        int pos;
        try
        {
            pos = std::stoi(match[1]);
        }
        catch (std::invalid_argument)
        {
            std::cout << "Invalid latitude/longitude strings\n";
            return {};
        }
        std::string sensorType = match[3];

        if (sensorType == "Depth")
            depthPos = pos;
        else if (sensorType == "Sound")  // Only captures "Sound" from "Sound Velocity"
            speedPos = pos;
        else if (sensorType == "Salinity")
            salinPos = pos;
        else if (sensorType == "Temperature")
            tempPos = pos;

        // Move past the found string to continue searching
        headSub = match.suffix();
    }

    /// @todo: Calculate sound speed from the other parameters (if depth is present)
    if (depthPos == -1 || speedPos == -1)
    {
        std::cout << "Missing sensor types in " << fileName << "\n";
        return {};
    }

    if (!ParseCnvTime(header, cast))
        return {};

    if (!ParseLatLon(header, cast))
        return {};

    /// @todo: May want to do something with the "# nvalues =" line

    while (std::getline(inFile, line))
    {
        if (line.size() == 0)
            break;

        //std::cout << line << "\n";

        auto lineStrings = SplitString(line);
        
        try
        {
            SCastEntry entry;
            entry.depth = std::stod(lineStrings[depthPos]);
            entry.c = std::stod(lineStrings[speedPos]);
            if (salinPos != -1)
                entry.salinity = std::stod(lineStrings[salinPos]);
            if (tempPos != -1)
                entry.temp = std::stod(lineStrings[tempPos]);

            entries.push_back(entry);
        }
        catch (std::invalid_argument)
        {
            std::cout << "Invalid latitude/longitude strings\n";
            return {};
        }
    }

    /// @ todo: Get latitude and longitude

    cast.desc = "Sea-Bird CNV";
    cast.fileName = fileName;

    return cast;
}


bool ParseTsvHeader(std::string line, SCast& cast)
{
    // Header string format: "## DATE:yyyy-mm-ddThh:mm:ss\tLATITUDE:xx.xx\tLONGITUDE:xx.xx"
    std::regex rgx("## DATE:([0-9]+)-([0-9]+)-([0-9]+)T([0-9])+:([0-9])+:([0-9]+).*LATITUDE:([+-]?([0-9]*[.])?[0-9]+).*LONGITUDE:([+-]?([0-9]*[.])?[0-9]+)");
    std::smatch matches;
    std::regex_search(line, matches, rgx);

    if (matches.size() != 11)
    {
        std::cout << "Sea-Bird .tsv has incorrect header\n";
        return false;
    }

    if (!CreateTime(matches[1], matches[2], matches[3], matches[4], matches[5], matches[6], cast.time))
        return false;

    try
    {
        cast.lat = std::stod(matches[7]);
        cast.lon = std::stod(matches[9]);
    }
    catch (std::invalid_argument)
    {
        std::cout << "Invalid latitude/longitude strings\n";
        return false;
    }

    return true;
}


std::optional<SCast> ReadSeaBirdTsv(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile)
    {
        std::cout << "Could not open file " << fileName << "\n";
        return {};
    }

    SCast cast;
    std::vector<SCastEntry>& entries = cast.entries;
    std::string line;

    try
    {
        if (!std::getline(inFile, line))
            throw "Line read failure";

        if (!ParseTsvHeader(line, cast))
            throw "Could not parse header line";

        while (std::getline(inFile, line))
        {
            if (line.size() == 0)
                break;

            // The depth and sound speed are required fields
            std::stringstream ss(line);
            SCastEntry entry;
            ss >> entry.depth >> entry.c;
            if (ss.fail())
                throw fmt::format("Incomplete entry for line #{}", entries.size() + 2);

            // Temperature and salinity are optional fields (may not be present in the file but have to be present together)
            ss >> entry.temp >> entry.salinity;
            if (ss.fail())
            {
                entry.temp = 0;
                entry.salinity = 0;
            }

            entries.push_back(entry);
        }
    }
    catch (std::string err)
    {
        std::cout << "Error reading Sea-Bird file (" << fileName << "): " << err << "\n";
        return {};
    }

    cast.lat = 0;
    cast.lon = 0;
    cast.desc = "Sea-Bird Nautilus";
    cast.fileName = fileName;

    return cast;
}


std::optional<SCast> ReadSeaBird(const std::string& fileName)
{
    /// @todo: Add .csv format
    return ReadSeaBirdTsv(fileName);
}

};  // End namespace ssp
