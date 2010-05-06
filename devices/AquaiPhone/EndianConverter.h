//
//  EndianConverter.h
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import <Foundation/Foundation.h>


@interface EndianConverter : NSObject 
{

}

+(bool) isLittleEndian;
+(int) swapIntEndian: (int)input;
+(float) swapFloatEndian: (float)input;
+(short) swapShortEndian: (short)input;

@end
