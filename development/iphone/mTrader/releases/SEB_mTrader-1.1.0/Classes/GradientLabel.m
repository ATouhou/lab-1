//
//  GradientLabel.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 19.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "GradientLabel.h"

#import <QuartzCore/QuartzCore.h>

@implementation GradientLabel

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
	}
    return self;
}

//- (void)drawRect:(CGRect)rect {
//	UIColor *sectionTextColor = [UIColor colorWithWhite:1.0 alpha:1.0];
//	UIColor *sectionTextShadowColor = [UIColor colorWithWhite:0.0 alpha:0.44];
//	CGSize shadowOffset = CGSizeMake(0.0, 1.0);
//	
//	[self setTextColor:sectionTextColor];
//	[self setShadowColor:sectionTextShadowColor];
//	[self setShadowOffset:shadowOffset];
//	[self setBackgroundColor:[UIColor clearColor]];
//	
//	[super drawRect:rect];
//	
//	// Render the dynamic gradient
//	CAGradientLayer *headerGradient = [CAGradientLayer layer];
//	UIColor *topLine = [UIColor colorWithRed:111.0/255.0 green:118.0/255.0 blue:123.0/255.0 alpha:1.0];
//	UIColor *shine = [UIColor colorWithRed:165.0/255.0 green:177/255.0 blue:186.0/255.0 alpha:1.0];
//	UIColor *topOfFade = [UIColor colorWithRed:184.0/255.0 green:193.0/255.0 blue:200.0/255.0 alpha:1.0];
//	UIColor *bottomOfFade = [UIColor colorWithRed:144.0/255.0 green:159.0/255.0 blue:170.0/255.0 alpha:1.0];
//	UIColor *bottomLine = [UIColor colorWithRed:152.0/255.0 green:158.0/255.0 blue:164.0/255.0 alpha:1.0];
//	NSArray *colors = [NSArray arrayWithObjects:(id)topLine.CGColor, 
//					   (id)shine.CGColor, 
//					   (id)topOfFade.CGColor, 
//					   (id)bottomOfFade.CGColor, 
//					   (id)bottomLine.CGColor, 
//					   nil];
//	NSArray *locations = [NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0], 
//						  [NSNumber numberWithFloat:0.05],
//						  [NSNumber numberWithFloat:0.10],
//						  [NSNumber numberWithFloat:0.95],
//						  [NSNumber numberWithFloat:1.0],
//						  nil];
//	headerGradient.colors = colors;
//	headerGradient.locations = locations;
//	headerGradient.frame = self.bounds;
//	
//	[self.layer insertSublayer:headerGradient atIndex:1];
//	
//}


- (void)dealloc {
    [super dealloc];
}


@end
