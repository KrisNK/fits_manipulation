#ifndef FITS_UI_H
#define FITS_UI_H

#include "fitsio.h"
#include <string>

struct header_card
{
    char card[FLEN_CARD];
};

struct img_param
{
    int bitpix;
    int naxis;
    long* naxes;
    long* fpixel;
    long imgSize;
    int numKeys;
};

class fits_ui
{
private:

    int numImg{0};
    int status{0};
    long** data;
    img_param* param;
    header_card** headers;

    int getDatatype(int* datatype, int index);

    int cfits_error();

    int extractData(fitsfile* source);
    int extractParam(fitsfile* source);
    int extractHeader(fitsfile* source);

    int extractAll(fitsfile* source);
    int manageDataArray();
    int manageParamArray();
    int manageHeaderArray();

public:
    fits_ui();
    ~fits_ui();

    // Extraction
    int extractFITS(std::string* sourcePath);

    // Output
    int outputImage(std::string* targetPath);
    int outputFrames(std::string* targetPath);
    int outputCube(std::string* targetPath);

    // Testing
    int printData();
};

#endif // FITS_UI_H
