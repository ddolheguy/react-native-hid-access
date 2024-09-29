// OrigoDelegateProxy.h
// Copyright (c) 2018 HID Origo Mobile Services
//
// All rights reserved.

#import "OrigoKeysManager.h"

#import <Foundation/Foundation.h>
#import "OrigoKeysManagerDelegate.h"
#import <SeosMobileKeysSDK/SeosMobileKeysSDK.h>
#import "OrigoEventManager.h"

@interface OrigoDelegateProxy : NSObject <MobileKeysManagerExtendedDelegate>

@property(nonatomic, strong) OrigoEventManager *eventManager;
@property(nonatomic, strong) OrigoKeysManager *origoKeysManager;


- (instancetype)initWithDelegate:(id)delegate;
- (BOOL)respondsToUserDidUnlockGesture;

@end
