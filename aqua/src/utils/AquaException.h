/**
 * AquaException.h
 * 
 * AquaException serves as a basic exception container which Aqua will use
 * to perform basic exception handling.
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
#ifndef _AQUAEXCEPTION_H_
#define _AQUAEXCEPTION_H_

#include <string>

#define AQUA_SOCKET_EXCEPTION 0

class AquaException {

// Attributes
private:
    int         _type;
    std::string _message;

// Methods
public:
    AquaException(int type, char* msg) {
        _type = type;
        _message = msg;
    }
    
    std::string getMessage() {
        return _message;
    }
    
    int getType() {
        return _type;
    }

};
#endif
