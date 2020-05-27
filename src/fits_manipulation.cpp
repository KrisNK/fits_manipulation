#include "fits_manipulation.h"

#include <iostream>


fits_manipulation::fits_manipulation()
{
    numfits = 0;
    data = new unsigned short* [numfits];
    for (int i{0}; i < numfits; ++i)
    {
        data[i] = new unsigned short [imgSize];
    }
}

fits_manipulation::~fits_manipulation()
{
    // Clean up data store
    for (int i{0}; i < numfits; ++i)
        delete [] data[i];
    delete[] data;

    // Clean up header store
    for (int i{0}; i < numKeys; ++i)
        delete [] header[i];
    delete [] header;
}

int fits_manipulation::manageFitsData()
{
    // Create buffers
    unsigned short** buffData = new unsigned short* [numfits];
    for (int i{0}; i < numfits; ++i)
    {
        buffData[i] = new unsigned short [imgSize];
    }
    // Copy arrays to buffers
    for (int i{0}; i < numfits; ++i)
    {
        for (int j{0}; j < imgSize; ++j)
        {
            buffData[i][j] = data[i][j];
        }
    }

    // Delete old arrays
    for (int i{0}; i < numfits; ++i)
    {
        delete [] data[i];
    }
    delete [] data;

    // Increment numfits
    ++numfits;

    // Create new arrays
    data = new unsigned short* [numfits];
    for (int i{0}; i < numfits; ++i)
    {
        data[i] = new unsigned short [imgSize];
    }

    // Copy buffers to new arrays
    for (int i{0}; i < numfits-1; ++i)
    {
        for (int j{0}; j < imgSize; ++j)
        {
            data[i][j] = buffData[i][j];
        }
    }

    // Delete buffers
    for (int i{0}; i < numfits-1; ++i)
    {
        delete [] buffData[i];
    }
    delete [] buffData;

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
    int numHDU{0};
    int _numKeys{0};
    fitsfile* source;
    fits_open_file(&source, sourcePath->c_str(), READONLY, &status);
    fits_get_num_hdus(source, &numHDU, &status);
    for (int i{0}; i < numHDU; ++i)
    {
        fits_movabs_hdu(source, (i+1), nullptr, &status);
        fits_get_img_param(source, MAXAXIS, &param.bitpix, &param.naxis, param.naxes, &status);
        if (imgSize == 0)
            imgSize = (param.naxes[0] * param.naxes[1]);
        if (imgSize != (param.naxes[0] * param.naxes[1]))
        {
            std::cout << "[ERROR] image dimensions do not match." << std::endl;
            return -1;
        }
        fits_get_hdrspace(source, &_numKeys, nullptr, &status);
        if (_numKeys > numKeys) { numKeys = _numKeys; }
        std::cout << "test" << std::endl;
        manageFitsData();
        std::cout << "test" << std::endl;
        extractData(source);
        //extractHeader(source);
    }
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

    for (int i{0}; i < numKeys; ++i)
    {
        fits_write_record(targetFile, *header[i], &status);
    }

    fits_close_file(targetFile, &status);

    delete [] targetData;
    std::cout << "FITS file written" << std::endl;
    return 0;
}

int fits_manipulation::printFitsData()
{
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
        for (int i{0}; i < numKeys; ++i)
        {
            //std::cout << header[k][i] << std::endl;
        }
    }

    std::cout << "Printed Fits Data" << std::endl;
    std::cout << imgSize * sizeof(unsigned short) * numfits << " bytes of data" << std::endl;
    return 0;
}

int fits_manipulation::extractData(fitsfile *source)
{
    unsigned short* buff{ new unsigned short[imgSize]{}};

    fits_read_pix(source, TUSHORT, param.fpixel, imgSize, nullptr, buff, nullptr, &status);

    param.bitpix = USHORT_IMG;

    data[numfits-1] = new unsigned short [imgSize];
    for (int ii{0}; ii < imgSize; ++ii)
    {
        data[numfits-1][ii] = buff[ii];
    }

    delete[] buff;
    return 0;
}

int fits_manipulation::extractHeader(fitsfile *source)
{
    for (int i{0}; i < numKeys; ++i)
    {
        fits_read_record(source, (i+1), header[numfits-1][i], &status);
        std::cout << status << std::endl;
        errorFits();
    }
    return 0;
}
