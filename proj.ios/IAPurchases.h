//
//  IAPurchases.h
//  SimpleGame
//
//  Created by qsmac on 12/9/14.
//
//

#import <Foundation/Foundation.h>

typedef void (^RequestProductsCompletionHandler)(BOOL success, NSArray * products);


@interface IAPurchases : NSObject

+ (instancetype) alloc __attribute__((unavailable("alloc not available, call user instead")));
- (instancetype) init __attribute__((unavailable("init not available, call user instead")));
+ (instancetype) new __attribute__((unavailable("new not available, call user instead")));


+ (instancetype)sharedInstance;
- (void)requestProductsWithCompletionHandler:(RequestProductsCompletionHandler)completionHandler;

- (void)buyProduct:(NSString *)productIdentifier;

@end
