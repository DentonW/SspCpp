//#include "pch.h"
#include <iostream>
//#include "../include/SspCpp/SoundSpeed.h"
#include <SspCpp/SoundSpeed.h>
#include "hyo2_location.h"


int main()
{
    //double pDbar = 10.1325;  // 1 atm
    //std::cout << ssp::Depth(pDbar, 0) << "\n";
    //std::cout << ssp::DepthToPressure(100, 0.0) << "\n";  // ~10.04 bar
    //std::cout << ssp::DepthToPressure(200, 0.0) << "\n";  // ~20.08 bar
    //std::cout << ssp::DepthToPressure(305, 0.0) << "\n";  // ~30.62 bar

    std::string hyo2 = HYO2_LOC;


    auto sspAoml1 = ssp::ReadCast(hyo2 + "aoml/_EX1812_AXBT005_181129.txt", ssp::eCastType::Aoml);
    if (!sspAoml1)
        return 1;
    //PlotCast(*sspAoml1);

    auto sspSeaBird7 = ssp::ReadCast(hyo2 + "seabird/007_201605262145_CTD-SB_CON_BIN_DER.cnv", ssp::eCastType::SeaBirdCnv);
    if (!sspSeaBird7)
        return 1;
    //ssp::RemoveNegativeSpeeds(*sspSeaBird7);  // This file has one line with bad data (sound speed of -9.990e-29)
    Cleanup(*sspSeaBird7);
    PlotCast(*sspSeaBird7);

    auto sspSeaSun1 = ssp::ReadCast(hyo2 + "seaandsun/1608240753.TOB", ssp::eCastType::SeaAndSun);
    if (!sspSeaSun1)
        return 1;
    PlotCast(*sspSeaSun1);

    auto sspAsvp1 = ssp::ReadCast(hyo2 + "asvp/_20120321_224205.asvp", ssp::eCastType::Asvp);
    if (!sspAsvp1)
        return 1;
    //PlotCast(*sspSonardyne1);

    auto sspSonardyne1 = ssp::ReadCast(hyo2 + "sonardyne/_Demo1.pro", ssp::eCastType::Sonardyne);
    if (!sspSonardyne1)
        return 1;
    //PlotCast(*sspSonardyne1);

    auto sspSonardyne2 = ssp::ReadCast(hyo2 + "sonardyne/Demo2.pro", ssp::eCastType::Sonardyne);
    if (!sspSonardyne2)
        return 1;
    //PlotCast(*sspSonardyne2);

    auto sspSeaBird1 = ssp::ReadCast(hyo2 + "seabird/_H1662.SVP.sampled.ascent.tsv", ssp::eCastType::SeaBirdTsv);
    if (!sspSeaBird1)
        return 1;
    //PlotCast(*sspSeaBird1);

    auto sspSeaBird2 = ssp::ReadCast(hyo2 + "seabird/_H1662.SVP.sampled.ascent.with_temp_and_sal.tsv", ssp::eCastType::SeaBirdTsv);
    if (!sspSeaBird2)
        return 1;
    //PlotCast(*sspSeaBird2);

    //auto sspSeaBird3 = ssp::ReadCast(hyo2 + "seabird/_2013_192_124923test.cnv", ssp::eCastType::SeaBirdCnv);
    //if (!sspSeaBird3)
    //    return 1;
    ////PlotCast(*sspSeaBird3);
    //Reorder(*sspSeaBird3);
    //RemoveNegativeDepths(*sspSeaBird3);

    auto sspSeaBird4 = ssp::ReadCast(hyo2 + "seabird/FK001_CTD02.cnv", ssp::eCastType::SeaBirdCnv);
    if (!sspSeaBird4)
        return 1;
    //PlotCast(*sspSeaBird4);

    // This one does not load (sound speed not present)
    auto sspSeaBird5 = ssp::ReadCast(hyo2 + "seabird/EX1811_DIVE01_20181031_ROVCTD.cnv", ssp::eCastType::SeaBirdCnv);
    //if (!sspSeaBird5)
    //    return 1;

    auto sspSeaBird6 = ssp::ReadCast(hyo2 + "seabird/ITF17019.cnv", ssp::eCastType::SeaBirdCnv);
    if (!sspSeaBird6)
        return 1;
    Reorder(*sspSeaBird6);
    RemoveDuplicateDepths(*sspSeaBird6);
    PlotCast(*sspSeaBird6);

    //auto sspSeaBird7 = ssp::ReadCast(hyo2 + "seabird/007_201605262145_CTD-SB_CON_BIN_DER.cnv", ssp::eCastType::SeaBirdCnv);
    //if (!sspSeaBird7)
    //    return 1;
    ////PlotCast(*sspSeaBird7);


    // University of New Brunswick
    auto sspUnb1 = ssp::ReadCast(hyo2 + "unb/_WOD_XBTO1114_4012672.unb", ssp::eCastType::Unb);
    if (!sspUnb1)
        return 1;

    auto sspUnb2 = ssp::ReadCast(hyo2 + "unb/_WOD_CTDO1014_3316204.unb", ssp::eCastType::Unb);
    if (!sspUnb2)
        return 1;


    return 0;
}
