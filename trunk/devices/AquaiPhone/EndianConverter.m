//
//  EndianConverter.m
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import "EndianConverter.h"


@implementation EndianConverter

+(bool) isLittleEndian
{
	int myInt = 1;
	if (*((char*) &myInt) ==1)
	{
		return YES;
	}
	else 
	{
		return NO;
	}

}

+(int) swapIntEndian: (int)input
{
	union u 
	{
		int f; 
		char temp[4];
	}un, vn;
	
	un.f = input;
	vn.temp[0] = un.temp[3];
	vn.temp[1] = un.temp[2];
	vn.temp[2] = un.temp[1];
	vn.temp[3] = un.temp[0];
	
	return vn.f;
}

+(float) swapFloatEndian: (float)input
{
	union u 
	{
		float f; 
		char temp[4];
	}un,vn;
	
	un.f = input;
	vn.temp[0] = un.temp[3];
	vn.temp[1] = un.temp[2];
	vn.temp[2] = un.temp[1];
	vn.temp[3] = un.temp[0];
	return vn.f;
}

+(short) swapShortEndian: (short)input
{
	union u 
	{
		unsigned short f; 
		char temp[2];
	}un,vn;
	
	un.f = input;
	vn.temp[0] = un.temp[1];
	vn.temp[1] = un.temp[0];
	return vn.f;
}
@end
