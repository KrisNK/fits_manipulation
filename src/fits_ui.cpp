#include "fits_ui.h"
#include <iostream>

fits_ui::fits_ui()
{
    param = new img_param [numImg];

    data = new long* [numImg];
    for (int i{0}; i < numImg; ++i)
        data[i] = new long [param[i].imgSize];

    headers = new header_card* [numImg];
    for (int i{0}; i < numImg; ++i)
        headers[i] = new header_card [param[i].numKeys];
}

fits_ui::~fits_ui()
{
    delete [] data;

    for (int i{0}; i < numImg; ++i)
    {
        delete [] param[i].naxes;
        delete [] param[i].fpixel;
        delete [] headers[i];
    }
    delete [] param;
    delete [] headers;
}

// Private Functions

// Utility Functions

int fits_ui::getDatatype(int *datatype, int index)
{
    switch (param[index].bitpix)
    {
    case (SHORT_IMG):
        *datatype = TSHORT;
        break;
    case (FLOAT_IMG):
        *datatype = TFLOAT;
        break;
    case (USHORT_IMG):
        *datatype = TUSHORT;
        break;
    default:
        *datatype = TBYTE;
        break;
    }
    return 0;
}

int fits_ui::cfits_error()
{
    if (status)
    {
        fits_report_error(stdout, status);
        status = 0;
    }
    return 0;
}

// Array Management

int fits_ui::manageParamArray()
{
    // numImg MUST BE INCREMENTED BEFORE USING THIS FUNCTION
    img_param* buffparam = new img_param [numImg-1];

    for (int i{0}; i < numImg-1; ++i)
        buffparam[i] = param[i];

    delete [] param;
    param = new img_param [numImg];

    for (int i{0}; i < numImg-1; ++i)
        param[i] = buffparam[i];
    delete []  buffparam;

    return 0;
}

int fits_ui::manageDataArray()
{
    // manageParamArray MUST BE USED BEFORE USING THIS FUNCTION
    long** buffdata = new long* [numImg-1];
    for (int i{0}; i < numImg-1; ++i)
        buffdata[i] = new long [param[i].imgSize];

    for (int i{0}; i < numImg-1; ++i)
    {
        for (int j{0}; j < param[i].imgSize; ++j)
            buffdata[i][j] = data[i][j];
    }

    for (int i{0}; i < numImg-1; ++i)
        delete [] data[i];
    delete [] data;

    data = new long* [numImg];
    for (int i{0}; i < numImg; ++i)
        data[i] = new long [param[i].imgSize];

    for (int i{0}; i < numImg-1; ++i)
    {
        for (int j{0}; j < param[i].imgSize; ++j)
            data[i][j] = buffdata[i][j];
    }

    for (int i{0}; i < numImg-1; ++i)
        delete [] buffdata[i];
    delete [] buffdata;
    return 0;
}

int fits_ui::manageHeaderArray()
{
    header_card** buffheaders = new header_card* [numImg-1];
    for (int i{0}; i < numImg-1; ++i)
        buffheaders[i] = new header_card [param[i].numKeys];

    for (int i{0}; i < numImg-1; ++i)
    {
        for (int j{0}; j < param[i].numKeys; ++j)
            buffheaders[i][j] = headers[i][j];
    }

    for (int i{0}; i < numImg-1; ++i)
        delete [] headers[i];
    delete [] headers;

    headers = new header_card* [numImg];
    for (int i{0}; i < numImg; ++i)
        headers[i] = new header_card [param[i].numKeys];

    for (int i{0}; i < numImg-1; ++i)
    {
        for (int j{0}; j < param[i].numKeys; ++j)
            headers[i][j] = buffheaders[i][j];
    }

    for (int i{0}; i < numImg-1; ++i)
        delete [] buffheaders[i];
    delete [] buffheaders;

    return 0;
}

// Extraction Functions (Data, Param, Header)

int fits_ui::extractParam(fitsfile *source)
{
    fits_get_hdrspace(source, &param[numImg-1].numKeys, nullptr, &status);
    cfits_error();

    fits_get_img_equivtype(source, &param[numImg-1].bitpix, &status);
    cfits_error();

    fits_get_img_dim(source, &param[numImg-1].naxis, &status);
    cfits_error();

    param[numImg-1].naxes = new long [param[numImg-1].naxis];
    param[numImg-1].fpixel = new long [param[numImg-1].naxis];
    for (int i{0}; i < param[numImg-1].naxis; ++i)
        param[numImg-1].fpixel[i] = 1;

    fits_get_img_size(source, param[numImg-1].naxis, param[numImg-1].naxes, &status);
    cfits_error();

    for (int i{0}; i < param[numImg-1].naxis; ++i)
    {
        if (i == 0)
            param[numImg-1].imgSize = param[numImg-1].naxes[i];
        else
            param[numImg-1].imgSize *= param[numImg-1].naxes[i];
    }
    return 0;
}

int fits_ui::extractData(fitsfile *source)
{
    int datatype{0};
    getDatatype(&datatype, numImg-1);
    data[numImg-1] = new long [param[numImg-1].imgSize];
    fits_read_pix(source, datatype, param[numImg-1].fpixel, param[numImg-1].imgSize, nullptr, data[numImg-1], nullptr, &status);
    cfits_error();
    return 0;
}

int fits_ui::extractHeader(fitsfile *source)
{
    for (int i{0}; i < param[numImg-1].numKeys; ++i)
    {
        fits_read_record(source, i+1, headers[numImg-1][i].card, &status);
        cfits_error();
    }
    return 0;
}

// Main extraction function (uses all management and extraction functions in order)

int fits_ui::extractAll(fitsfile* source)
{
    int numHDU{0};
    fits_get_num_hdus(source, &numHDU, &status);
    cfits_error();
    for (int i{0}; i < numHDU; ++i)
    {

        fits_movabs_hdu(source, i+1, nullptr, &status);
        cfits_error();
        ++numImg;
        manageParamArray();
        extractParam(source);
        manageHeaderArray();
        extractHeader(source);
        manageDataArray();
        extractData(source);
    }

    return 0;
}

// Public Functions

// Extraction
int fits_ui::extractFITS(std::string *sourcePath)
{
    fitsfile* source;
    fits_open_file(&source, sourcePath->c_str(), READONLY, &status);
    cfits_error();
    extractAll(source);
    fits_close_file(source, &status);
    cfits_error();
    return 0;
}

// Output
int fits_ui::outputImage(std::string* targetPath)
{

    return 0;
}

int fits_ui::outputFrames(std::string* targetPath)
{
    int datatype{0};
    int index {numImg};
    fitsfile* target;

    std::remove(targetPath->c_str());
    fits_create_file(&target, targetPath->c_str(), &status);
    cfits_error();
    fits_open_file(&target, targetPath->c_str(), READWRITE, &status);
    cfits_error();

    for (int i{0}; i < index; ++i)
    {
        std::cout << "Writing HDU #" << i+1 << std::endl;
        fits_create_img(target, param[i].bitpix, param[i].naxis, param[i].naxes, &status);
        cfits_error();

        getDatatype(&datatype, i);
        fits_write_pix(target, datatype, param[i].fpixel, param[i].imgSize, data[i], &status);
        cfits_error();
        for (int j{0}; j < param[i].numKeys; ++j)
        {
            fits_write_record(target, headers[i][j].card, &status);
            cfits_error();
        }
    }

    fits_close_file(target, &status);
    cfits_error();

    return 0;
}

int fits_ui::outputCube(std::string* targetPath)
{

    return 0;
}

// Testing
int fits_ui::printData()
{
    for (int i{0}; i < numImg; ++i)
    {
        std::cout << "Image #" << i+1 << std::endl << std::endl;
        for (int j{0}; j < param[i].imgSize; ++j)
        {
            if (j % 1000000 == 0)
                std::cout << data[i][j] << std::endl;
        }
        if (param[i].imgSize == 0)
            std::cout << "No image data" << std::endl;
        std::cout << std::endl;
        for (int j{0}; j < param[i].numKeys; ++j)
        {
            std::cout << headers[i][j].card << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}
