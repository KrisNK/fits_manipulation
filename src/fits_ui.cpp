#include "fits_ui.h"
#include <iostream>

fits_ui::fits_ui()
{
    data = new long* [numImg];
    param = new img_param [numImg];
    for (int i{0}; i < numImg; ++i)
        data[i] = new long [param[i].imgSize];
}

fits_ui::~fits_ui()
{
    delete [] data;

    for (int i{0}; i < numImg; ++i)
    {
        delete [] param[i].naxes;
        delete [] param[i].fpixel;
    }
    delete [] param;
}

// Private Functions

// Utility Functions

int fits_ui::getDatatype(int *datatype)
{
    switch (param[numImg-1].bitpix)
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

// Extraction Functions (Data, Param, Header)

int fits_ui::extractParam(fitsfile *source)
{
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
    getDatatype(&datatype);
    data[numImg-1] = new long [param[numImg-1].imgSize];
    fits_read_pix(source, datatype, param[numImg-1].fpixel, param[numImg-1].imgSize, nullptr, data[numImg-1], nullptr, &status);
    cfits_error();
    return 0;
}




// Launch Functions

int fits_ui::extractAll(fitsfile* source)
{
    int numHDU{0};
    fits_get_num_hdus(source, &numHDU, &status);
    cfits_error();
    std::cout << "File contains " << numHDU << " HDU(s)" << std::endl;
    for (int i{0}; i < numHDU; ++i)
    {

        fits_movabs_hdu(source, i+1, nullptr, &status);
        cfits_error();
        ++numImg;
        manageParamArray();
        extractParam(source);
        manageDataArray();
        extractData(source);
    }

    return 0;
}

// Public Functions

int fits_ui::extractFITS(std::string *sourcePath)
{
    fitsfile* source;
    fits_open_file(&source, sourcePath->c_str(), READONLY, &status);
    cfits_error();
    extractAll(source);
    fits_close_file(source, &status);
    cfits_error();
    std::cout << "FITS file extracted" << std::endl;
    return 0;
}

int fits_ui::outputFitsImage(std::string *targetPath)
{
    int datatype{0};
    // create new img_param variable
    // create function which decides the ideal values for the struct
    // Populate the rest of this function with said values
    getDatatype(&datatype);
    std::remove(targetPath->c_str());
    fitsfile* target;
    fits_create_file(&target, targetPath->c_str(), &status);
    fits_open_file(&target, targetPath->c_str(), READWRITE, &status);
    fits_create_img(target, param[numImg-1].bitpix, param[numImg-1].naxis, param[numImg-1].naxes, &status);


    long* output = new long [param[numImg-1].imgSize];
    for (int i{0}; i < numImg; ++i)
    {
        for (int j{0}; j < param[numImg-1].imgSize; ++j)
        {
            output[j] = data[i][j];
        }
    }

    fits_write_pix(target, datatype, param[numImg-1].fpixel, param[numImg-1].imgSize, output, &status);

    fits_close_file(target, &status);

    delete [] output;

    return 0;
}

int fits_ui::printData()
{
    for (int i{0}; i < numImg; ++i)
    {
        std::cout << "Image #" << i+1 << std::endl << std::endl;
        for (int j{0}; j < param[i].imgSize; ++j)
        {
            if (j % 100000 == 0)
                std::cout << data[i][j] << std::endl;
        }
        if (param[i].imgSize == 0)
            std::cout << "No image data" << std::endl;
        std::cout << std::endl;
    }
    return 0;
}
