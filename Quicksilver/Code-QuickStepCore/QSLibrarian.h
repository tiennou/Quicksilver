#import <Foundation/Foundation.h>
#import "QSCatalogEntry.h"

#define kCustomCatalogID @"QSCatalogCustom"

extern CGFloat QSMinScore;

@class QSBasicObject;
@class QSObject;
@class QSActions;
@class QSAction;
@class QSTask;

@class QSLibrarian;
extern QSLibrarian *QSLib; // Shared Instance

extern NSString *const QSCatalogStructureChangedNotification;
extern NSString *const QSCatalogIndexingCompletedNotification;
extern NSString *const QSCatalogSourceInvalidatedNotification;

@interface QSLibrarian : NSObject

@property (retain) QSThreadSafeMutableDictionary *objectDictionary;
@property (retain) QSObject *pasteboardObject;
@property (retain, readonly) QSTask *scanTask;
@property (readonly, retain) QSCatalogEntry *catalog;

+ (instancetype)sharedInstance;

- (QSCatalogEntry *)catalogCustom; // Rename to -customCatalogEntry

- (void)removeIndexes;
- (void)loadDefaultCatalog;

- (void)writeCatalog:(id)sender;

#pragma mark -
#pragma mark Objects

- (QSObject *)objectWithIdentifier:(NSString *)ident;
- (void)setIdentifier:(NSString *)ident forObject:(QSObject *)obj;
- (void)removeObjectWithIdentifier:(NSString *)ident;
- (void)assignCustomAbbreviationForItem:(QSObject *)item;

#pragma mark -
#pragma mark Object types

- (NSArray *)arrayForType:(NSString *)string;
- (NSArray *)scoredArrayForType:(NSString *)string;
- (NSDictionary *)typeArraysFromArray:(NSArray *)array;

- (void)recalculateTypeArraysForItem:(QSCatalogEntry *)entry;

#pragma mark -
#pragma mark Catalog scanning

- (void)scanCatalogIgnoringIndexes:(BOOL)force;
- (void)scanCatalogWithDelay:(id)sender QS_DEPRECATED;

- (BOOL)itemIsOmitted:(QSBasicObject *)item;
- (void)setItem:(QSBasicObject *)item isOmitted:(BOOL)omit;

#pragma mark -
#pragma mark Ranking objects

- (NSMutableArray *)scoredArrayForString:(NSString *)string;
- (NSMutableArray *)scoredArrayForString:(NSString *)string inNamedSet:(NSString *)setName QS_DEPRECATED;
- (NSMutableArray *)scoredArrayForString:(NSString *)searchString inSet:(id)set;
- (NSMutableArray *)scoredArrayForString:(NSString *)searchString inSet:(NSArray *)set mnemonicsOnly:(BOOL)mnemonicsOnly;

#ifdef DEBUG
- (CGFloat)estimatedTimeForSearchInSet:(id)set;
- (NSMutableArray *)scoreTest:(id)sender;
#endif

#pragma mark -
#pragma mark Shelves

- (NSMutableArray *)shelfNamed:(NSString *)shelfName;
- (void)saveShelf:(NSString *)key;

#pragma mark -
#pragma mark Entries

- (QSCatalogEntry *)entryForID:(NSString *)theID;
- (QSCatalogEntry *)firstEntryContainingObject:(QSObject *)object;

- (NSNumber *)presetIsEnabled:(QSCatalogEntry *)preset;
- (void)setPreset:(QSCatalogEntry *)preset isEnabled:(BOOL)flag;

- (void)savePasteboardHistory QS_DEPRECATED; // This must go, use -saveShelf:

@end
