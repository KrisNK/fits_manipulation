# fits_manipulation
C++ class for basic manipulation of FITS files

## Prerequisites

This basic class requires the CFITSIO library developped by Dr. William D. Pence from NASA.
https://heasarc.gsfc.nasa.gov/docs/software/fitsio/fitsio.html

## Description

This basic class for the manipulation of FITS image files can extract the HDUs of individual files to then manipulate the data and headers.

## Functions

#### extractFITS(std::string* sourcePath)
* Extracts and stores the image data and headers of every HDU in the FITS file.
* Returns 0 (and eventually -1 when error management is better implemented).


#### printData()
* Prints the headers and the value of every 100 000th pixel of each HDU
