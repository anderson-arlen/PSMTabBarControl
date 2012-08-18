//
//  PSMOverflowPopUpButton.h
//  NetScrape
//
//  Created by John Pannell on 8/4/04.
//  Copyright 2004 Positive Spin Media. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface PSMRolloverButton : NSButton {
	NSImage* rolloverImage;
	NSImage* usualImage;
	NSTrackingRectTag myTrackingRectTag;
}

@property (nonatomic, retain) NSImage* rolloverImage;
@property (nonatomic, retain) NSImage* usualImage;
@property (nonatomic) NSTrackingRectTag myTrackingRectTag;

// tracking rect for mouse events
- (void)addTrackingRect;
- (void)removeTrackingRect;
@end