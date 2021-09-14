#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <SspCpp/LatLong.h>
#include <SspCpp/SoundSpeed.h>
#include "../src/TimeStruct.h"


TEST_CASE("Time creation test", "[times]")
{
    auto time = ssp::CreateTime(2021, 7, 7, 22, 25, 0);
    REQUIRE(time.tm_yday == 187);
    REQUIRE(time.tm_wday == 3);
    REQUIRE(time.tm_isdst == 1);

    return;
}


TEST_CASE("Wong-Zhu equation", "[ssp-calc]")
{
    // ssp::WongZhu(temp, salin, pressure)

    REQUIRE(ssp::WongZhu(0, 25, 0) == Approx(1435.790));
    REQUIRE(ssp::WongZhu(0, 25, 100) == Approx(1452.012));
    REQUIRE(ssp::WongZhu(0, 25, 200) == Approx(1468.623));
    REQUIRE(ssp::WongZhu(0, 25, 300) == Approx(1485.579));
    REQUIRE(ssp::WongZhu(0, 25, 400) == Approx(1502.838));
    REQUIRE(ssp::WongZhu(0, 25, 500) == Approx(1520.359));
    REQUIRE(ssp::WongZhu(0, 25, 600) == Approx(1538.099));
    REQUIRE(ssp::WongZhu(0, 25, 700) == Approx(1556.016));
    REQUIRE(ssp::WongZhu(0, 25, 800) == Approx(1574.067));
    REQUIRE(ssp::WongZhu(0, 25, 900) == Approx(1592.212));
    REQUIRE(ssp::WongZhu(0, 25, 1000) == Approx(1610.407));

    REQUIRE(ssp::WongZhu(20, 35, 400) == Approx(1587.932));

    REQUIRE(ssp::WongZhu(40, 40, 0) == Approx(1568.141));
    REQUIRE(ssp::WongZhu(40, 40, 100) == Approx(1585.006));
    REQUIRE(ssp::WongZhu(40, 40, 200) == Approx(1601.841));
    REQUIRE(ssp::WongZhu(40, 40, 300) == Approx(1618.619));
    REQUIRE(ssp::WongZhu(40, 40, 400) == Approx(1635.310));
    REQUIRE(ssp::WongZhu(40, 40, 500) == Approx(1651.885));
    REQUIRE(ssp::WongZhu(40, 40, 600) == Approx(1668.316));
    REQUIRE(ssp::WongZhu(40, 40, 700) == Approx(1684.574));
    REQUIRE(ssp::WongZhu(40, 40, 800) == Approx(1700.629));
    REQUIRE(ssp::WongZhu(40, 40, 900) == Approx(1716.453));
    REQUIRE(ssp::WongZhu(40, 40, 1000) == Approx(1732.017));

    return;
}


TEST_CASE("Latitude-longitude setting", "[lat-long]")
{
    ssp::SLatLong s;
    s.lat = 10;
    s.lon = -30;

    REQUIRE(s.latitude == 10);
    REQUIRE(s.longitude == -30);

    return;
}


TEST_CASE("Latitude-longitude conversions", "[lat-long]")
{
    REQUIRE(ssp::LatLonToDecimal(36, 10, 27.5) == Approx(36.1743055));
    REQUIRE(ssp::LatLonToDecimal(-36, 10, 27.5) == Approx(-36.1743055));

    double d, m, s;
    REQUIRE(ssp::LatLonFromDecimal(36.1743055, d, m, s) == true);
    REQUIRE(d == Approx(36));
    REQUIRE(m == Approx(10));
    REQUIRE(s == Approx(27.5));

    REQUIRE(ssp::LatLonFromDecimal(-36.1743055, d, m, s) == true);
    REQUIRE(d == Approx(-36));
    REQUIRE(m == Approx(10));
    REQUIRE(s == Approx(27.5));

    double dec = ssp::LatLonToDecimal(-82, 3, 54.617);
    ssp::LatLonFromDecimal(dec, d, m, s);
    REQUIRE(d == Approx(-82));
    REQUIRE(m == Approx(3));
    REQUIRE(s == Approx(54.617));

    dec = ssp::LatLonToDecimal(65, 0, 54.617);
    ssp::LatLonFromDecimal(dec, d, m, s);
    REQUIRE(d == Approx(65));
    REQUIRE(m == Approx(0));
    REQUIRE(s == Approx(54.617));

    return;
}



TEST_CASE("Conductivity to salinity", "[salinity]")
{
    // Values from referenced paper, pages 13-14
    REQUIRE(ssp::ConductivityToSalinity(1.0 * 4.2914, 0, 15) == Approx(35.0));
    REQUIRE(ssp::ConductivityToSalinity(0.6990725 * 4.2914, 0, 0) == Approx(36.2864));
    REQUIRE(ssp::ConductivityToSalinity(0.6990725 * 4.2914, 0, 10) == Approx(26.8609));
    REQUIRE(ssp::ConductivityToSalinity(0.6990725 * 4.2914, 0, 20) == Approx(20.8085));
    REQUIRE(ssp::ConductivityToSalinity(1.165120 * 4.2914, 1000, 20) == Approx(36.3576));
    REQUIRE(ssp::ConductivityToSalinity(1.3981451 * 4.2914, 2000, 30) == Approx(35.5783));

    return;
}