//
//  LastChangeView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@class Symbol;
@class ChartController;

@interface LastChangeView : UIView {
@private	
	Symbol *_symbol;
	
	ChartController *_chartController;
	
	UILabel *_timeLabel;
	UILabel *_lastLabel;
	UILabel *_lastChangeLabel;
	UILabel *_lastPercentChangeLabel;
}

@property (nonatomic, retain) Symbol *symbol;

@end
