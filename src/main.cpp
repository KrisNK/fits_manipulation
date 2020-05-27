#include <iostream>
#include "fits_ui.h" // MAJOR BUG SEE fits_ui.h for more info

int main()
{
    // Paths for test files
    std::string path1 = "/home/kris/Desktop/test_o.fits";    // Multiple frames
    std::string path2 = "/home/kris/Desktop/testfits/2483142r.fits";    // Fits cube
    std::string path3 = "/home/kris/Projects/FITS/fits_manipulation/res/test.fit";            // Single fits image
    std::string path4 = "/home/kris/Projects/FITS/fits_manipulation/res/test2.fit";
    std::string path5 = "/home/kris/Projects/FITS/fits_manipulation/res/test3.fit";

    std::string target = "/home/kris/Desktop/output.fits";

    //fits_ui
    fits_ui test;
    //test.extractFITS(&path1);
    test.extractFITS(&path4);
    test.extractFITS(&path5);
    test.printData();
    //test.outputFitsImage(&target);
}
