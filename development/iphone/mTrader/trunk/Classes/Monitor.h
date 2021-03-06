//
//  mTraderServerMonitor.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "mTraderCommunicator.h"
#import "Communicator.h"
#import "Reachability.h"

enum {
	SETUP = 0,
	DISCONNECTED,
	START,
	CONNECTING,
	CONNECTED,
	LOGGINGIN,
	LOGGEDIN,
	LOGOUT,
	NOUSERNAMEORPASSWORD
} monitorStates;

@class Communicator;
@class StatusController;

@interface Monitor : NSObject <mTraderStatusDelegate, UIAlertViewDelegate, CommunicatorStatusDelegate> {
@private
	Communicator *_communicator;
	Communicator *_mTradingCommunicator;
	mTraderCommunicator *_mTCom;
	
	Reachability *_reachability;
	
	NSURL *_mTraderURL;
	NSString *_mTraderHost;
	NSUInteger _mTraderPort;
	
	NSURL *_mTradingURL;
	NSString *_mTradingHost;
	NSUInteger _mTradingPort;
	
	NSUInteger _state;
	BOOL _connecting;
	BOOL _connected;
	BOOL _loggedIn;
	
	StatusController *_statusController;
}

@property (readonly) NSString *mTraderHost;
@property (readonly) NSUInteger mTraderPort;
@property (readonly) NSString *mTradingHost;
@property (readonly) NSUInteger mTradingPort;

@property (readonly) BOOL connected;
@property (readonly) BOOL loggedIn;
@property (readonly) NSUInteger bytesReceived;
@property (readonly) NSUInteger bytesSent;
@property (nonatomic, retain) StatusController *statusController;

+ (Monitor *)sharedManager;
- (NetworkStatus)currentReachabilityStatus;
- (void)applicationDidFinishLaunching;
- (void)applicationWillTerminate;
- (void)applicationWillResignActive;
- (void)applicationDidBecomeActive;
- (void)usernameAndPasswordChanged;

- (NSArray *)interfaces;
- (NSArray *)serverAddresses;

@end
