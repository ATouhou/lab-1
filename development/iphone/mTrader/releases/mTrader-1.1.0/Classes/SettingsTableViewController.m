//
//  SettingsTableViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "SettingsTableViewController.h"

#import "mTraderAppDelegate.h"

#import "mTraderCommunicator.h"
#import "mTraderServerMonitor.h"
#import "UserDefaults.h"

#import "QFields.h"
#import "AboutViewController.h"

@implementation SettingsTableViewController
@synthesize tableView;
@synthesize aboutView;
@synthesize userTextField;
@synthesize passwordTextField;

- (id)init {
	self = [super init];
	if (self != nil) {
		defaults = [UserDefaults sharedManager];
		communicator = [mTraderCommunicator sharedManager];
		
		UIImage* anImage = [UIImage imageNamed:@"SettingsTab.png"];
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"SettingsTab", @"The settings tab label") image:anImage tag:SETTINGS];
		self.tabBarItem = theItem;
		[theItem release];
		
		sectionsArray = [[NSArray alloc] initWithObjects:NSLocalizedString(@"LoginSettings", @"Login settings for infront account"), NSLocalizedString(@"Company", @"Company name"), nil];
		loginSectionArray = [[NSArray alloc] initWithObjects:NSLocalizedString(@"Username", @"Username string"), NSLocalizedString(@"Password", @"Password string"), nil];
		infrontSectionArray = [[NSArray alloc] initWithObjects:NSLocalizedString(@"About", @"About string"), nil];
		self.title = NSLocalizedString(@"SettingsTab", @"The settings tab label");
		
		tableView = [[UITableView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame] style:UITableViewStyleGrouped];
		tableView.delegate = self;
		tableView.dataSource = self;
		self.view = tableView;
	}
	return self;
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	QFields *qFields = [[QFields alloc] init];
	communicator.qFields = qFields;
	[qFields release];
	
	[communicator setStreamingForFeedTicker:nil];
}


// Table View Data Source Methods
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [sectionsArray count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	if (section == LOGINDETAILS) {
		return [loginSectionArray count];
	} else if (section == INFRONT) {
		return [infrontSectionArray count];
	} else {
		return 0;
	}
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	return [sectionsArray objectAtIndex:section];
}

- (UITableViewCell *)tableView:(UITableView *)_tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *CellIdentifier = @"SettingsCell";
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
	}
	if (indexPath.section == LOGINDETAILS) {
		[cell.textLabel setText:[loginSectionArray objectAtIndex:indexPath.row]];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		
		UITextField *textField = [[UITextField alloc] initWithFrame:CGRectMake(120, 5, 175, 35)];
		textField.delegate = self;
		textField.autocorrectionType = UITextAutocorrectionTypeNo;
		textField.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
		textField.textColor = [[UIColor alloc] initWithRed:.25 green:.35 blue:.55 alpha:1];
		textField.autocapitalizationType = UITextAutocapitalizationTypeNone;
		textField.borderStyle = UITextBorderStyleNone;
		textField.returnKeyType = UIReturnKeyDone;
		if (indexPath.row == USERNAME_FIELD) {
			textField.tag = USERNAME_FIELD;
			textField.text = defaults.username;
			self.userTextField = textField;
		} else if (indexPath.row == PASSWORD_FIELD) {
			textField.secureTextEntry = YES;
			textField.clearButtonMode = UITextFieldViewModeWhileEditing;
			textField.tag = PASSWORD_FIELD;
			textField.text = defaults.password;
			self.passwordTextField = textField;
		}
		
		[cell addSubview:textField];
		[textField release];
	} else if (indexPath.section == INFRONT) {
		[cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
		[cell.textLabel setText:[infrontSectionArray objectAtIndex:indexPath.row]];
	} else {
		[cell.textLabel setText:@"Hello, Dolly!"];
	}
	return cell;
}

// Table View Delegate Method
- (void)tableView:(UITableView *)_tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	if (indexPath.section == LOGINDETAILS) {
		if (indexPath.row == 0) {
		} else if (indexPath.row == 1) {
		}
	} else if (indexPath.section == INFRONT) {
		[_tableView deselectRowAtIndexPath:indexPath animated:NO];
		AboutViewController *aboutViewController = [[AboutViewController alloc] initWithNibName:@"AboutView" bundle:nil];
		[self.navigationController pushViewController:aboutViewController animated:YES];
		
		[aboutViewController release];
	}	
}

// Text Field Delegate Methods
- (void)textFieldDidEndEditing:(UITextField *)textField {
	NSString *username = defaults.username;
	NSString *password = defaults.password;
	
	BOOL update = NO;
	if (textField.tag == USERNAME_FIELD) {
		if (![username isEqualToString:textField.text]) {
			username = textField.text;
			update = YES;
		}
	} else if (textField.tag == PASSWORD_FIELD) {
		if (![password isEqualToString:textField.text]) {
			password = textField.text;
			update = YES;
		}
	}
	
	if (update == YES) {
		defaults.username = username;
		defaults.password = password;
		[defaults saveSettings];
	}
	
	// As long as username and password are not empty or nil attempt to connect
	if (username != nil && password != nil && ![username isEqualToString:@""] && ![password isEqualToString:@""]) {
		[[mTraderServerMonitor sharedManager] attemptConnection];
	}
}

-(BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
	return YES;
}

- (void)dealloc {
	[sectionsArray release];
	[loginSectionArray release];
	[infrontSectionArray release];
	
	[tableView release];
	
    [super dealloc];
}

@end
