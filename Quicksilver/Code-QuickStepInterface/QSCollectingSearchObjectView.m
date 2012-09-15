//
// QSCollectingSearchObjectView.m
// Quicksilver
//
// Created by Alcor on 3/22/05.
// Copyright 2005 Blacktree. All rights reserved.
//

#import "QSCollection.h"
#import "QSCollectingSearchObjectView.h"

@interface QSCollectingSearchObjectView ()
@property (assign) BOOL collecting;
@property (retain) QSCollection *collection;
@end

@implementation QSCollectingSearchObjectView
- (void)awakeFromNib {
	[super awakeFromNib];
	self.collection = [[QSCollection alloc] init];
	self.collecting = NO;
	self.collectionEdge = NSMinYEdge;
	self.collectionSpace = 16.0;
}

- (NSSize) cellSize {
	NSSize size = [super cellSize];
	if (self.collectionSpace < 0.0001)
		size.width += self.collection.count * 16;
	return size;
}

- (void)drawRect:(NSRect)rect {
	NSRect frame = [self frame];
    NSUInteger count = self.collection.count;
	if (![self currentEditor] && count) {
		CGFloat totalSpace = self.collectionSpace + 4;
		if (self.collectionSpace < 0.0001) {
			totalSpace = count * 16 + 8;
		}
		frame.origin = NSZeroPoint;
		NSRect mainRect, collectRect;
		NSDivideRect(frame, &collectRect, &mainRect, totalSpace, self.collectionEdge);
		[[self cell] drawWithFrame:mainRect inView:self];
		[[NSColor colorWithDeviceWhite:1.0 alpha:0.92] set];
		if (self.collectionSpace < 0.0001)
			collectRect.origin.x += 8;
		CGFloat iconSize = self.collectionSpace ?: 16;
		CGFloat opacity = self.collecting ? 1.0 : 0.5;
		QSObject *object;
		for (NSUInteger i = 0; i < count; i++) {
			object = [self.collection objectAtIndex:i];
			NSImage *icon = [object icon];
			[icon setSize:QSSize16];
			[icon drawInRect:NSMakeRect(collectRect.origin.x+iconSize*i, collectRect.origin.y+2, iconSize, iconSize) fromRect:rectFromSize([icon size]) operation:NSCompositeSourceOver fraction:opacity];
		}
	} else {
		[super drawRect:rect];
	}
}

- (void)insertText:(id)aString replacementRange:(NSRange)replacementRange
{
	if (!self.collecting && ![partialString length]) {
		[self emptyCollection:nil];
	}
	[super insertText:aString replacementRange:replacementRange];
}

- (IBAction)collect:(id)sender { //Adds additional objects to a collection
	if (!self.collecting) self.collecting = YES;
	if ([super objectValue] && ![self.collection containsObject:[super objectValue]]) {
		[self.collection addObject:[super objectValue]];
		[self updateHistory];
		[self saveMnemonic];
		[self setNeedsDisplay:YES];
	}
	[self setShouldResetSearchString:YES];
}

- (IBAction)uncollect:(id)sender { //Removes an object to a collection
	NSInteger position = -1;
	if (self.collection.count) {
		position = [self.collection indexOfObject:[super objectValue]] - 1;
		[self.collection removeObject:[super objectValue]];
	}
	if (position >= 0) {
		[self selectObjectValue:[self.collection objectAtIndex:position]];
	} else {
		[self selectObjectValue:[self.collection lastObject]];
	}
	if (self.collection.count <= 1) {
		// stop collecting if there's only one object
		[self emptyCollection:nil];
	}
	[self clearSearch];
	[self setNeedsDisplay:YES];
}

- (IBAction)uncollectLast:(id)sender { //Removes an object to a collection
	if (self.collection.count)
		[self.collection removeLastObject];
	if (self.collection.count <= 1) {
		// stop collecting if there's only one object
		[self emptyCollection:nil];
	}
	[self setNeedsDisplay:YES];
	//if ([[resultController window] isVisible])
	//	[resultController->resultTable setNeedsDisplay:YES];}
}

- (IBAction)goForwardInCollection:(id)sender
{
	if (self.collection.count <= 1) {
		return;
	}
	QSObject *selected = [super objectValue];
	NSUInteger position = [self.collection indexOfObject:selected];
	if (position == self.collection.count - 1 || position == NSNotFound) {
		// end of the list or not in list at all, wrap to beginning
		position = 0;
	} else {
		// go forward one
		position++;
	}
	// prepare the state of the view
	[self clearSearch];
	// change the selection
	QSObject *newSelected = [self.collection objectAtIndex:position];
	[self selectObjectValue:newSelected];
}

- (IBAction)goBackwardInCollection:(id)sender
{
	if (self.collection.count <= 1) {
		return;
	}
	QSObject *selected = [super objectValue];
	NSUInteger position = [self.collection indexOfObject:selected];
	if (position == 0 || position == NSNotFound) {
		// beginning of the list or not in list at all, wrap to end
		position = self.collection.count - 1;
	} else {
		// go back one
		position--;
	}
	// prepare the state of the view
	[self clearSearch];
	// change the selection
	QSObject *newSelected = [self.collection objectAtIndex:position];
	[self selectObjectValue:newSelected];
}

- (void)clearObjectValue {
	[self emptyCollection:nil];
	[super clearObjectValue];
}

- (IBAction)emptyCollection:(id)sender {
	self.collecting = NO;
	[self.collection removeAllObjects];

}
- (IBAction)combine:(id)sender { //Resolve a collection as a single object
	[self setObjectValue:[self objectValue]];
	[self emptyCollection:sender];
}

- (id)objectValue {
	if (self.collection.count)
		return self.collection;
	else
		return [super objectValue];
}

- (BOOL)objectIsInCollection:(QSObject *)thisObject {
	return [self.collection containsObject:thisObject];
}

- (void)explodeCombinedObject
{
	QSObject *selected = [super objectValue];
	NSMutableArray *components;
	if (self.collection.count) {
		components = [self.collection mutableCopy];
	} else {
		components = [[selected splitObjects] mutableCopy];
		selected = nil;
	}
	if (selected && ![components containsObject:selected]) {
		[components addObject:selected];
	}
	if ([components count] <= 1) {
		NSBeep();
		return;
	}
	[[self controller] showArray:components];
}

- (void)deleteBackward:(id)sender {
	if (self.collection.count && ![partialString length]) {
		if (![self.collection containsObject:[super objectValue]]) {
			// search string cleared, but main object was never added to the collection
			[self.collection addObject:[super objectValue]];
		}
		[self uncollect:sender];
	} else {
		[super deleteBackward:sender];
    }
}

- (void)reset:(id)sender {
	self.collecting = NO;
	[super reset:sender];
}

- (void)redisplayObjectValue:(QSObject *)newObject
{
	if ([newObject count] > 1) {
		self.collection = [[newObject splitObjects] mutableCopy];
		[self.collection makeObjectsPerformSelector:@selector(loadIcon)];
		newObject = [self.collection lastObject];
	} else {
		[self emptyCollection:nil];
	}
	self.collecting = NO;
	[self selectObjectValue:newObject];
}

- (void)selectObjectValue:(QSObject *)newObject {
	if (!self.collecting)
		[self emptyCollection:nil];
	[super selectObjectValue:newObject];
}

- (void)setObjectValue:(QSBasicObject *)newObject {
    if (newObject == [self objectValue]) {
        return;
    }
	if (!self.collecting) {
        [self emptyCollection:self];
    }
    // If the new object is 'nil' (i.e. the pane has been cleared) then also clear the underlying text editor (for the 1st pane only)
    if (!newObject && self == [[super controller] dSelector]) {
        NSTextView *editor = (NSTextView *)[[self window] fieldEditor:NO forObject: self];
        if (editor) {
            [editor setString:@""];
        }
    }
    
	[super setObjectValue:newObject];
}

@end
