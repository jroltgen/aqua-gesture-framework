/**
 * FileSystem.cpp
 * 
 * This class provides cross-platform filesystem-related utilities for Aqua.
 * 
 * This file is part of Aqua Universal Gesture Recognition Framework.
 * Copyright (C) 2010  Jay Roltgen.
 *
 * Aqua is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#else
#include <sys/types.h>
#include <dirent.h>
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
void FileSystem::getSharedLibraryFiles(string dir /*in*/, 
        vector<string>* files /*out*/) {

    DIR*            d;
    struct dirent*  dEntry;

    if ((d = opendir(dir.c_str())) == NULL) {
        printf("[FileSystem] Error opening directory: %s\n", dir.c_str());
        return;
    }

    while ((dEntry = readdir(d)) != NULL) {
        string tempString(dEntry->d_name);
        if (tempString.find(".so") != string::npos ||
                tempString.find(".SO") != string::npos) {
            files->push_back(string(tempString));
        }
    }
    closedir(d);
}
#endif
