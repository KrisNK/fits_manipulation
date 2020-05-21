#ifndef FITS_MANIPULATION_H
#define FITS_MANIPULATION_H

#include <string>
#include <fitsio.h>

#define MAXAXIS 2
#define RECORD_SIZE 80

//  Read FITS project status
//  Currently can:
//      Read, save and write data
//      Read, save and write headers
//      Dynamically accept any number of images
//
//  TO DO:
//      Read, save and write ASCII table data ??? Do research on what ASCII table is if necessary
//
//  LIMITATIONs:
//      Data type currently hardcoded to 16 bit unsigned short
//      Header data is from the first image. More info needed to determine what to do with header

struct fits_2d_img_data
{
    int bitpix{0};
    int naxis{0};
    long naxes[2]{0, 0};
    long fpixel[2]{1, 1};
};

class fits_manipulation
{
private:
    int status{0};
    int keysexist{0};
    int numfits{0};     // Saves the number of images to feed to the fits manipulation object
    long imgSize{0};    // Saves the dimensions of the images ( X-Pixels x Y-Pixels)
    fits_2d_img_data param;
    unsigned short** data;      // Double pointer to 2D dynamic array
    char** header;

    int manageData();
    int errorFits();

public:
    fits_manipulation();
    ~fits_manipulation();

    int extractFitsData(std::string* sourcePath);
    int extractFitsHeader(fitsfile* file);
    int writeFitsImage(std::string* targetPath);
    int printFitsData(); // For testing
};

#endif // FITS_MANIPULATION_H
