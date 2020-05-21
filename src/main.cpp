#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "fits_manipulation.h"

int main()
{
    fits_manipulation fitsfiles;
    std::string target = "~/Desktop/output.fits";
    std::string path1 = "../test.fit";
    std::string path2 = "test2.fit";
    std::string path3 = "test3.fit";
    std::string path4 = "test4.fit";
    std::string path5 = "test5.fit";
    fitsfiles.extractFitsData(&path1);
    fitsfiles.extractFitsData(&path2);
    fitsfiles.extractFitsData(&path3);
    fitsfiles.extractFitsData(&path4);
    fitsfiles.extractFitsData(&path5);
    fitsfiles.writeFitsImage(&target);
}
