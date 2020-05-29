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

    int getDatatype(int* datatype);

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

    int extractFITS(std::string* sourcePath);
    int outputFitsImage(std::string* targetPath);
    int outputFitsFrames(std::string* targetPath);
    int outputFitsCube(std::string* targetPath);

    int printData();
};

#endif // FITS_UI_H
