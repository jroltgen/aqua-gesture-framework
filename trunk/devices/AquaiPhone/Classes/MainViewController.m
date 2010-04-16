//
//  MainViewController.m
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import "MainViewController.h"


@implementation MainViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}


- (void)viewDidLoad 
{
	[super viewDidLoad];
}


-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{	
	//get the width and height
	int width;
	int height;
	if(self.interfaceOrientation == UIInterfaceOrientationPortrait || 
	   self.interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
	{
		width = 320;
		height = 480;
	}
	else if(self.interfaceOrientation == UIInterfaceOrientationLandscapeLeft || 
			self.interfaceOrientation == UIInterfaceOrientationLandscapeRight)
	{
		width = 480;
		height = 320;
	}
	for (UITouch *touch in touches) 
	{
		int ID = [self getIDOfTouch:touch];
		float loc[3];
		loc[0] = [touch locationInView:self.view].x / width;
		loc[1] = [touch locationInView:self.view].y / height;
		loc[2] = 0.0;
		UnifiedEvent *newEvent = [[UnifiedEvent alloc]initWithName: @"UnifiedEvent" description:@"iPhone Event" type: EVENT_TYPE_DOWN ID: ID location: loc];
		NSLog(@"Touch %d Begin X: %f  Y: %f\n", ID, loc[0], loc[1]);
		if([networkConnection sendEvent:newEvent])
		{
			[self removeTouchFromList:touch];
			[self showInfo];
		}
		
		[newEvent release];
		
	}
}
-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{ 	
	//get the width and height
	int width;
	int height;
	if(self.interfaceOrientation == UIInterfaceOrientationPortrait || 
	   self.interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
	{
		width = 320;
		height = 480;
	}
	else if(self.interfaceOrientation == UIInterfaceOrientationLandscapeLeft || 
			self.interfaceOrientation == UIInterfaceOrientationLandscapeRight)
	{
		width = 480;
		height = 320;
	}
	for (UITouch *touch in touches) 
	{
		int ID = [self getIDOfTouch:touch];
		float loc[3];
		loc[0] = [touch locationInView:self.view].x / width;
		loc[1] = [touch locationInView:self.view].y / height;
		loc[2] = 0.0;
		UnifiedEvent *newEvent = [[UnifiedEvent alloc]initWithName: @"UnifiedEvent" description:@"iPhone Event" type: EVENT_TYPE_MOVE ID: ID location: loc];  
		NSLog(@"Touch %d Move X: %f  Y: %f\n", ID, loc[0], loc[1]);
		
		//if send fails fix it
		if([networkConnection sendEvent:newEvent])
		{
			[self removeTouchFromList:touch];
			[self showInfo];
		}		
		[newEvent release];
	}
}
-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{	
	//get the width and height
	int width;
	int height;
	if(self.interfaceOrientation == UIInterfaceOrientationPortrait || 
	   self.interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
	{
		width = 320;
		height = 480;
	}
	else if(self.interfaceOrientation == UIInterfaceOrientationLandscapeLeft || 
			self.interfaceOrientation == UIInterfaceOrientationLandscapeRight)
	{
		width = 480;
		height = 320;
	}
	for (UITouch *touch in touches) 
	{
		int ID = [self getIDOfTouch:touch];
		float loc[3];
		loc[0] = [touch locationInView:self.view].x / width;
		loc[1] = [touch locationInView:self.view].y / height ;
		loc[2] = 0.0;
		UnifiedEvent *newEvent = [[UnifiedEvent alloc]initWithName: @"UnifiedEvent" description:@"iPhone Event" type: EVENT_TYPE_UP ID: ID location: loc];  
		NSLog(@"Touch %d End X: %f  Y: %f\n", ID, loc[0], loc[1]);
		[self removeTouchFromList:touch];
		if([networkConnection sendEvent:newEvent])
		{
			[self removeTouchFromList:touch];
			[self showInfo];
		}
		[newEvent release];
	}
}

- (void)removeTouchFromList: (UITouch *)tp
{
	for(int i = 0; i < 10; i ++)
	{
		if(touchArr[i] == tp)
		{
			touchArr[i] = nil;
			return;
		}
	}
	NSLog(@"Something bad has happend");
}
//this method will use the touch arr and check to see if its there, return the tp if not
- (int)getIDOfTouch: (UITouch *)tp
{
	for(int i = 0; i < 10; i ++)
	{
		if(touchArr[i] == tp)
		{
			return i;
		}
	}
	//if its not in there, go though again and return first nil spot
	for(int i = 0; i < 10; i ++)
	{
		if(touchArr[i] == nil)
		{
			touchArr[i] = tp;
			return i;
		}
	}
	
	return -1;
}

- (void)setNetworkConnection: (AquaConnection *)netConn
{
	networkConnection = netConn;
}
- (void)flipsideViewControllerDidFinish:(FlipsideViewController *)controller {
    
	[self dismissModalViewControllerAnimated:YES];
}


- (IBAction)showInfo 
{    
	FlipsideViewController *controller = [[FlipsideViewController alloc] initWithNibName:@"FlipsideView" bundle:nil];
	controller.delegate = self;
	
	controller.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
	[self presentModalViewController:controller animated:YES];
	[controller setNetworkConnection: networkConnection];
	[controller release];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	return YES;
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc. that aren't in use.
}


- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

- (void)dealloc 
{
    [super dealloc];
}


@end
