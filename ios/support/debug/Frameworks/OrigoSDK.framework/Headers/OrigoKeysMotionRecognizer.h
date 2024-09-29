// OrigoKeysMotionRecognizer.h
// Copyright (c) 2018 HID Origo Mobile Services
//
// All rights reserved.


#import <Foundation/Foundation.h>

/**
 * The client who wishes to receieve callbacks when the Twist and Go motion has been detected.
 */
@protocol OrigoKeysMotionRecognizerDelegate <NSObject>
- (void)userMadeUnlockGesture;
@end

/**
 * Implementation of the ASSA ABLOY patented Twist and Go motion. The user turns his phone in one direction
 * and then back again, simulating turning a door knob.
 *
 * @warning Opening the closest reader using Twist and Go is already supported by the SDK. Simply pass
 * `+[OrigoKeysOpeningType OrigoKeysOpeningTypeMotion]` as a one of the values in the array of `supportedOpeningTypes`
 * when calling `-[OrigoKeysManager startReaderScanInMode:supportedOpeningTypes:lockServiceCodes:error:]`.
 * You (the integrator) should only use this class if the default Twist and Go opening implementation in the SDK does not cover
 * your specific use case.
 *
 * @note to preserve power, please use the `-[OrigoKeysMotionRecognizer startTrackingRotation]` and
 * `-[OrigoKeysMotionRecognizer stopTrackingRotation]` when your use case does not mandate that the Twist and Go
 * motion pattern shouldn't be detected.
 */
@interface OrigoKeysMotionRecognizer : NSObject

@property(weak, nonatomic) id <OrigoKeysMotionRecognizerDelegate> internalDelegate DEPRECATED_MSG_ATTRIBUTE("This property will be renamed to delegate");
@property(strong, nonatomic) dispatch_queue_t queue;

/**
 * Initializes the `OrigoKeysMotionRecognizer`
 * delegate of `OrigoKeysMotionRecognizerDelegate` to send callbacks to.
 */
- (instancetype)initWithDelegate:(id<OrigoKeysMotionRecognizerDelegate>)delegate NS_DESIGNATED_INITIALIZER;

/**
 * The class overrides `-[NSObject dealloc]` to unregister itself from the `CMMotionManager` upon deallocation
 */
- (void)dealloc;

/**
 * Start listening to the gyroscope pattern, if not already listening.
 */
- (void)startTrackingRotation;

/**
 * Stop listening to the gyroscope
 */
- (void)stopTrackingRotation;

/**
 * @return YES if the OrigoKeysMotionRecognizer is currently listening for callbacks from the `CMMotionManager`, otherwise NO.
 */
- (BOOL)isTrackingRotation;

@end
