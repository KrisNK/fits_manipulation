#include <iostream>
#include "fits_ui.h"

int main()
{
    // Paths for test files
    std::string path1 = "/home/kris/Desktop/testfits/2483142o.fits";    // Multiple frames
    std::string path2 = "/home/kris/Desktop/testfits/2483142r.fits";    // Fits cube
    std::string path3 = "../fits_manipulation/res/test.fit";            // Single fits image

    std::string target = "/home/kris/Desktop/output.fits";

    //fits_ui
    fits_ui test;
    test.extractFITS(&path1);
    test.outputFitsImage(&target);
}
