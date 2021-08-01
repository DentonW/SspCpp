//#include "pch.h"
#include <iostream>
//#include "../include/SspCpp/SoundSpeed.h"
#include <SspCpp/SoundSpeed.h>


int main()
{
    double pDbar = 10.1325;  // 1 atm
    std::cout << ssp::Depth(pDbar, 0) << "\n";

    auto sspSeaBird7 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/007_201605262145_CTD-SB_CON_BIN_DER.cnv", ssp::eCastType::SeaBirdCnv);
    if (!sspSeaBird7)
        return 1;
    //ssp::RemoveNegativeSpeeds(*sspSeaBird7);  // This file has one line with bad data (sound speed of -9.990e-29)
    Cleanup(*sspSeaBird7);
    PlotCast(*sspSeaBird7);

    auto sspSeaSun1 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seaandsun/1608240753.tob", ssp::eCastType::SeaAndSun);
    if (!sspSeaSun1)
        return 1;
    PlotCast(*sspSeaSun1);

    auto sspAsvp1 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/asvp/_20120321_224205.asvp", ssp::eCastType::Asvp);
    if (!sspAsvp1)
        return 1;
    //PlotCast(*sspSonardyne1);

    auto sspSonardyne1 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/sonardyne/_Demo1.pro", ssp::eCastType::Sonardyne);
    if (!sspSonardyne1)
        return 1;
    //PlotCast(*sspSonardyne1);

    auto sspSonardyne2 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/sonardyne/Demo2.pro", ssp::eCastType::Sonardyne);
    if (!sspSonardyne2)
        return 1;
    //PlotCast(*sspSonardyne2);

    auto sspSeaBird1 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/_H1662.SVP.sampled.ascent.tsv", ssp::eCastType::SeaBirdTsv);
    if (!sspSeaBird1)
        return 1;
    //PlotCast(*sspSeaBird1);

    auto sspSeaBird2 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/_H1662.SVP.sampled.ascent.with_temp_and_sal.tsv", ssp::eCastType::SeaBirdTsv);
    if (!sspSeaBird2)
        return 1;
    //PlotCast(*sspSeaBird2);

    //auto sspSeaBird3 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/_2013_192_124923test.cnv", ssp::eCastType::SeaBirdCnv);
    //if (!sspSeaBird3)
    //    return 1;
    ////PlotCast(*sspSeaBird3);
    //Reorder(*sspSeaBird3);
    //RemoveNegativeDepths(*sspSeaBird3);

    auto sspSeaBird4 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/FK001_CTD02.cnv", ssp::eCastType::SeaBirdCnv);
    if (!sspSeaBird4)
        return 1;
    //PlotCast(*sspSeaBird4);

    // This one does not load (sound speed not present)
    auto sspSeaBird5 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/EX1811_DIVE01_20181031_ROVCTD.cnv", ssp::eCastType::SeaBirdCnv);
    //if (!sspSeaBird5)
    //    return 1;

    auto sspSeaBird6 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/ITF17019.cnv", ssp::eCastType::SeaBirdCnv);
    if (!sspSeaBird6)
        return 1;
    Reorder(*sspSeaBird6);
    RemoveDuplicateDepths(*sspSeaBird6);
    PlotCast(*sspSeaBird6);

    //auto sspSeaBird7 = ssp::ReadCast("F:/Coding/hyo2_soundspeed/data/input/seabird/007_201605262145_CTD-SB_CON_BIN_DER.cnv", ssp::eCastType::SeaBirdCnv);
    //if (!sspSeaBird7)
    //    return 1;
    ////PlotCast(*sspSeaBird7);

    return 0;
}
