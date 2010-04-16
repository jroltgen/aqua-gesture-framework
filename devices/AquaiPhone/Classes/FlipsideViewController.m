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

@synthesize delegate, serverAddress, connectLabel;

- (IBAction) serverAddressChanged: (id)sender;
{
	[self connectToServer:sender];
}

- (IBAction) connectToServer: (id)sender
{
	//[serverAddress resignFirstResponder];
	connectLabel.text = @"Connecting....";
	[networkConnection connect: [serverAddress text]];
	connectLabel.text = networkConnection.currentStatus;
}


- (void)viewDidLoad 
{
    [super viewDidLoad];
	connectLabel.text = @"";
	serverAddress.text = @"MultiTouch-PC.student.iastate.edu";
	self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor];      
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
