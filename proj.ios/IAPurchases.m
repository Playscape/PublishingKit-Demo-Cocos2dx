//
//  IAPurchases.m
//  SimpleGame
//
//  Created by qsmac on 12/9/14.
//
//

#import "IAPurchases.h"
#import <StoreKit/StoreKit.h>
#import <objc/runtime.h>

@interface IAPurchases () <SKProductsRequestDelegate, SKPaymentTransactionObserver>

//      Purchases
@property (strong, nonatomic) SKProductsRequest *productsRequest;
@property (strong, nonatomic) RequestProductsCompletionHandler completionHandler;
@property (strong, nonatomic) NSSet *productIdentifiers;

@property (strong, nonatomic) NSArray *skProducts;

@end

@implementation IAPurchases

+ (instancetype)sharedInstance {
    static dispatch_once_t once_token = 0;
    static id shared = nil;
    dispatch_once(&once_token, ^{
        shared = [[super alloc] initUniqueInstance];
    });
    return shared;
}

- (instancetype)initUniqueInstance {
    _productIdentifiers = [NSSet setWithArray:@[ [NSString stringWithFormat:@"%@.ninja_stars_item", [[NSBundle mainBundle] bundleIdentifier]],
                                                 [NSString stringWithFormat:@"%@.ninja_sword_item", [[NSBundle mainBundle] bundleIdentifier]],
                                                 [NSString stringWithFormat:@"%@.samurai_shield_item", [[NSBundle mainBundle] bundleIdentifier]],
                                                 [NSString stringWithFormat:@"%@.shogun_katana_item", [[NSBundle mainBundle] bundleIdentifier]] ]];
    
    return [super init];
}

- (void)requestProductsWithCompletionHandler:(RequestProductsCompletionHandler)completionHandler {
    _completionHandler = [completionHandler copy];
    
    _productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:_productIdentifiers];
    _productsRequest.delegate = self;
    [_productsRequest start];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    NSLog(@"Loaded list of products...");
    _productsRequest = nil;
    
    NSArray *skProducts = response.products;
    
    for (SKProduct *skProduct in skProducts) {
        NSLog(@"Found product: %@ %@ %@ %0.2f",
              skProduct.productIdentifier,
              skProduct.localizedTitle,
              [skProduct.priceLocale localeIdentifier],
              skProduct.price.floatValue);
    }
    
    if (_completionHandler != nil) {
        if (skProducts.count > 0) {
            _skProducts = [skProducts copy];
            _completionHandler(YES, skProducts);
        } else {
            _completionHandler(NO, nil);
        }
        _completionHandler = nil;
    }
}

- (void)buyProduct:(NSString *)productIdentifier {
    if (productIdentifier != nil) {
        SKProduct *currentProduct;
        
        for (SKProduct *product in _skProducts) {
            if ([product.productIdentifier isEqualToString:productIdentifier]) {
                currentProduct = product;
                break;
            }
        }
        
        if (currentProduct != nil) {
            [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
            
            SKPayment *payment = [SKPayment paymentWithProduct:currentProduct];
            [[SKPaymentQueue defaultQueue] addPayment:payment];
        }
    }
}

#pragma mark - SKPaymentTransactionObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    for (SKPaymentTransaction * transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
                break;
            default:
                break;
        }
    };
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    if (transaction.payment.quantity > 0) {
    } else {
        NSLog(@"Something went wrong!");
    }
    
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"failedTransaction...");
    
    if (transaction.error.code != SKErrorPaymentCancelled) {
        NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
        
    } else if (transaction.error.code != SKErrorClientInvalid) {
        NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
        
    } else {
    }
    
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

@end
