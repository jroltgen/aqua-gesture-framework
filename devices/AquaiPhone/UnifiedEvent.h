//
//  UnifiedEvent.h
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import <Foundation/Foundation.h>
#define EVENT_TYPE_DOWN  0
#define EVENT_TYPE_MOVE  1
#define EVENT_TYPE_UP    2
#define EVENT_TYPE_HOVER 3
#define EVENT_TYPE_OTHER 4

@interface UnifiedEvent : NSObject 
{
	NSString	*_name;
	NSString	*_description;
	char		_type;
	int			_ID;
	float		_location[3];
}

//- (UnifiedEvent *) initWithData: (char*) data;
- (UnifiedEvent *) initWithName: (NSString*)name description: (NSString *)desc type: (char)type ID: (int)ID location:(float*)location;
- (char*) serializeData:(short*) lengthOut;
@end
