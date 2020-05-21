#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "fits_manipulation.h"

int main()
{
    fits_manipulation fitsfiles;
    std::string target = "/home/kris/Desktop/output.fits";
    std::string path1 = "../res/test.fit";
    std::string path2 = "../res/test2.fit";
    std::string path3 = "../res/test3.fit";
    std::string path4 = "../res/test4.fit";
    std::string path5 = "../res/test5.fit";
    fitsfiles.extractFitsData(&path1);
    fitsfiles.extractFitsData(&path2);
    fitsfiles.extractFitsData(&path3);
    fitsfiles.extractFitsData(&path4);
    fitsfiles.extractFitsData(&path5);
    fitsfiles.writeFitsImage(&target);
}
