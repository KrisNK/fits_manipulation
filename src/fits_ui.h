#ifndef FITS_UI_H
#define FITS_UI_H

#include "fitsio.h"
#include <string>

struct header_card
{
    char keyname[FLEN_KEYWORD];
    char value[FLEN_VALUE];
    char comment[FLEN_COMMENT];
};

struct img_param
{
    int bitpix;
    int naxis;
    long* naxes;
    long* fpixel;
    long imgSize;
};

class fits_ui
{
private:

    int numImg{0};
    int status{0};
    long** data;
    img_param* param;

    int getDatatype(int* datatype);

    int extractData(fitsfile* source);
    int extractParam(fitsfile* source);

    int extractAll(fitsfile* source);
    int manageDataArray();
    int manageParamArray();

public:
    fits_ui();
    ~fits_ui();

    int extractFITS(std::string* sourcePath);
    int outputFitsImage(std::string* targetPath);
    int outputFitsFrames(std::string* targetPath);
    int outputFitsCube(std::string* targetPath);
};

#endif // FITS_UI_H
