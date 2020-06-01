#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include <string>
#include <cstring>
#include <vector>
#include <dirent.h>
#include <iostream>

class file_management
{
private:
    std::string dir;
public:
    file_management(std::string* directory);
    ~file_management();
    int getFitsFiles(std::vector<std::string>* filelist);
};

#endif // FILE_MANAGEMENT_H
