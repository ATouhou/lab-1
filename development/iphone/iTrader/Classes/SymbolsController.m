//
//  StocksController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "SymbolsController.h"
#import "iTraderCommunicator.h"
#import "Symbol.h"
#import "Feed.h"

@implementation SymbolsController
@synthesize symbols, orderedSymbols, feeds, orderedFeeds;
@synthesize updateDelegate;

static SymbolsController *sharedSymbolsController = nil;

+ (SymbolsController *)sharedManager {
	if (sharedSymbolsController == nil) {
		sharedSymbolsController = [[super allocWithZone:NULL] init];
	}
	return sharedSymbolsController;
}

+ (id)allocWithZone:(NSZone *)zone {
	return [[self sharedManager] retain];
}

- (id)copyWithZone:(NSZone *)zone {
	return self;
}

- (id)retain {
	return self;
}

- (NSUInteger)retainCount {
	return NSUIntegerMax;
}

- (void)release {
	// do nothing
}

- (id)autorelease {
	return self;
}

-(id)init {
	self = [super init];
	if (self != nil) {
		communicator = [iTraderCommunicator sharedManager];
		communicator.symbolsDelegate = self;
		symbols = [[NSMutableDictionary alloc] init];
		orderedSymbols = [[NSMutableArray alloc] init];
		feeds = [[NSMutableDictionary alloc] init];
		orderedFeeds = [[NSMutableArray alloc] init];
		// If I store the info on the phone I should load it up now.
	}
	
	return self;
}

-(void)dealloc {
	[symbols release];
	[feeds release];
	[orderedFeeds release];
	
	[super dealloc];
}

-(void)addSymbol:(Symbol *)symbol withFeed:(Feed *)aFeed {
	if (symbol != nil && aFeed != nil) {
		if ([self.feeds objectForKey:aFeed.number] == nil) { // if we haven't seen this feed before.
			NSUInteger index = [self.orderedFeeds count];
			[self.orderedFeeds addObject:aFeed];
			[self.feeds setObject:[NSNumber numberWithUnsignedInteger:index] forKey:aFeed.number];			
		}
		
		Feed *feed = [self.feeds objectForKey:aFeed.number]
		NSMutableArray *symbols = feed.symbols;
		if ([symbols objectForKey:symbol.feedTicker] == nil) {
			NSUInteger index = [orderedSymbols count];
			[orderedSymbols addObject:symbol];
			[symbols setObject:[NSNumber numberWithUnsignedInteger:index] forKey:symbol.feedTicker];
		}		
	}
	
	if (updateDelegate && [updateDelegate respondsToSelector:@selector(symbolAdded:)]) {
		[self.updateDelegate symbolAdded:symbol];
	}
}

-(void)addFeed:(Feed *)feed {
	// This needs to be fixed
	assert(feed != nil);
	if ([feeds objectForKey:feed.number] == nil) { // if we haven't seen this feed before.		
		NSUInteger index = [orderedFeeds count];
		[orderedFeeds addObject:feed];
		[feeds setObject:[NSNumber numberWithUnsignedInteger:index] forKey:feed.number];
		
	}
}

-(void)updateQuotes:(NSArray *)quotes {	
	NSMutableArray *updatedQuotes = [[NSMutableArray alloc] init];
	
	for (NSString *quote in quotes) {
		NSArray *values = [self cleanQuote:quote];
		
		NSString *feedTicker = [values objectAtIndex:0];
		NSUInteger index = [(NSNumber *)[self.symbols objectForKey:feedTicker] unsignedIntegerValue];
		
		//18177/OSEBX;380.983;0.22;;;;;0.827
		// feed/ticker

		Symbol *symbol = [self.orderedSymbols objectAtIndex:index];
		// last trade
		NSString *lastTrade = [values objectAtIndex:1];

		if ([lastTrade isEqualToString:@""] == NO && [lastTrade isEqualToString:@"-"] == NO) {
			symbol.lastTrade = [NSNumber numberWithFloat:[lastTrade floatValue]];
		}
		
		// percent change
		symbol.percentChange = [NSNumber numberWithInteger:[[values objectAtIndex:2] integerValue]];
		// bid price
		symbol.bidPrice = [NSNumber numberWithInteger:[[values objectAtIndex:3] integerValue]];
		// ask price
		symbol.askPrice = [NSNumber numberWithInteger:[[values objectAtIndex:4] integerValue]];
		// ask volume
		symbol.askVolume = [NSNumber numberWithInteger:[[values objectAtIndex:5] integerValue]];
		// bid volume
		symbol.bidVolume = [NSNumber numberWithInteger:[[values objectAtIndex:6] integerValue]];
		// change
		symbol.change = [NSNumber numberWithFloat:[[values objectAtIndex:7] floatValue]];
		// high
		symbol.high = [NSNumber numberWithInteger:[[values objectAtIndex:8] integerValue]];
		// low
		symbol.low = [NSNumber numberWithInteger:[[values objectAtIndex:9] integerValue]];
		// open
		symbol.open = [NSNumber numberWithInteger:[[values objectAtIndex:10] integerValue]];
		// volume
		symbol.volume = [NSNumber numberWithInteger:[[values objectAtIndex:11] integerValue]];
		
		[updatedQuotes addObject:feedTicker];
	}
	
	if (updateDelegate && [updateDelegate respondsToSelector:@selector(symbolsUpdated:)]) {
		[self.updateDelegate symbolsUpdated:updatedQuotes];
	}
	
	[updatedQuotes release];
}

-(NSArray *)cleanQuote:(NSString *)quote {
	NSCharacterSet *whitespaceAndNewline = [NSCharacterSet whitespaceAndNewlineCharacterSet];
	NSArray *providedValues = [quote componentsSeparatedByString:@";"];
	
	NSMutableArray *paddedArray = [[NSMutableArray alloc] init];
	for (NSString *value in providedValues) {
		[paddedArray addObject:[value stringByTrimmingCharactersInSet:whitespaceAndNewline]];
	}
		
	for (int i = [paddedArray count] - 1; i < 12; i++) {
		[paddedArray addObject:@""];
	}
	
	NSArray *finalProduct = [NSArray arrayWithArray:paddedArray];
	[paddedArray release];
	
	return finalProduct;
}

	
	-(NSArray *)cleanStrings:(NSArray *)strings {
		NSCharacterSet *whitespaceAndNewline = [NSCharacterSet whitespaceAndNewlineCharacterSet];
		NSMutableArray *mutableProduct = [[NSMutableArray alloc] init];
		
		for (NSString *string in strings) {
			[mutableProduct addObject:[string stringByTrimmingCharactersInSet:whitespaceAndNewline]];
		}
		
		NSArray *finalProduct = [NSArray arrayWithArray:mutableProduct];
		[mutableProduct release];
		
		return finalProduct;
	}

@end
