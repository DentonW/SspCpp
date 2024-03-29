# SspCpp: A C++ Underwater Sound Speed Profile File Reader Library

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [1.7.1] - 2023-01-02

### Changed

- Changes to CMake scripts to make it easier to include in other projects


## [1.7.0] - 2023-01-02

### Changed

- Added proper export decorations to API functions
- Better CMake install locations


## [1.6.4] - 2021-09-21

### Fixed

- Oceanscience reader compilation issue


## [1.6.3] - 2021-09-21

### Added

- Beginning of OceanScience reader

### Fixed

- Incorrect install target


## [1.6.2] - 2021-09-13

### Added

- Conductivity -> salinity function


## [1.6.1] - 2021-09-12

### Fixed

- Corrected determining sound speed file type from extension


## [1.6.0] - 2021-09-09

### Added

- Reader for a simple text sound speed file


## [1.5.1] - 2021-09-09

### Added

- Determining type based on filename extension


## [1.5.0] - 2021-09-06

### Added

- Hypack (.vel) file support


## [1.4.2] - 2021-08-29

### Fixed

- Incorrect include for LatLong.h


## [1.4.1] - 2021-08-29

### Added

- More checks for physically realizable values


## [1.4.0] - 2021-08-29

### Added

- matplotlib-cpp support for plotting SSPs


## [1.3.0] - 2021-08-28

### Added

- University of New Brunswick (.unb) file support
- Check for valid latitude/longitude values
