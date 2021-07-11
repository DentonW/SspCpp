#include "pch.h"
#include "SeaAndSun.h"
#include <array>
#include <fstream>
#include <regex>
#include "SoundSpeed.h"
#include "StringUtilities.h"
#include "TimeStruct.h"

namespace ssp
{

namespace internal
{
    //! The month names are in German. Returns a -1 on failure, 1 = January, 2 = February, etc.
    int Month(std::string month)
    {
        const std::array<std::string, 12> monthsGerman =
            { "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };

        for (size_t n = 0; n < monthsGerman.size(); ++n)
        {
            if (month == monthsGerman[n])
                return static_cast<int>(n) + 1;  // 1-indexed
        }

        return -1;  // Error - no match
    }


    bool ParseDateTime(const std::string& line, SCast& cast)
    {
        // Example date/time line: "Freitag, 20. Juli 2018 16:54:38"
        std::regex rgxDateTime("[a-zA-Z]+[,] ([0-9]+)[.] ([a-zA-Z]+) ([0-9]+) ([0-9]+):([0-9]+):([0-9]+)");
        std::smatch match;
        std::regex_search(line, match, rgxDateTime);
        if (match.size() != 7)
            return false;

        int monthNum = Month(match[2]);
        if (monthNum == -1)
            return false;
        std::string month = std::to_string(monthNum);

        if (!CreateTime(match[3], month, match[1], match[4], match[5], match[6], cast.time))
            return false;

        return true;
    }


    bool ParseLatLon(const std::string& line, SCast& cast)
    {
        std::stringstream ss(line);
        auto headVec = SplitString(line);
        if (headVec.size() < 10)
            return false;
        if (headVec[2] != "Lat.:" || headVec[6] != "Lon.:")
            return false;
        std::string lat1 = headVec[3], lat2 = headVec[4];
        std::string lon1 = headVec[7], lon2 = headVec[8];
        std::string NS = headVec[5];
        std::string EW = headVec[9];

        // Get rid of the degree and ' symbols at the ends of these strings
        lat1 = lat1.substr(0, lat1.size() - 1);
        lat2 = lat2.substr(0, lat2.size() - 1);
        lon1 = lon1.substr(0, lon1.size() - 1);
        lon2 = lon2.substr(0, lon2.size() - 1);

        try
        {
            // Convert the separate parts from degrees and minutes (plus fraction of minutes) into
            //  single latitude/longitude values
            cast.lat = (double)std::stoi(lat1);
            cast.lat += std::stod(lat2) / 60.0;
            cast.lon = (double)std::stoi(lon1);
            cast.lon += std::stod(lon2) / 60.0;

            if (NS != "N" && NS != "S")
                return false;
            if (EW != "E" && EW != "W")
                return false;
            if (NS == "S")
                cast.lat = -cast.lat;
            if (EW == "W")
                cast.lon = -cast.lon;
        }
        catch (std::invalid_argument)
        {
            return false;
        }
        return true;

    }
};


std::optional<SCast> ReadSeaAndSun(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile)
    {
        std::cout << "Could not open file " << fileName << "\n";
        return {};
    }

    SCast cast;
    std::vector<SCastEntry>& entries = cast.entries;
    std::string line, dateLine, timeLine;

    int lineNum = 0;
    int numDataLines;

    try
    {
        std::string line;
        bool bFound = false;

        /// @todo: Read lines for time, lat/lon

        while (std::getline(inFile, line))
        {
            lineNum++;

            if (lineNum == 3)
            {
                if (!internal::ParseDateTime(line, cast))
                    throw "Could not parse date/time line";
            }

            // Lat/lon information
            if (line.compare(0, 12, "  Position :") == 0)
            {
                if (!internal::ParseLatLon(line, cast))
                    throw "Could not parse lat/lon line";
            }

            // Number of SSP entries in the file
            if (line.compare(0, 7, "Lines :") == 0)  // Line starts with "Lines :"
            {
                std::stringstream ss(line);
                std::string s1, s2, numLinesStr;
                ss >> s1 >> s2 >> numLinesStr;

                try
                {
                    numDataLines = std::stoi(numLinesStr);
                }
                catch (std::invalid_argument)
                {
                    throw "Invalid number of entries string";
                }

                bFound = true;
                break;
            }
        }

        if (!bFound)
            throw "Missing number of entries line";

        std::getline(inFile, line);  // Skip - only has a ';'

        std::getline(inFile, line);
        auto dataSetVec = SplitString(line);

        if (dataSetVec[1] != "Datasets")
            throw "Data sets not specified";
        dataSetVec.erase(begin(dataSetVec), begin(dataSetVec)+2);

        int speedPos = -1, pressPos = -1, tempPos = -1, salinPos = -1, condPos = -1, sigmaPos = -1;
        for (int n = 0; n < static_cast<int>(dataSetVec.size()); ++n)
        {
            if (dataSetVec[n] == "Press")
                pressPos = n;
            if (dataSetVec[n] == "Temp")
                tempPos = n;
            if (dataSetVec[n] == "Cond")
                condPos = n;
            if (dataSetVec[n] == "SALIN")
                salinPos = n;
            if (dataSetVec[n] == "SIGMA")
                sigmaPos = n;
            if (dataSetVec[n] == "SOUND")
                speedPos = n;
        }

        // Can have 0 or multiple spaces at the beginning
        // Example line: "[ dbar]  [ degC]  [mS/cm]  [  ppt]  [kg/m3]  [  m/s]  [    _]"
        std::regex rgxUnits(R"(\[ *?([a-zA-Z0-9/_]+)\])");
        std::smatch match;
        std::vector<std::string> units;
        std::getline(inFile, line);
        while (std::regex_search(line, match, rgxUnits))
        {
            // Different versions of their files can have either degC or °C
            std::string unit = (match[1] != "degC") ? match[1].str() : "°C";
            units.push_back(unit);

            // Move past the found string to continue searching
            line = match.suffix();
        }

        std::getline(inFile, line);  // Skip - only has a ';'
        lineNum += 4;  // Read 4 lines in the meantime

        while (std::getline(inFile, line))
        {
            lineNum++;
            SCastEntry entry;

            auto entryVec = SplitString(line);
            entryVec.erase(begin(entryVec));  // Each line has an extra entry with the index number
            if (entryVec.size() != dataSetVec.size())
            {
                throw fmt::format("Incomplete line #{}", lineNum);
            }

            entry.depth = lineNum;  /// @todo: Just for testing! Remove and calculate!

            try
            {
                entry.c = std::stod(entryVec[speedPos]);
                entry.temp = std::stod(entryVec[tempPos]);
                entry.salinity = std::stod(entryVec[salinPos]);
                double pressure = std::stod(entryVec[pressPos]);
                entry.pressure = pressure / 10;  // decibar to bar

                // The depth has to be calculated
                double sigma = std::stod(entryVec[sigmaPos]);
                double density = 1000 + sigma;  // https://en.wikipedia.org/wiki/Sigma-t
                entry.depth = Depth(pressure, cast.lat);
            }
            catch (std::invalid_argument)
            {
                throw fmt::format("Invalid number on line #{}", lineNum);
            }

            entries.push_back(entry);
        }

        if (entries.size() != numDataLines)
            throw "Number of entries does not match";
    }
    catch (std::string err)
    {
        std::cout << "Error reading Sea & Sun file (" << fileName << "): " << err << "\n";
        return {};
    }

    cast.lat = 0;
    cast.lon = 0;
    cast.desc = "Sea & Sun (.tob)";
    cast.fileName = fileName;

    return cast;
}

};  // End namespace ssp
