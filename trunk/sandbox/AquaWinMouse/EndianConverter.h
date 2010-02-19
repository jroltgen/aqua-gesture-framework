/**
 * EndianConverter.h
 * 
 * Handles Endian conversion efficiently.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
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
