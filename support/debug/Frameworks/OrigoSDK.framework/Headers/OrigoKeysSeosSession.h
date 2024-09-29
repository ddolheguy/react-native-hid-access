// OrigoKeysSeosSession.h
// Copyright (c) 2018 HID Origo Mobile Services
//
// All rights reserved.

#import "OrigoKeysApduResponse.h"
#import "OrigoKeysApduCommand.h"
#import "OrigoKeysApduConnectionProtocol.h"

@class AMSSessionCrypto;
@class OrigoKeysSessionParameters;

/**
 * Configuration enum to perform the session over contact or contactless
 */
typedef NS_ENUM(NSUInteger, OrigoKeysSeosSessionType) {
    /**
     * Contact session
     */
    OrigoKeysSeosSessionTypeContact = 1,
    /**
     * Contactless session
     */
    OrigoKeysSeosSessionTypeContactless = 2,
};

/**
 * For Origo Access SDK 5.0.0, you normally do not use this class directly. Use the OrigoKeysSeosProvider instead, as
 * that class takes care of ADF Selection, Mutual Authentication and session management.
 *
 * @note since version 5.0.0
 */
@interface OrigoKeysSeosSession : NSObject

/**
 * Property that describes wheteher or not this session currently uses secure messaging, i.e. has a validatied session crypto
 * @since 6.0.0
 */
@property(nonatomic) BOOL usesSecureMessaging;

/**
 * Session parameters for this session.
 */
@property(nonatomic) OrigoKeysSessionParameters * sessionParameters;

/**
 * This gets set whenever an OID is selected
 * @since 6.0.0
 */
@property(nonatomic, strong) NSData *currentlySelectedOid;

/**
 * Constructs a `OrigoKeysSeosSession` with the specified `OrigoKeysApduConnectionProtocol`
 *
 * @param apduConnection    the APDU connection to use in this session
 * @return                  a session instance
 */
- (instancetype)initWithApduConnection:(id <OrigoKeysApduConnectionProtocol>)apduConnection DEPRECATED_MSG_ATTRIBUTE("Use initWithApduConnection:sessionParameters:sessionSemaphore: instead");

- (instancetype)initWithApduConnection:(id <OrigoKeysApduConnectionProtocol>)apduConnection sessionParameters:(OrigoKeysSessionParameters *)sessionParameters sessionSemaphore:(dispatch_semaphore_t)sessionSemaphore;

- (BOOL)closeSessionWithError:(NSError **)error;

/**
 * Check to see if the connection is active
 * @return YES if there is an active connection
 */
- (BOOL)activeConnection;

/**
 * Sends an APDU command and receives a response from the Seos applet. This method allows the caller to utilize the
 * automatic APDU Command splitting and APDU Respose Joining of the SDK.
 *
 * @param command               the data command to send.
 * @param autoSplit             set this to YES to enable auto split of large apdu commands.
 * @param autoJoin              set this to YES to enable auto join of apdu responses.
 * @param error                 if something went wrong this parameter is set, otherwise nil.
 * @return                      the response from Seos processing the command or nil if something went wrong
 */
- (OrigoKeysApduResponse *)processApduCommand:(OrigoKeysApduCommand *)command autoSplitLargeApduCommands:(BOOL)autoSplit autoJoinMultipleApduResponses:(BOOL)autoJoin withError:(NSError **)error;

/**
 * Sends an APDU command and receives a response from the Seos applet.
 * This method is deprecated.
 *
 * @param command           the data command to send.
 * @param ignoreChaining        set this to NO to disable honoring the chaining flag.
 * @param error                 if something went wrong this parameter is set, otherwise nil.
 * @return                      the response from Seos processing the command or nil if something went wrong
 * @deprecated since 5.2.0
 */
- (OrigoKeysApduResponse *)processApduCommand:(OrigoKeysApduCommand *)command ignoreChaining:(BOOL)ignoreChaining withError:(NSError **)error DEPRECATED_MSG_ATTRIBUTE("Use processApduCommand:autoSplitLargeApduCommands:autoJoinMultipleApduResponses:withError: instead");


/**
 * Sends an APDU command and receives a response from the Seos applet.
 *
 * @note Convenience method for `processApduCommand:ignoreChaining:withError` with `ignoreChaining` set to `NO`
 * @param error                 if something went wrong this parameter is set, otherwise nil.
 * @return                      the response from Seos processing the command or nil if something went wrong
 */
- (OrigoKeysApduResponse *)processApduCommand:(OrigoKeysApduCommand *)apduCommand withError:(NSError **)error;

@end
