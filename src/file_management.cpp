#include "file_management.h"

file_management::file_management(std::string* directory)
{
    dir = directory->c_str();
}

file_management::~file_management()
{

}

int file_management::getFitsFiles(std::vector<std::string>* filelist)
{
    DIR* dirp = opendir(dir.c_str());
    if (errno == ENOTDIR)
    {
        std::cout << "[ERROR] Selected directory path is not a directory" << std::endl;
        return -1;
    }
    dirent* dp;
    while ((dp = readdir(dirp)) != nullptr)
    {
        if (strstr(dp->d_name, ".fit") != nullptr )
        {
            filelist->push_back(dir + dp->d_name);
        }
    }
    closedir(dirp);
    return 0;
}
