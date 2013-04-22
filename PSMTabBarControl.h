//
//  PSMTabBarControl.h
//  PSMTabBarControl
//
//  Created by John Pannell on 10/13/05.
//  Copyright 2005 Positive Spin Media. All rights reserved.
//

/*
 This view provides a control interface to manage a regular NSTabView.
 It looks and works like the tabbed browsing interface of many popular browsers.
 */

#import <Cocoa/Cocoa.h>

static NSString *PSMTabDragDidEndNotification       = @"PSMTabDragDidEndNotification";
static NSString *PSMTabDragDidBeginNotification     = @"PSMTabDragDidBeginNotification";

#define kPSMTabBarControlHeight                     22

// internal cell border
#define MARGIN_X                                    6
#define MARGIN_Y                                    3

// padding between objects
#define kPSMTabBarCellPadding                       4

// fixed size objects
#define kPSMMinimumTitleWidth                       30
#define kPSMTabBarIndicatorWidth                    16.0
#define kPSMTabBarIconWidth                         16.0
#define kPSMHideAnimationSteps                      3.0

// Value used in _currentStep to indicate that resizing operation is not in progress
#define kPSMIsNotBeingResized                       -1

// Value used in _currentStep when a resizing operation has just been started
#define kPSMStartResizeAnimation                    0

typedef enum {
	PSMTabBarHorizontalOrientation,
	PSMTabBarVerticalOrientation
} PSMTabBarOrientation;

typedef enum {
	PSMTabBarTearOffAlphaWindow,
	PSMTabBarTearOffMiniwindow
} PSMTabBarTearOffStyle;

enum {
	PSMTab_SelectedMask				= 1 << 1,
	PSMTab_LeftIsSelectedMask		= 1 << 2,
	PSMTab_RightIsSelectedMask		= 1 << 3,
	PSMTab_PositionLeftMask			= 1 << 4,
	PSMTab_PositionMiddleMask		= 1 << 5,
	PSMTab_PositionRightMask		= 1 << 6,
	PSMTab_PositionSingleMask		= 1 << 7,
};

@class PSMOverflowPopUpButton, PSMRolloverButton, PSMTabBarCell, PSMTabBarController;
@protocol PSMTabBarControlDelegate, PSMTabStyle;

@interface PSMTabBarControl : NSControl <NSTabViewDelegate> {
    IBOutlet id<PSMTabBarControlDelegate> _delegate;
    IBOutlet NSTabView *_tabView;
    IBOutlet id _partnerView;
												
	// control basics
	NSMutableArray							*_cells;								// the cells that draw the tabs
	PSMOverflowPopUpButton					*_overflowPopUpButton;				// for too many tabs
	PSMRolloverButton						*_addTabButton;
	PSMTabBarController						*_controller;
    
    // control configuration
    id <PSMTabStyle> _style;
    PSMTabBarOrientation _orientation;
    BOOL _canCloseOnlyTab;
    BOOL _disableTabClose;
    BOOL _hideForSingleTab;
    BOOL _showAddTabButton;
    NSInteger _cellMinWidth;
    NSInteger _cellMaxWidth;
    NSInteger _cellOptimumWidth;
    BOOL _sizeCellsToFit;
    BOOL _useOverflowMenu;
    BOOL _allowsBackgroundTabClosing;
    BOOL _allowsResizing;
    BOOL _selectsTabsOnMouseDown;
    BOOL _automaticallyAnimates;
    BOOL _alwaysShowActiveTab;
    BOOL _allowsScrubbing;
    
    // control state
    BOOL _isHidden;

	// Spring-loading.
	NSTimer									*_springTimer;
	NSTabViewItem								*_tabViewItemWithSpring;

	// drawing style
	NSInteger									_resizeAreaCompensation;
	NSTimer									*_animationTimer;
	PSMTabBarTearOffStyle					_tearOffStyle;

	// vertical tab resizing
	BOOL										_resizing;

	// animation for hide/show
	NSInteger									_currentStep;
	BOOL										_awakenedFromNib;
	NSInteger									_tabBarWidth;
	NSTimer									*_showHideAnimationTimer;

	// drag and drop
	NSEvent									*_lastMouseDownEvent;				// keep this for dragging reference
	BOOL										_didDrag;
	BOOL										_closeClicked;
}

@property (nonatomic, assign) IBOutlet id<PSMTabBarControlDelegate> delegate;
@property (nonatomic, retain) IBOutlet NSTabView *tabView;
@property (nonatomic, retain) IBOutlet id partnerView;

// control configuration
@property (nonatomic, retain) id <PSMTabStyle> style;
@property (nonatomic) PSMTabBarOrientation orientation;
@property (nonatomic) BOOL canCloseOnlyTab;
@property (nonatomic) BOOL disableTabClose;
@property (nonatomic) BOOL hideForSingleTab;
@property (nonatomic) BOOL showAddTabButton;
@property (nonatomic) NSInteger cellMinWidth;
@property (nonatomic) NSInteger cellMaxWidth;
@property (nonatomic) NSInteger cellOptimumWidth;
@property (nonatomic) BOOL sizeCellsToFit;
@property (nonatomic) BOOL useOverflowMenu;
@property (nonatomic) BOOL allowsBackgroundTabClosing;
@property (nonatomic) BOOL allowsResizing;
@property (nonatomic) BOOL selectsTabsOnMouseDown;
@property (nonatomic) BOOL automaticallyAnimates;
@property (nonatomic) BOOL alwaysShowActiveTab;
@property (nonatomic) BOOL allowsScrubbing;
@property (nonatomic) PSMTabBarTearOffStyle tearOffStyle;

// control state
@property (nonatomic, readonly) BOOL isHidden;

// control characteristics
+ (NSBundle *)bundle;
- (CGFloat)availableCellWidth;
- (NSRect)genericCellRect;

// control configuration
- (NSString *)styleName;
- (void)setStyleNamed:(NSString *)name;

// the buttons
- (PSMRolloverButton *)addTabButton;
- (PSMOverflowPopUpButton *)overflowPopUpButton;

// tab information
- (NSMutableArray *)representedTabViewItems;
- (NSInteger)numberOfVisibleTabs;
- (PSMTabBarCell *)lastVisibleTab;

// special effects
- (void)hideTabBar:(BOOL) hide animate:(BOOL)animate;
- (BOOL)isAnimating;

// internal bindings methods also used by the tab drag assistant
- (void)bindPropertiesForCell:(PSMTabBarCell *)cell andTabViewItem:(NSTabViewItem *)item;
- (void)removeTabForCell:(PSMTabBarCell *)cell;

@end


@protocol PSMTabBarControlDelegate <NSTabViewDelegate>
@optional

//Standard NSTabView methods
- (BOOL)tabView:(NSTabView *)aTabView shouldCloseTabViewItem:(NSTabViewItem *)tabViewItem;
- (void)tabView:(NSTabView *)aTabView didCloseTabViewItem:(NSTabViewItem *)tabViewItem;

//"Spring-loaded" tabs methods
- (NSArray *)allowedDraggedTypesForTabView:(NSTabView *)aTabView;
- (void)tabView:(NSTabView *)aTabView acceptedDraggingInfo:(id <NSDraggingInfo>) draggingInfo onTabViewItem:(NSTabViewItem *)tabViewItem;

//Contextual menu method
- (NSMenu *)tabView:(NSTabView *)aTabView menuForTabViewItem:(NSTabViewItem *)tabViewItem;

//Drag and drop methods
- (BOOL)tabView:(NSTabView *)aTabView shouldDragTabViewItem:(NSTabViewItem *)tabViewItem fromTabBar:(PSMTabBarControl *)tabBarControl;
- (BOOL)tabView:(NSTabView *)aTabView shouldDropTabViewItem:(NSTabViewItem *)tabViewItem inTabBar:(PSMTabBarControl *)tabBarControl;
- (BOOL)tabView:(NSTabView *)aTabView shouldAllowTabViewItem:(NSTabViewItem *)tabViewItem toLeaveTabBar:(PSMTabBarControl *)tabBarControl;
- (void)tabView:(NSTabView*)aTabView didDropTabViewItem:(NSTabViewItem *)tabViewItem inTabBar:(PSMTabBarControl *)tabBarControl;


//Tear-off tabs methods
- (NSImage *)tabView:(NSTabView *)aTabView imageForTabViewItem:(NSTabViewItem *)tabViewItem offset:(NSSize *)offset styleMask:(NSUInteger *)styleMask;
- (PSMTabBarControl *)tabView:(NSTabView *)aTabView newTabBarForDraggedTabViewItem:(NSTabViewItem *)tabViewItem atPoint:(NSPoint)point;
- (void)tabView:(NSTabView *)aTabView closeWindowForLastTabViewItem:(NSTabViewItem *)tabViewItem;

//Overflow menu validation
- (BOOL)tabView:(NSTabView *)aTabView validateOverflowMenuItem:(NSMenuItem *)menuItem forTabViewItem:(NSTabViewItem *)tabViewItem;
- (void)tabView:(NSTabView *)aTabView tabViewItem:(NSTabViewItem *)tabViewItem isInOverflowMenu:(BOOL)inOverflowMenu;

//tab bar hiding methods
- (void)tabView:(NSTabView *)aTabView tabBarDidHide:(PSMTabBarControl *)tabBarControl;
- (void)tabView:(NSTabView *)aTabView tabBarDidUnhide:(PSMTabBarControl *)tabBarControl;
- (CGFloat)desiredWidthForVerticalTabBar:(PSMTabBarControl *)tabBarControl;

//closing
- (BOOL)tabView:(NSTabView *)aTabView disableTabCloseForTabViewItem:(NSTabViewItem *)tabViewItem;

//tooltips
- (NSString *)tabView:(NSTabView *)aTabView toolTipForTabViewItem:(NSTabViewItem *)tabViewItem;

//accessibility
- (NSString *)accessibilityStringForTabView:(NSTabView *)aTabView objectCount:(NSInteger)objectCount;

@end
