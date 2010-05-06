//
//  AquaConnection.h
//  AquaGiPhoneDriver
//
//  Created by Tony Milosch (amilosch@iastate.edu) on 4/11/10.
//  Copyright 2010 Virtual Reality Applications Center
//  Iowa State University All rights reserved.
//

#import <Foundation/Foundation.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#import "UnifiedEvent.h"
#import "EndianConverter.h"


@interface AquaConnection : NSObject 
{
	int sock;
	bool connected;
	NSString *currentStatus;
}
@property (nonatomic, retain) NSString *currentStatus;

- (int) connect:(NSString *)serverAddress portNum: (int)portNumber;
- (int) sendEvent:(UnifiedEvent *) UE;
- (int) closeConnection;
@end
