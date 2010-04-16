//
//  UnifiedEvent.m
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import "EndianConverter.h"
#import "UnifiedEvent.h"


@implementation UnifiedEvent

//- (UnifiedEvent *) initWithData:(char *)data
//{
//	//normal event data stuff;
//	int i;
//	char *ptr = data;
//	
//	//get the name up till the null
//	char * cStr = ptr;
//	ptr += (strlen(cStr) + 1);
//	[_name initWithUTF8String:cStr];
//	
//	//do the same for the description
//	cStr = ptr;
//	ptr += (strlen(cStr) + 1);
//	[_description initWithUTF8String:cStr];
//	
//	//get the type
//	_type = *ptr;
//	ptr ++;
//	
//	//get the ID
//	memcpy(&_ID, ptr, 4);
//	ptr += 4;
//	
//	//get location
//	memcpy(_location, ptr, 12);
//	
//	//swap, yeah iPhone is little
//	if([EndianConverter isLittleEndian])
//	{
//		_ID = [EndianConverter swapIntEndian: _ID];
//		for(i = 0; i < 3; i ++)
//		{
//			_location[i] = [EndianConverter swapFloatEndian: _location[i]];
//		}
//	}
//	//don't have to init any member variables
//	return self;
//}

- (UnifiedEvent *) initWithName:(NSString *)name description:(NSString *)desc type:(char)type ID:(int)ID location:(float*) location
{
	//do standard event stuff
	_name = name;
	_description = desc;
	_type = type;
	_ID = ID;
	_location[0] = location[0];
	_location[1] = location[1];
	_location[2] = location[2];
	
	//no init on member variables
	return self;
}

- (char*) serializeData:(short*) lengthOut
{
	int i;
	const char *cName = [_name cStringUsingEncoding:NSASCIIStringEncoding];
	const char *cDesc = [_description cStringUsingEncoding:NSASCIIStringEncoding];
	
	*lengthOut = (strlen(cName) + strlen(cDesc) + 2 + 17);
	
	int tempInt;
	float tempFloat;
	
	char *ret = malloc(sizeof(char) * (*lengthOut));
	
	//zero out the memory...Do I need this
	bzero(ret, *lengthOut);
	
	char *bufferPtr = ret;
	
	//name
	memcpy(bufferPtr, cName, strlen(cName));
	bufferPtr += strlen(cName) +1;
	
	//desc
	memcpy(bufferPtr, cDesc, strlen(cDesc));
	bufferPtr += strlen(cDesc) +1;
	
	//type
	*bufferPtr = _type;
	bufferPtr ++;
	
	//ID
	tempInt = _ID;
	if([EndianConverter isLittleEndian])
	{
		tempInt = [EndianConverter swapIntEndian:tempInt]; 
	}
	memcpy(bufferPtr, &tempInt, 4);
	bufferPtr += 4;
	
	//location
	for(i = 0; i < 3; i ++)
	{
		tempFloat = _location[i];
		if([EndianConverter isLittleEndian])
		{
			tempFloat = [EndianConverter swapFloatEndian:tempFloat];
		}
		memcpy(bufferPtr, &tempFloat, 4);
		bufferPtr += 4;
	}

	return ret;
}
@end
