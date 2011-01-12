/**
 * EndianConverter.h
 * 
 * Handles Endian conversion efficiently.
 * 
 * This file is part of Aqua Universal Gesture Recognition Framework.
 * Copyright (C) 2010 Iowa State University.
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
#ifndef _ENDIANCONVERTER_H_
#define _ENDIANCONVERTER_H_

class EndianConverter {

// Attributes

// Methods
public:
    static inline bool isLittleEndian() {
        int myInt = 1;
        if (*((char*) &myInt) == 1) {
            return true;
        } else {
            return false;
        }
    }
    
    static inline int swapIntEndian(int x) {
        union  u {
            int f; 
            char temp[4];
        } un, vn;
        
        un.f = x;
        vn.temp[0] = un.temp[3];
        vn.temp[1] = un.temp[2];
        vn.temp[2] = un.temp[1];
        vn.temp[3] = un.temp[0];
        
        return vn.f;
    }
    
    static inline float swapFloatEndian(float x) {
        union  u {
            float f; 
            char temp[4];
        } un,vn;
        
        un.f = x;
        vn.temp[0] = un.temp[3];
        vn.temp[1] = un.temp[2];
        vn.temp[2] = un.temp[1];
        vn.temp[3] = un.temp[0];
        return vn.f;
    }
    
    static inline unsigned short swapShortEndian(unsigned short x) {
        union  u {
            unsigned short f; 
            char temp[2];
        } un,vn;
        
        un.f = x;
        vn.temp[0] = un.temp[1];
        vn.temp[1] = un.temp[0];
        return vn.f;
    } 
};

#endif
