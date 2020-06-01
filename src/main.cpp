#include <iostream>
#include "fits_ui.h"
#include "file_management.h"
int main()
{
    std::vector<std::string> list;
    std::string directory{"/home/kris/Projects/C++/fits_manipulation/res/"};
    std::string targetFits{"/home/kris/Desktop/bruh.fits"};

    file_management folder(&directory);
    if (folder.getFitsFiles(&list) == -1)
        return -1;

    fits_ui image;
    for (auto file : list)
    {
        image.extractFITS(&file);
    }
    image.outputFrames(&targetFits);
    std::cout << "Fits file written" << std::endl;
}
