/**
 * FileSystem.cpp
 * 
 * This class provides cross-platform filesystem-related utilities for Aqua.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#else
//TODOlinux support
#endif 

#include "FileSystem.h"

using namespace std;

//FileSystem::getSharedLibraryFiles(const char* dir, vector<string> &files);

#ifdef _WIN32
/**
 * Gets all files in the directory that contain the substring ".DLL" or ".dll"
 * anywhere in the filename.  The windows implementation utilizes the 
 * FindFirstFile and FindNextFile functions.
 */
void FileSystem::getSharedLibraryFiles(string dir, vector<string>* files) {
 
    WIN32_FIND_DATA     findData;
    HANDLE              findHandle;
    
    files->clear();
    
    findHandle = FindFirstFile(dir.c_str(), &findData);
    
    if (findHandle == INVALID_HANDLE_VALUE) {
        printf("[FileSystem] Error: Invalid Gesture Directory: %s\n", 
                dir.c_str());
        return;
    }
    
    do {
        string tempString = findData.cFileName;
        if (tempString.find(".dll") != string::npos || 
                tempString.find(".DLL") != string::npos) {
            files->push_back(string(tempString));
        }
    
    } while (FindNextFile(findHandle, &findData) != 0);     
}
#else
void FileSystem::getSharedLibraryFiles(std::string &directory /*in*/, 
        std::vector<std::string> &files /*out*/) {
        
    // TODO linux support  
}
#endif