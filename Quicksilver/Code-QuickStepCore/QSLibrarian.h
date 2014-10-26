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

- (void)removeIndexes;
- (void)loadDefaultCatalog;

- (void)assignCustomAbbreviationForItem:(QSObject *)item;
//- (void)saveCatalogArrays;
- (void)registerPresets:(NSArray *)newPresets inBundle:(NSBundle *)bundle scan:(BOOL)scan;
//- (void)loadCatalog;
- (void)dealloc;
- (void)writeCatalog:(id)sender;
//- (NSArray *)entriesForSource:(NSString *)source;
- (void)reloadSource:(NSNotification *)notif;
- (void)reloadEntrySources:(NSNotification *)notif;
- (void)reloadEntry:(NSNotification *)notif;
- (void)reloadIDDictionary:(NSNotification *)notif;
- (void)reloadSets:(NSNotification *)notif;
- (QSCatalogEntry *)firstEntryContainingObject:(QSObject *)object;
- (void)loadShelfArrays;
- (BOOL)loadCatalogArrays;
//- (BOOL)loadIndexesForEntries:(NSArray *)theEntries;
- (void)recalculateTypeArraysForItem:(QSCatalogEntry *)entry;
- (QSObject *)objectWithIdentifier:(NSString *)ident;
- (void)setIdentifier:(NSString *)ident forObject:(QSObject *)obj;
- (void)removeObjectWithIdentifier:(NSString *)ident;
- (NSArray *)arrayForType:(NSString *)string;
- (NSArray *)scoredArrayForType:(NSString *)string;
- (NSDictionary *)typeArraysFromArray:(NSArray *)array;
- (void)loadMissingIndexes;
- (void)savePasteboardHistory;
- (void)saveShelf:(NSString *)key;
- (void)scanCatalogIgnoringIndexes:(BOOL)force;
- (void)startThreadedScan;
- (void)startThreadedAndForcedScan;
- (IBAction)forceScanCatalog:(id)sender;
- (IBAction)scanCatalog:(id)sender;
- (void)scanCatalogWithDelay:(id)sender QS_DEPRECATED;
- (BOOL)itemIsOmitted:(QSBasicObject *)item;
- (void)setItem:(QSBasicObject *)item isOmitted:(BOOL)omit;
#ifdef DEBUG
- (CGFloat) estimatedTimeForSearchInSet:(id)set;
- (NSMutableArray *)scoreTest:(id)sender;
#endif
- (NSMutableArray *)scoredArrayForString:(NSString *)string;
- (NSMutableArray *)scoredArrayForString:(NSString *)string inNamedSet:(NSString *)setName QS_DEPRECATED;
- (NSMutableArray *)scoredArrayForString:(NSString *)searchString inSet:(id)set;
- (NSMutableArray *)scoredArrayForString:(NSString *)searchString inSet:(NSArray *)set mnemonicsOnly:(BOOL)mnemonicsOnly;
- (NSMutableArray *)shelfNamed:(NSString *)shelfName;
//- (void)registerActions:(id)actionObject;
//- (void)loadActionsForObject:(id)actionObject;
- (QSCatalogEntry *)catalogCustom;
- (void)enableEntries;
- (NSMutableSet *)defaultSearchSet ;
- (void)setDefaultSearchSet:(NSMutableSet *)newDefaultSearchSet ;
- (NSMutableDictionary *)appSearchArrays ;
- (void)setAppSearchArrays:(NSMutableDictionary *)newAppSearchArrays ;
- (NSMutableDictionary *)catalogArrays ;
- (void)setCatalogArrays:(NSMutableDictionary *)newCatalogArrays ;
- (NSMutableDictionary *)typeArrays ;
- (void)setTypeArrays:(NSMutableDictionary *)newTypeArrays ;
- (NSMutableDictionary *)shelfArrays ;
- (void)setShelfArrays:(NSMutableDictionary *)newShelfArrays ;
- (NSNumber *)presetIsEnabled:(QSCatalogEntry *)preset;
- (void)setPreset:(QSCatalogEntry *)preset isEnabled:(BOOL)flag;
- (QSCatalogEntry *)entryForID:(NSString *)theID;
- (void)pruneInvalidChildren:(id)sender;
- (void)loadCatalogInfo;
- (void)initCatalog;

@end
