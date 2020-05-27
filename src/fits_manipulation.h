#ifndef FITS_MANIPULATION_H
#define FITS_MANIPULATION_H

#include <string>
#include <fitsio.h>

#define MAXAXIS 2

//  Read FITS project status
//  Currently can:
//      Read, save and write data
//      Read, save and write headers
//      Dynamically accept any number of images
//
//  TO DO:
//      Read, save and write ASCII table data ??? Do research on what ASCII table is if necessary
//      Better file management (overwrite, figure out how many files in folder, etc)
//      ***Save images with multiple extensions
//
//  LIMITATIONs:
//      Data type currently hardcoded to 16 bit unsigned short
//      Header data is from the first image. More info needed to determine what to do with header

struct fits_1d_img_data
{
    int bitpix{0};
    int naxis{0};
    long naxes[1]{0};
    long fpixel[1]{1};
};

struct fits_2d_img_data
{
    int bitpix{0};
    int naxis{0};
    long naxes[2]{0, 0};
    long fpixel[2]{1, 1};
};

struct fits_3d_img_data
{
    int bitpix{0};
    int naxis{0};
    long naxes[3]{0, 0};
    long fpixel[3]{1, 1, 1};
};

class fits_manipulation
{
private:
    int status{0};
    int numKeys{0};
    int numfits{0};     // Saves the number of images to feed to the fits manipulation object
    long imgSize{0};    // Saves the dimensions of the images ( X-Pixels x Y-Pixels)
    fits_2d_img_data param;
    unsigned short** data;      // Double pointer to 2D dynamic array
    char*** header;

    int manageFitsData();
    int errorFits();



public:
    fits_manipulation();
    ~fits_manipulation();

    int extractFitsData(std::string* sourcePath);
    int writeFitsImage(std::string* targetPath);
    int printFitsData(); // For testing
    int extractData(fitsfile* source);
    int extractHeader(fitsfile* source);
};

#endif // FITS_MANIPULATION_H
