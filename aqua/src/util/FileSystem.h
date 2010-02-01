/**
 * FileSystem.h
 * 
 * This class provides cross-platform filesystem-related utilities for Aqua.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _FILESYS_H_
#define _FILESYS_H_

#include <string>
#include <vector>

class FileSystem {

// Attributes

// Methods
public:
    static void getSharedLibraryFiles(std::string directory, 
            std::vector<std::string>* files);


};

#endif