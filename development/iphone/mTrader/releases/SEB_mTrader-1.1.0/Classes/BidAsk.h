//
//  BidAsk.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 16.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Symbol;

@interface BidAsk :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * index;
@property (nonatomic, retain) NSNumber * askSize;
@property (nonatomic, retain) NSNumber * bidPercent;
@property (nonatomic, retain) NSNumber * askPrice;
@property (nonatomic, retain) NSNumber * bidPrice;
@property (nonatomic, retain) NSNumber * askPercent;
@property (nonatomic, retain) NSNumber * bidSize;
@property (nonatomic, retain) Symbol * symbol;

@end



