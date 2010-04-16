//
//  MainViewController.h
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import "FlipsideViewController.h"

@interface MainViewController : UIViewController <FlipsideViewControllerDelegate> 
{
	//Array of possable touches
	UITouch *touchArr[10];
	AquaConnection *networkConnection;
}

- (IBAction)showInfo;
- (int)getIDOfTouch: (UITouch *)tp;
- (void)removeTouchFromList: (UITouch *)tp;
- (void)setNetworkConnection: (AquaConnection *)netConn;
@end
