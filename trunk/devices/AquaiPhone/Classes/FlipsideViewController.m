//
//  FlipsideViewController.m
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import "FlipsideViewController.h"


@implementation FlipsideViewController

@synthesize delegate, serverAddress, connectLabel, portPicker;

- (IBAction) serverAddressChanged: (id)sender;
{
	[self setServerAddressToFile:[serverAddress text]];
	[self connectToServer:sender];
}

- (IBAction) connectToServer: (id)sender
{
	//[self setServerAddressToFile:[serverAddress text]];
	[serverAddress resignFirstResponder];
	connectLabel.text = @"Connecting....";
	if(portPicker.selectedSegmentIndex == 0)
	{
		[networkConnection connect: [serverAddress text] portNum: 5945];
	}
	else if(portPicker.selectedSegmentIndex == 1)
	{
		[networkConnection connect: [serverAddress text] portNum: 3007];
	}
	
	connectLabel.text = networkConnection.currentStatus;
	if([networkConnection.currentStatus isEqual:@"Connected"])
	{
		[self done];
	}
}


- (void)viewDidLoad 
{
    [super viewDidLoad];
	connectLabel.text = @"";
	
	//read server address from file
	serverAddress.text = [self getServerAddressFromFile];
	self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor];      
}

- (NSString *)getServerAddressFromFile
{
	NSString *filePath = [[NSBundle mainBundle]pathForResource:@"defaultServerAddress" ofType:@"txt"];
   	NSString *retVal = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil];
	return retVal;
}

- (void)setServerAddressToFile: (NSString *)string
{
	NSString *filePath = [[NSBundle mainBundle]pathForResource:@"defaultServerAddress" ofType:@"txt"];
	[string writeToFile:filePath atomically:YES encoding:NSUTF8StringEncoding error:nil];
}

- (IBAction)done 
{
	[self.delegate flipsideViewControllerDidFinish:self];	
}

- (void) setNetworkConnection: (AquaConnection *)netConn
{
	networkConnection = netConn;
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}


- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	return YES;
}



- (void)dealloc {
    [super dealloc];
}


@end
