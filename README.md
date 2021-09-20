[![Windows](https://github.com/DentonW/SspCpp/actions/workflows/windows.yml/badge.svg)](https://github.com/DentonW/SspCpp/actions/workflows/windows.yml)
[![Linux](https://github.com/DentonW/SspCpp/actions/workflows/linux.yml/badge.svg)](https://github.com/DentonW/SspCpp/actions/workflows/linux.yml)

# SspCpp: A C++ Underwater Sound Speed Profile File Reader Library

Underwater conductivity/temperature/depth (CTD) cast equipment can generate one of many output file
types. This is usually dependent on the manufacturer of the equipment. This library seeks to provide
a common interface to load many types of CTD casts to determine the sound speed profile.

SspCpp is loosely modeled after the [HydrOffice Sound Speed](https://github.com/hydroffice/hyo2_soundspeed)
Python library.

## Supported File Formats

These are the currently supported formats, and support for other formats is planned.

* AOML AMVER-SEAS XBT (.txt)
* Kongsberg Maritime (.asvp)
* Hypack (.vel)
* Oceanscience (.asc)
* Sea&Sun (.tob)
* Sea-Bird (.cnv, .tsv)
* SonarDyne (.pro)
* University of New Brunswick (.unb)
* A simple text-based format
