//
//  FlipsideViewController.h
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AquaConnection.h"

@protocol FlipsideViewControllerDelegate;


@interface FlipsideViewController : UIViewController 
{
	id <FlipsideViewControllerDelegate> delegate;
	IBOutlet UILabel *connectLabel;
	IBOutlet UITextField *serverAddress;
	AquaConnection *networkConnection;
	
}

@property (nonatomic, retain) UITextField *serverAddress;
@property (nonatomic, retain) UILabel *connectLabel;


- (IBAction) serverAddressChanged: (id)sender;
- (IBAction) connectToServer: (id)sender;
- (void) setNetworkConnection: (AquaConnection *)netConn;

@property (nonatomic, assign) id <FlipsideViewControllerDelegate> delegate;
- (IBAction)done;

@end


@protocol FlipsideViewControllerDelegate
- (void)flipsideViewControllerDidFinish:(FlipsideViewController *)controller;
@end

