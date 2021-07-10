#include "pch.h"
#include "Asvp.h"
#include "StringUtilities.h"
#include "TimeStruct.h"


std::optional<ssp::SCast> ssp::ReadAsvp(const std::string& fileName)
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

    try
    {
        if (!std::getline(inFile, line))  // Header
            throw "Line read failure";

        auto headVec = SplitString(line);
        if (headVec.size() < 7)
            throw "Could not parse header";
        if (headVec[1] != "SoundVelocity")
            throw "Wrong type of data";

        /// @todo: Move header parsing into a separate function

        try
        {
            // Convert date/time string
            std::string timeStr = headVec[4];
            if (timeStr.size() == 12 || timeStr.size() == 14)
            {
                // %Y%m%d%H%M or %Y%m%d%H%M%S
                int year   = std::stoi(timeStr.substr(0, 4));
                int month  = std::stoi(timeStr.substr(4, 2));
                int day    = std::stoi(timeStr.substr(6, 2));
                int hour   = std::stoi(timeStr.substr(8, 2));
                int minute = std::stoi(timeStr.substr(10, 2));
                int second = 0;
                if (timeStr.size() == 14)
                    second = std::stoi(timeStr.substr(12, 2));

                cast.time = CreateTime(year, month, day, hour, minute, second);
            }
            else
            {
                throw "Header time invalid format";
            }

            // Convert latitude/longitutde strings
            std::string latStr = headVec[5];
            std::string lonStr = headVec[6];
            cast.lat = std::stod(latStr);
            cast.lon = std::stod(lonStr);
        }
        catch (std::invalid_argument)
        {
            throw "Invalid latitude/longitude strings";
        }

        while (std::getline(inFile, line))
        {
            if (line.size() == 0)
                break;

            // The depth and sound speed are required fields...
            std::stringstream ss(line);
            SCastEntry entry;
            ss >> entry.depth >> entry.c;
            if (ss.fail())
                throw fmt::format("Incomplete entry for line #{}", entries.size() + 2);

            entries.push_back(entry);
        }
    }
    catch (std::string err)
    {
        std::cout << "Error reading Kongsberg Maritime file (" << fileName << "): " << err << "\n";
        return {};
    }

    cast.lat = 0;
    cast.lon = 0;
    cast.desc = "Kongsberg Maritime (.asvp)";
    cast.fileName = fileName;

    return cast;
}
