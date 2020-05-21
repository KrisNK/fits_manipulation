#include "fits_manipulation.h"

#include <iostream>


fits_manipulation::fits_manipulation()
{
    numfits = 1;
    data = new unsigned short* [numfits];
}

fits_manipulation::~fits_manipulation()
{
    // Clean up data store
    for (int i{0}; i < numfits; ++i)
        delete [] data[i];
    delete[] data;

    // Clean up header store
    for (int i{0}; i < keysexist; ++i)
        delete [] header[i];
    delete [] header;
}

int fits_manipulation::manageData()
{
    unsigned short** buff = new unsigned short* [numfits];
    for (int k{0}; k < numfits; ++k)
    {
        buff[k] = new unsigned short [imgSize];
        for (int i{0}; i < imgSize; ++i)
        {
            buff[k][i] = data[k][i];
        }
    }

    for (int i{0}; i < numfits; ++i)
        delete [] data[i];
    delete [] data;

    ++numfits;

    data = new unsigned short* [numfits];

    for (int k{0}; k < numfits-1; ++k)
    {
        data[k] = new unsigned short [imgSize];
        for (int i{0}; i < imgSize; ++i)
        {
            data[k][i] = buff[k][i];
        }
    }
    delete [] buff;
    return 0;
}

int fits_manipulation::errorFits()
{
    char errorStr[30];
    if (status)
    {
        fits_get_errstatus(status, errorStr);
        std::cout << errorStr << std::endl;
        return -1;
    }
    return 0;

}

int fits_manipulation::extractFitsData(std::string* sourcePath)
{
    if (imgSize != 0)
        manageData();

    fitsfile* sourceFile;

    fits_open_file(&sourceFile, sourcePath->c_str(), READONLY, &status);
    fits_get_img_param(sourceFile, MAXAXIS, &param.bitpix, &param.naxis, param.naxes, &status);

    if (imgSize == 0)
        extractFitsHeader(sourceFile);

    if (imgSize == 0)
        imgSize = (param.naxes[0] * param.naxes[1]);
    if (imgSize != (param.naxes[0] * param.naxes[1]))
    {
        std::cout << "[ERROR] image dimensions do not match." << std::endl;
        return -1;
    }

    unsigned short* buff{ new unsigned short[imgSize]{}};

    fits_read_pix(sourceFile, TUSHORT, param.fpixel, imgSize, nullptr, buff, nullptr, &status);

//==========================================================//
//  Problem with cfitsio                                    //
//  bitpix is set to SIGNED short instead of UNSIGNED short //
//  Workaround: Harcode set to UNSIGNED short               //
    param.bitpix = USHORT_IMG;
//==========================================================//

    // Insert algorithm to

    data[numfits-1] = new unsigned short [imgSize];
    for (int ii{0}; ii < imgSize; ++ii)
    {
        data[numfits-1][ii] = buff[ii];
    }

    fits_close_file(sourceFile, &status);

    delete[] buff;
    return 0;
}

int fits_manipulation::writeFitsImage(std::string* targetPath)
{
    unsigned short* targetData = new unsigned short [imgSize];

    // INSERT ALGORITHM HERE?
    for (int i{0}; i < imgSize; ++i)
    {
        for (int k{0}; k < numfits; ++k)
        {
            if ((targetData[i] + data[k][i]) < 65000)
            {
                targetData[i] += data[k][i];
            }
        }
    }
    // END OF ALGORITHM

    std::remove(targetPath->c_str());
    fitsfile* targetFile;
    fits_create_file(&targetFile, targetPath->c_str(), &status);
    fits_create_img(targetFile, param.bitpix, param.naxis, param.naxes, &status);
    fits_open_file(&targetFile, targetPath->c_str(), READWRITE, &status);
    fits_write_pix(targetFile, TUSHORT, param.fpixel, param.naxes[1]*param.naxes[0], targetData, &status);

    for (int i{0}; i < keysexist; ++i)
    {
        fits_write_record(targetFile, header[i], &status);
    }

    fits_close_file(targetFile, &status);

    delete [] targetData;
    std::cout << "FITS file written" << std::endl;
    return 0;
}

int fits_manipulation::printFitsData()
{
    //std::cout << data[0][0] << std::endl;
    //std::cout << data[1][0] << std::endl;
    for (int k{0}; k < numfits; ++k)
    {
        std::cout << "FITS image #" << k+1 << std::endl;
        for (int i{0}; i < imgSize; ++i)
        {
            // Only shows every 100 000th data value for testing purposes
            // All the data was too much to handle lmao
            if (i % 100000 == 0)
                std::cout << (data[k][i]) << std::endl;
        }
    }

    std::cout << "Printed Fits Data" << std::endl;
    std::cout << imgSize * sizeof(unsigned short) * numfits << " bytes of data" << std::endl;
    return 0;
}

int fits_manipulation::extractFitsHeader(fitsfile* file)
{
    fits_get_hdrspace(file, &keysexist, nullptr, &status);

    header = new char* [keysexist];

    for (int i{0}; i < keysexist; ++i)
    {
        header[i] = new char [RECORD_SIZE];
        fits_read_record(file, (i+1), header[i], &status);
        errorFits();
    }
    return 0;
}
