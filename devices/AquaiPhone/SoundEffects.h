//
//  SoundEffects.h
//  ButApp
//
//  Created by Milosch, Anthony M on 3/8/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>


@interface SoundEffects : NSObject 
{
	SystemSoundID _soundID;
}

+(id)soundEffectWithContentsOfFile:(NSString *)aPath;
-(id)initWithContentsOfFile: (NSString *)path;
-(void)play;

@end
