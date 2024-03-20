//
//  YCAspects.h
//  YCAspects
//
//  Created by maple on 2024/3/20.
//

#import <Foundation/Foundation.h>

typedef NS_OPTIONS(NSUInteger, AspectOptions) {
    AspectPositionAfter   = 0,            /// Called after the original implementation (default)
    AspectPositionInstead = 1,            /// Will replace the original implementation.
    AspectPositionBefore  = 2,            /// Called before the original implementation.
    
    AspectOptionAutomaticRemoval = 1 << 3 /// Will remove the hook after the first execution.
};

@protocol AspectToken <NSObject>

- (BOOL)remove;

@end

@protocol AspectInfo <NSObject>

- (id)instance;

- (NSInvocation *)originalInvocation;

- (NSArray *)arguments;

@end

@interface NSObject (YCAspects)

+ (id<AspectToken>)aspect_hookSelector:(SEL)selector
                             withOptions:(AspectOptions)options
                              usingBlock:(id)block
                                   error:(NSError **)error;


@end

typedef NS_ENUM(NSUInteger, AspectErrorCode) {
    AspectErrorSelectorBlacklisted,                   /// Selectors like release, retain, autorelease are blacklisted.
    AspectErrorDoesNotRespondToSelector,              /// Selector could not be found.
    AspectErrorSelectorDeallocPosition,               /// When hooking dealloc, only AspectPositionBefore is allowed.
    AspectErrorSelectorAlreadyHookedInClassHierarchy, /// Statically hooking the same method in subclasses is not allowed.
    AspectErrorFailedToAllocateClassPair,             /// The runtime failed creating a class pair.
    AspectErrorMissingBlockSignature,                 /// The block misses compile time signature info and can't be called.
    AspectErrorIncompatibleBlockSignature,            /// The block signature does not match the method or is too large.
    
    AspectErrorRemoveObjectAlreadyDeallocated = 100   /// (for removing) The object hooked is already deallocated.
};

extern NSString *const AspectErrorDomain;
