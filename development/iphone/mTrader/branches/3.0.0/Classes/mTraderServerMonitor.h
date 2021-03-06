//
//  mTraderServerMonitor.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "mTraderCommunicator.h"

@class Reachability;

@interface mTraderServerMonitor : NSObject <mTraderServerMonitorDelegate> {
	Reachability *_reachability;
}
@property (nonatomic,retain) Reachability *reachability;

+ (mTraderServerMonitor *)sharedManager;

@end
