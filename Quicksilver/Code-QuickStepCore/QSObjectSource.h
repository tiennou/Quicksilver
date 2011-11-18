

#import <Foundation/Foundation.h>
@protocol QSObjectSource
- (NSImage *)iconForEntry:(NSDictionary *)theEntry;
@optional;
- (NSString *)nameForEntry:(NSDictionary *)theEntry;
- (NSArray *)objectsForEntry:(NSDictionary *)theEntry;
- (BOOL)indexIsValidFromDate:(NSDate *)indexDate forEntry:(NSDictionary *)theEntry;
- (BOOL)entryCanBeIndexed:(NSDictionary *)theEntry;
- (void)invalidateSelf;
- (BOOL)isVisibleSource;
- (BOOL)usesGlobalSettings;
@end

@protocol QSObjectSourceSettings
- (void)populateFields;
- (NSMutableDictionary *)currentEntry;
- (NSView *)settingsView;
- (void)setSettingsView:(NSView *)newSettingsView;
@optional;
- (void)setCurrentEntry:(NSMutableDictionary *)newCurrentEntry;
@end

@class QSCatalogEntry;
@interface QSObjectSource : NSObject <QSObjectSource, QSObjectSourceSettings> {
	IBOutlet NSView *settingsView;
	QSCatalogEntry *selection;
	NSMutableDictionary *currentEntry;
}
- (NSImage *)iconForEntry:(NSDictionary *)theEntry;
- (NSString *)nameForEntry:(NSDictionary *)theEntry;
- (NSArray *)objectsForEntry:(NSDictionary *)theEntry;
- (BOOL)indexIsValidFromDate:(NSDate *)indexDate forEntry:(NSDictionary *)theEntry;
- (void)populateFields;

- (void)updateCurrentEntryModificationDate;
- (NSMutableDictionary *)currentEntry;
//- (void)setCurrentEntry:(NSMutableDictionary *)newCurrentEntry;
- (NSView *)settingsView;
- (void)setSettingsView:(NSView *)newSettingsView;

- (QSCatalogEntry *)selection;
- (void)setSelection:(QSCatalogEntry *)newSelection;

@end




