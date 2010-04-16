//
//  AquaGiPhoneDriverAppDelegate.h
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AquaConnection.h"

@class MainViewController;

@interface AquaGiPhoneDriverAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    MainViewController *mainViewController;
	AquaConnection *networkConnection;
	
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) MainViewController *mainViewController;

@end

