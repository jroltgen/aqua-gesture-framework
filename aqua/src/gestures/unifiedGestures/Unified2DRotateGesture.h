/**
 * Unified2DRotateGesture.h
 *
 * Processes a 2D Rotate gesture.
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
#ifndef _UNIFIEDROTATEGESTURE_H_
#define _UNIFIEDROTATEGESTURE_H_

#include "UnifiedStandardDynamicGesture.h"

#define PI 3.14159265359

class Unified2DRotateGesture : public UnifiedStandardDynamicGesture {

// Attributes
private:

// Methods
public:
    Unified2DRotateGesture(EventProcessor* publisher, int regionID = -1);
    
protected:
    virtual bool processDown(TouchData& data);
    virtual bool processMove(TouchData& data);
    virtual bool processUp  (TouchData& data);
    
private:
    float calculateAngleChange();

};
#endif;
