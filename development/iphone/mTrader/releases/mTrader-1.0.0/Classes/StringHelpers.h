//
//  StringHelpers.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 29.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@interface StringHelpers : NSObject {

}

+ (NSString *)cleanString:(NSString *)aString;
+ (NSArray *)cleanComponents:(NSArray *)arrayOfStrings;

@end