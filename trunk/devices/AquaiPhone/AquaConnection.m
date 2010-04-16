//  AquaConnection.m
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//


#import "AquaConnection.h"


@implementation AquaConnection
@synthesize currentStatus;
- (int) connect:(NSString *)serverAddress
{
	if(connected)
	{
		NSLog(@"Tried to connect when we were already connected, closing");
		currentStatus = @"Tried to connect when we were already connected, closing Connection";
		[self closeConnection];
	}
	
	NSLog(@"Attempting to connect to %@\n", serverAddress);
	currentStatus = @"Attempting to connect...";

	struct hostent *server;
	struct sockaddr_in serv_addr;

	char buffer[1];
	
	//set this to be an input device type
	buffer[0] = 2;
	
	server = gethostbyname([serverAddress UTF8String]);
	if(server == nil)
	{
		NSLog(@"could not find host\n");
		currentStatus = @"Could not find host...right address?";
		return -1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(3007);
	
	//connect
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1) 
	{
		NSLog(@"Could not make a socket.\n");
		currentStatus = @"Could not connect to host...server running?";
		return - 1;
	}
	
	if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
	{
		NSLog(@"Could not connect to host\n");
		currentStatus = @"Could not connect to host...server running?";
		return -1;
	}
	write(sock, buffer, 1);
	NSLog(@"Connection to %@ successful\n", serverAddress);
	currentStatus = @"Connected";
	
	connected = YES;
	return 0;
}

- (int) sendEvent:(UnifiedEvent *)UE
{
	if(!connected)
	{
		NSLog(@"Can't send while not connected");
		return -1;
	}
	//This code was copied from the X11 mouse driver
	char outLength[2];
	short length;
	short tempLength;
	int iResult;
	
	char *eventData = [UE serializeData: &length];
	tempLength = length;
	if([EndianConverter isLittleEndian])
	{
		tempLength = [EndianConverter swapShortEndian: tempLength];
	}
	//Sent the length of the event
	memcpy(outLength, &tempLength, 2);
	
	iResult = write(sock, outLength, 2);
	if(iResult == -1)
	{
		NSLog(@"There was an error sending the length\n");
		[self closeConnection];
		return -1;
	}
	
	
	//send the real event
	iResult = write(sock, eventData, length);
	if(iResult == -1)
	{
		NSLog(@"There was an error sending the event\n");
		[self closeConnection];
		return -1;
	}
	
	//free here
	free(eventData);
	return 0;
}

- (id) init
{
	if([super init])
	{
		connected = NO;
	}
	return self;
}
- (int) closeConnection
{
	NSLog(@"Closing the connection");
	return shutdown(sock, 2);
	connected = NO;
}

@end
