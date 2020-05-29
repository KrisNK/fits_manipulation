#include <iostream>
#include "fits_ui.h"

int main()
{
    // Paths for test files
    // Files on my laptop
    std::string path1 = "/home/kris/Desktop/test_o.fits";    // Multiple frames
    std::string path2 = "/home/kris/Desktop/testfits/2483142r.fits";    // Fits cube
    std::string path3 = "/home/kris/Projects/FITS/fits_manipulation/res/test.fit";            // Single fits image
    std::string path4 = "/home/kris/Projects/FITS/fits_manipulation/res/test2.fit";
    std::string path5 = "/home/kris/Projects/FITS/fits_manipulation/res/test3.fit";

    // Files on my desktop
    std::string test1 = "/home/kris/Projects/C++/fits_manipulation/res/test.fit";
    std::string test2 = "/home/kris/Projects/C++/fits_manipulation/res/test2.fit";
    std::string test3 = "/home/kris/Projects/C++/fits_manipulation/res/test3.fit";
    std::string test4 = "/home/kris/Projects/C++/fits_manipulation/res/test4.fit";
    std::string test5 = "/home/kris/Projects/C++/fits_manipulation/res/test5.fit";

    std::string target = "/home/kris/Desktop/output.fits";

    //fits_ui
    fits_ui test;

    test.extractFITS(&test1);
    test.extractFITS(&test2);
    test.extractFITS(&test3);
    test.extractFITS(&test4);
    test.extractFITS(&test5);

    test.printData();

    test.outputFitsImage(&target);
}
