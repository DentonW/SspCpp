#include "pch.h"
#include <fmt/format.h>
#include "Readers/TimeStruct.h"
#include "SoundSpeed.h"


TEST(Times, CreateTimeTest)
{
    auto time = ssp::CreateTime(2021, 7, 7, 22, 25, 0);
    EXPECT_EQ(time.tm_yday, 187);
    EXPECT_EQ(time.tm_wday, 3);
    EXPECT_EQ(time.tm_isdst, 1);

    return;
}


TEST(SspCalc, WongZhu)
{
    // ssp::WongZhu(temp, salin, pressure)

    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 0),    1435.790);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 100),  1452.012);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 200),  1468.623);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 300),  1485.579);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 400),  1502.838);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 500),  1520.359);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 600),  1538.099);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 700),  1556.016);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 800),  1574.067);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 900),  1592.212);
    EXPECT_FLOAT_EQ(ssp::WongZhu(0, 25, 1000), 1610.407);

    EXPECT_FLOAT_EQ(ssp::WongZhu(20, 35, 400), 1587.932);

    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 0), 1568.141);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 100), 1585.006);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 200), 1601.841);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 300), 1618.619);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 400), 1635.310);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 500), 1651.885);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 600), 1668.316);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 700), 1684.574);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 800), 1700.629);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 900), 1716.453);
    EXPECT_FLOAT_EQ(ssp::WongZhu(40, 40, 1000), 1732.017);

    return;
}
