#import "SoundEffects.h"
@implementation SoundEffects + (id)soundEffectWithContentsOfFile:(NSString *)aPath 
{
	if (aPath) 
	{ 
		return [[[SoundEffects alloc] initWithContentsOfFile:aPath] autorelease]; 
	}
	return nil;
}

// Given a path to a .wav file create a SystemSoundID to reference this path
- (id)initWithContentsOfFile:(NSString *)path 
{ 
	self = [super init];
	if (self != nil) 
	{ 
		NSURL *aFileURL = [NSURL fileURLWithPath:path isDirectory:NO];
		if (aFileURL != nil) 
		{ 
			SystemSoundID aSoundID; 
			OSStatus error = AudioServicesCreateSystemSoundID((CFURLRef)aFileURL, &aSoundID);
			if (error == kAudioServicesNoError) 
			{ 
				_soundID = aSoundID;
			} 
			else 
			{ 
				NSLog(@"Error %d loading sound at path: %@", error, path);
				[self release], self = nil;
			} 
		} 
		else 
		{
			NSLog(@"NSURL is nil for path: %@", path); [self release], self = nil;
		}
	}
	return self;
}
	
-(void)dealloc
{ 
	AudioServicesDisposeSystemSoundID(_soundID);
	[super dealloc];
}
	
-(void)play 
{ 
	AudioServicesPlaySystemSound(_soundID);
}
@end