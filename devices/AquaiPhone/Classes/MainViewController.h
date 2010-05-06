//
//  MainViewController.h
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import "FlipsideViewController.h"
#import "SoundEffects.h"

@class SoundEffects;

@interface MainViewController : UIViewController <FlipsideViewControllerDelegate> 
{
	//Array of possable touches
	UITouch *touchArr[10];
	IBOutlet UITextView *logView;
	AquaConnection *networkConnection;
	SoundEffects *upSound;
	SoundEffects *downSound;
}
bool soundsLoaded;
@property (nonatomic, retain) UITextView *logView;
- (void)log: (NSString *)newString;
- (IBAction)showInfo;
- (int)getIDOfTouch: (UITouch *)tp;
- (void)removeTouchFromList: (UITouch *)tp;
- (void)setNetworkConnection: (AquaConnection *)netConn;
@end
