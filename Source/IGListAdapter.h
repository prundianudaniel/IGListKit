/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <UIKit/UIKit.h>

#import <IGListKit/IGListAdapterDataSource.h>
#import <IGListKit/IGListAdapterDelegate.h>
#import <IGListKit/IGListCollectionContext.h>
#import <IGListKit/IGListCollectionView.h>
#import <IGListKit/IGListExperiments.h>
#import <IGListKit/IGListSectionType.h>
#import <IGListKit/IGListMacros.h>

@protocol IGListUpdatingDelegate;

@class IGListSectionController;

NS_ASSUME_NONNULL_BEGIN

/**
 A block to execute when list updates completes.

 @param finished Specifies whether or not the updates finished.
 */
typedef void (^IGListUpdaterCompletion)(BOOL finished);

/**
 `IGListAdapter` objects provide an abstraction for feeds of objects in a `UICollectionView` by breaking each object into
 individual sections, called "section controllers". These controllers (objects conforming to `IGListSectionType`) act as a
 data source and delegate for each section.

 Feed implementations must act as the data source for an `IGListAdapter` in order to drive the objects and section
 controllers in a collection view.
 */
IGLK_SUBCLASSING_RESTRICTED
@interface IGListAdapter : NSObject

/**
 The view controller that houses the adapter.
 */
@property (nonatomic, nullable, weak) UIViewController *viewController;

/**
 The collection view used with the adapter.
 */
@property (nonatomic, nullable, weak) IGListCollectionView *collectionView;

/**
 The object that acts as the data source for the list adapter.
 */
@property (nonatomic, nullable, weak) id <IGListAdapterDataSource> dataSource;

/**
 The object that receives top-level events for section controllers.
 */
@property (nonatomic, nullable, weak) id <IGListAdapterDelegate> delegate;

/**
 The object that receives `UICollectionViewDelegate` events.

 @note This object *will not* receive `UIScrollViewDelegate` events. Instead use scrollViewDelegate.
 */
@property (nonatomic, nullable, weak) id <UICollectionViewDelegate> collectionViewDelegate;

/**
 The object that receives `UIScrollViewDelegate` events.
 */
@property (nonatomic, nullable, weak) id <UIScrollViewDelegate> scrollViewDelegate;

/**
 A bitmask of experiments to conduct on the adapter.
 */
@property (nonatomic, assign) IGListExperiment experiments;

/**
 Initializes a new `IGListAdapter` object.

 @param updatingDelegate An object that manages updates to the collection view.
 @param viewController   The view controller that will house the adapter.
 @param workingRangeSize The number of objects before and after the viewport to consider within the working range.

 @return A new list adapter object.

 @note The working range is the number of objects beyond the visible objects (plus and minus) that should be
 notified when they are close to being visible. For instance, if you have 3 objects on screen and a working range of 2,
 the previous and succeeding 2 objects will be notified that they are within the working range. As you scroll the list
 the range is updated as objects enter and exit the working range.

 To opt out of using the working range, you can provide a value of `0`.
 */
- (instancetype)initWithUpdater:(id <IGListUpdatingDelegate>)updatingDelegate
                 viewController:(nullable UIViewController *)viewController
               workingRangeSize:(NSUInteger)workingRangeSize NS_DESIGNATED_INITIALIZER;

/**
 Perform an update from the previous state of the data source. This is analagous to calling
 `-[UICollectionView performBatchUpdates:completion:]`.

 @param animated   A flag indicating if the transition should be animated.
 @param completion The block to execute when the update completes.
 */
- (void)performUpdatesAnimated:(BOOL)animated completion:(nullable IGListUpdaterCompletion)completion;

/**
 Perform an immediate reload of the data in the data source, discarding the old objects.

 @param completion The block to execute when the reload completes.
 */
- (void)reloadDataWithCompletion:(nullable IGListUpdaterCompletion)completion;

/**
 Reload the list for only the specified objects.

 @param objects The objects to reload.
 */
- (void)reloadObjects:(NSArray *)objects;

/**
 Query the section index of a list. Constant time lookup.

 @param sectionController A list object.

 @return The section index of the list if it exists, otherwise `NSNotFound`.
 */
- (NSUInteger)sectionForSectionController:(IGListSectionController <IGListSectionType> *)sectionController;

/**
 Returns the section controller for the specified object. Constant time lookup.

 @param object An object from the data source.

 @return An section controller or `nil` if `object` is not in the list.

 @see `-[IGListAdapterDataSource listAdapter:sectionControllerForObject:]`
 */
- (__kindof IGListSectionController <IGListSectionType> * _Nullable)sectionControllerForObject:(id)object;

/**
 Returns the object corresponding to a section in the list. Constant time lookup.

 @param section A section in the list.

 @return The object for the specified section, or `nil` if the section does not exist.
 */
- (nullable id)objectAtSection:(NSUInteger)section;

/**
 Returns the section corresponding to the specified object in the list. Constant time lookup.

 @param object An object in the list.

 @return The section index of `object` if found, otherwise `NSNotFound`.
 */
- (NSUInteger)sectionForObject:(id)object;

/**
 Returns a copy of all the objects currently powering the adapter.

 @return An array of objects.
 */
- (NSArray *)objects;

/**
 An unordered array of the currently visible section controllers.

 @return An array of section controllers.
 */
- (NSArray<IGListSectionController<IGListSectionType> *> *)visibleSectionControllers;

/**
 An unordered array of the currently visible objects.

 @return An array of objects
 */
- (NSArray *)visibleObjects;

/**
 Scrolls to the sepcified object in the list adapter.

 @param object             The object to which to scroll.
 @param supplementaryKinds The types of supplementary views in the section.
 @param scrollDirection    A flag indicating the direction to scroll.
 @param animated           A flag indicating if the transition should be animated.
 */
- (void)scrollToObject:(id)object
    supplementaryKinds:(nullable NSArray<NSString *> *)supplementaryKinds
       scrollDirection:(UICollectionViewScrollDirection)scrollDirection
              animated:(BOOL)animated;

/**
 Returns the size of a cell at the specified index path.

 @param indexPath The index path of the cell.

 @return The size of the cell.
 */
- (CGSize)sizeForItemAtIndexPath:(NSIndexPath *)indexPath;

/**
 Returns the size of a supplementary view in the list at the specified index path.

 @param elementKind The kind of supplementary view.
 @param indexPath   The index path of the supplementary view.

 @return The size of the supplementary view.
 */
- (CGSize)sizeForSupplementaryViewOfKind:(NSString *)elementKind
                             atIndexPath:(NSIndexPath *)indexPath;

/**
 :nodoc:
 */
- (instancetype)init NS_UNAVAILABLE;

/**
 :nodoc:
 */
+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
