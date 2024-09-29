// OrigoKeysSessionParameters.h
// Copyright (c) 2018 HID Origo Mobile Services
//
// All rights reserved.

#import <Foundation/Foundation.h>

@class OrigoKeysPrivacyKeySet;
@class OrigoKeysAuthenticationKeySet;
@class OrigoKeysSymmetricKeyPair;
/**
 * Describes the type of ADF selection to use during session establishment.
 */
typedef NS_ENUM(NSInteger, OrigoKeysSelectionType) {
    /**
     * Generates a command to select an ADF as specified by the SELECT ADF by sequence of OID with Privacy.
     */
    OrigoKeysSelectionADF,
    /**
     * Selects the GDF.
     */
    OrigoKeysSelectionGDF,
    /**
     *  Generates a command to select an ADF. This version of select ignores the disabled state of an ADF, and can
     *  be used to select an ADF even if it's disabled.
     *
     *  @note This version of select ADF requires previous authentication with a key granting
     *  "core administration" permissions.
     */
    OrigoKeysSelectionExtendedADF,
    /**
     * Selects an empty Seos card, use the {@link GenesisPrivacyKeyset} for privacy when using this
     * selection mode.
     */
    OrigoKeysSelectionEmptySeos,
    /**
     * Do not perform any selection.
     */
    OrigoKeysSelectionNone
};


typedef NS_ENUM(NSInteger, OrigoKeysSessionClientIdentifier) {
    /**
     * Specifies that the session was initiated by an unspecified client.
     */
    OrigoKeysSessionClientUnspecified =0x01,
    /**
     * Specifies that the session was initiated by the Origo Access SDK Bluetooth subsystem.
     */
    OrigoKeysSessionClientBle=0x02,
    /**
     * Specifies that the session was initiated by the Origo Access SDK TSM Communication subsystem.
     */
    OrigoKeysSessionClientTsm=0x03,
    /**
     * Specifies that the session was initiated by the Origo Access SDK Readback Communication subsystem.
     */
    OrigoKeysSessionClientReadback=0x04,
    /**
     * Specifies that the session was initiated by the Origo Access SDK Readback Communication subsystem.
     */
    OrigoKeysSessionClientInternal=0x05,
    
    /**
     * Specifies that the session was initiated by a third party client
     */
    OrigoKeysSessionClientThirdParty=0x06
};


/**
 * Represents key number of a key in an ADF
 */
typedef NS_ENUM(uint8_t, OrigoKeysKeyNumber) {
    ORIGO_KEYS_KEY_0 = 0x00,
    ORIGO_KEYS_KEY_1 = 0x01,
    ORIGO_KEYS_KEY_2 = 0x02,
    ORIGO_KEYS_KEY_3 = 0x03,
    ORIGO_KEYS_KEY_4 = 0x04,
    ORIGO_KEYS_KEY_5 = 0x05,
    ORIGO_KEYS_KEY_6 = 0x06,
    ORIGO_KEYS_KEY_7 = 0x07,
    ORIGO_KEYS_KEY_8 = 0x08,
    ORIGO_KEYS_KEY_9 = 0x09,
    ORIGO_KEYS_KEY_10 = 0x0A,
    ORIGO_KEYS_KEY_11 = 0x0B,
    ORIGO_KEYS_KEY_12 = 0x0C,
    ORIGO_KEYS_KEY_13 = 0x0D,
    ORIGO_KEYS_KEY_14 = 0x0E,
    ORIGO_KEYS_KEY_15 = 0x0F,
    ORIGO_KEYS_KEY_16 = 0x10,
    ORIGO_KEYS_KEY_17 = 0x11,
    ORIGO_KEYS_KEY_18 = 0x12,
    ORIGO_KEYS_KEY_19 = 0x13,
    ORIGO_KEYS_KEY_20 = 0x14,
    ORIGO_KEYS_KEY_21 = 0x15,
    ORIGO_KEYS_KEY_22 = 0x16,
    ORIGO_KEYS_KEY_23 = 0x17,
    ORIGO_KEYS_KEY_24 = 0x18,
    ORIGO_KEYS_KEY_25 = 0x19,
    ORIGO_KEYS_KEY_26 = 0x1A,
    ORIGO_KEYS_KEY_27 = 0x1B,
    ORIGO_KEYS_KEY_28 = 0x1C,
    ORIGO_KEYS_KEY_29 = 0x1D,
    ORIGO_KEYS_KEY_30 = 0x1E,
    ORIGO_KEYS_KEY_31 = 0x1F,
    
    ORIGO_KEYS_KEY_0_GLOBAL = 0x40,
    ORIGO_KEYS_KEY_1_GLOBAL = 0x01 | ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_2_GLOBAL = 0x02 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_3_GLOBAL = 0x03 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_4_GLOBAL = 0x04 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_5_GLOBAL = 0x05 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_6_GLOBAL = 0x06 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_7_GLOBAL = 0x07 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_8_GLOBAL = 0x08 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_9_GLOBAL = 0x09 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_10_GLOBAL = 0x0A & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_11_GLOBAL = 0x0B & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_12_GLOBAL = 0x0C & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_13_GLOBAL = 0x0D & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_14_GLOBAL = 0x0E & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_15_GLOBAL = 0x0F & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_16_GLOBAL = 0x10 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_17_GLOBAL = 0x11 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_18_GLOBAL = 0x12 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_19_GLOBAL = 0x13 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_20_GLOBAL = 0x14 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_21_GLOBAL = 0x15 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_22_GLOBAL = 0x16 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_23_GLOBAL = 0x17 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_24_GLOBAL = 0x18 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_25_GLOBAL = 0x19 & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_26_GLOBAL = 0x1A & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_27_GLOBAL = 0x1B & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_28_GLOBAL = 0x1C & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_29_GLOBAL = 0x1D & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_30_GLOBAL = 0x1E & ORIGO_KEYS_KEY_0_GLOBAL,
    ORIGO_KEYS_KEY_31_GLOBAL = 0x1F & ORIGO_KEYS_KEY_0_GLOBAL,
    
};

/**
 * Seos encryption algorithms supported by the API
 */
typedef NS_ENUM(Byte, OrigoKeysEncryptionAlgorithm) {
    /**
     * Unknown or unsupported algorithm
     */
    ORIGO_KEYS_ENCRYPTION_ALGO_ERROR = 0x00,
    /**
     * AES 128
     */
    ORIGO_KEYS_ENCRYPTION_ALGO_AES_128 = 0x09
};

/**
 * Seos hash algorithms supported by the API
 */
typedef NS_ENUM(Byte, OrigoKeysHashAlgorithm) {
    /**
     * Unknown or unsupported hash algorithm
     */
    ORIGO_KEYS_HASH_ALGO_ERROR = 0x00,
    /**
     * HMAC SHA1
     * @warning currently not supported
     */
    ORIGO_KEYS_HASH_ALGO_HMAC_SHA1 = 0x06,
    /**
     * HMAC SHA256
     */
    ORIGO_KEYS_HASH_ALGO_HMAC_SHA_256 = 0x07,
    /**
     * CMAC AES
     */
    ORIGO_KEYS_HASH_ALGO_CMAC_AES = 0x09,
};

/**
 * When establishing a session to Seos, pass a MobileKeysSessionParameters to MobileKeysSeosProvider#openSessionWithParams:withError: to configure the
 * session. Here's an overview of the parameters and what they are used for:
 *  - mobileKeysSelectionType: The type of initial selection that should be performed when establishing the session
 *  - mobileKeysPrivacyKeySet: The keyset used to decrypt the initial selection response for any selection type other than
 *  MobileKeysSelectionType#MobileKeysSelectionNone
 *  -  mobileKeysAuthenticationKeySet: The authentication Keyset
 *  - contactless: NO if this is a contactless connection
 *  - requireSelectAID: YES if the Seos AID should be selected before the session is established
 *  - useTestVectors: YES if the session should be established using test vectors
 *  - requiresSecureMessaging: If NO, the session establishement will not do mutual authentication
 *  - oidSelectionList: A list of OIDs that should be used when the MobileKeysSeosProvider selects the ADF or GDF
 * @note since version 5.0.0
 */
@interface OrigoKeysSessionParameters : NSObject

/**
 * The type of selection that the OrigoKeysSeosProvider#openSessionWithParams:withError: should perform.
 *  - OrigoKeysSelectionADF : Select an ADF
 *  - OrigoKeysSelectionGDF : Select the GDF
 *  - OrigoKeysSelectionExtendedADF : Administration
 *  - OrigoKeysSelectionNone : do not perform an initial select
 */
@property(nonatomic) OrigoKeysSelectionType origoKeysSelectionType;

/**
 * Open Sessions can be classified by the client identifier.
 *  - OrigoKeysSessionClientUnspecified : Select an ADF
 *  - OrigoKeysSessionClientBle : Internally used by the Origo Access SDK. Don't use.
 *  - OrigoKeysSessionClientTsm : Internally used by the Origo Access SDK. Don't use.
 *  - OrigoKeysSessionClientThirdParty : Third party integrators should use this.
 */
@property(nonatomic) OrigoKeysSessionClientIdentifier origoKeysSessionClientIdentifier;

/**
 * This is privacy keyset used when selecting the ADF or GDF.
 */
@property(nonatomic, strong) OrigoKeysPrivacyKeySet *origoKeysPrivacyKeySet;
/**
 * This is the authentication keyset used for mutual authentication when setting up a session with Seos.
 * Set this to nil if no mutual authentication should be performed.
 *
 * @see {@link OrigoKeysAuthenticationKeyset}
 * @see {@link OrigoKeysMasterAuthenticationKeyset}
 */
@property(nonatomic, strong) OrigoKeysAuthenticationKeySet *origoKeysAuthenticationKeySet;

/**
 * Not implemented yet. Let us know if this is needed. :)
 */
@property(nonatomic, strong) OrigoKeysSymmetricKeyPair *origoKeysKekKey;

/**
 * If the Seos implementation supports it, this parameter will define whether or not the communication will be
 * performed over the contactless or the contact interface.
 */
@property(nonatomic) BOOL contactless;

/**
 * If this property is set to YES, the SDK will gladly accept payload larger than 255 bytes, and will automatically split
 * these APDUs into multiple commands. Normally, this can be used to PUT data larger than 255 bytes to a tag.
 * Defaults to NO
 */
@property(nonatomic) BOOL autoSplitLargeApduCommands;

/**
 * If the property is set to YES, the SDK will automatically request the next response if a ApduResponse indicates that
 * there is more data to read. Normally, this can be used to GET data from a tag where the data is larger than 255 bytes.
 * Defaults to NO.
 */
@property(nonatomic) BOOL autoJoinMultipleApduResponses;

/**
 * If this parameter is set to YES, the session will initially select the Seos application by it's AID.
 * Default value is YES.
 */
@property(nonatomic) BOOL requireSelectAid;
/**
 * If this parameter is YES, the mutual authentication to Seos will be performed using hard-coded test vectors
 * according to the Seos Specification.
 */
@property(nonatomic) BOOL useTestVectors;
/**
 * A list of OIDs that will be used when selecting the ADF or the GDF. It is possible to pass multiple OIDs,
 * and Seos will try to select them in the order in which they are specified. Only one OID will be selected however,
 * and the OID of the successfully selected file will be returned by the OrigoKeysSeosProvider#openSessionWithParams:withError:
 * method
 *
 * Seos supports partial selects, and keep in mind that if multiple keys match a partial OID selection, the first found key
 * (undefined order) will be selected.
 */
@property(nonatomic, strong) NSArray <NSData *> *oidSelectionList;

/**
 * A timeout when acquiring a seos session, since Seos can only handle one session at time. Defaults to "0" meaning that session opening fails immediately if Seos is busy.
 * If this is set to a positive number larger than zero, the thread will lock and wait for the specified amount waiting for a Seos session to become
 * available. If no session can be opened within timeout seconds, the session opening will fail.
 * The timeout needs to be specified in miliseconds.
 * @since 6.1.0 The timeout needs to be specified in miliseconds. This change was neccessary to avoid bugs in 5.5.0
 */
@property(nonatomic) NSInteger timeoutInMilliSeconds;

/**
 * A dictionary of options to pass to the ApduConnectionProtocol instance on setup. This dictionary will be passed "as is"
 * to the OrigoKeysApduConnectionProtocol when the session is opened. The actual options themselves are implementation
 * specific. If this property is nil, no options will be passed.
 */
@property (nonatomic, strong) NSDictionary  *apduConnectionPreSessionSetupOptions;

/**
 * A dictionary of options to pass to the ApduConnectionProtocol instance on teardown. This dictionary will be passed "as is"
 * to the OrigoKeysApduConnectionProtocol when the session is closed. The actual options themselves are implementation
 * specific. If this property is nil, no options will be passed.
 */
@property (nonatomic, strong) NSDictionary  *apduConnectionPostSessionTeardownOptions;


/**
 * Constructs a `OrigoKeysSessionParameters` using the selected parameters
 *
 * @param origoKeysSelectionType           the type of Selection to be performed. Specifies the type of select (ADF or GDF) to use in the session
 * @param origoKeysPrivacyKeySet           the privacy credential to use to protect privacy during ADF/GDF selection
 * @param origoKeysAuthenticationKeySet    authentication credential to use during mutual authentication. If no authentication key is specified, no mutual authentication will be performed during session establishment.
 * @return                                  an instance of `OrigoKeysSessionParameters`
 */
- (instancetype)initWithOrigoKeysSelectionType:(OrigoKeysSelectionType)origoKeysSelectionType origoKeysPrivacyKeySet:(OrigoKeysPrivacyKeySet *)origoKeysPrivacyKeySet origoKeysAuthenticationKeySet:(OrigoKeysAuthenticationKeySet *)origoKeysAuthenticationKeySet;

/**
 * Constructs a `OrigoKeysSessionParameters` using the selected parameters
 *
 * @param origoKeysSelectionType           the type of Selection to be performed. Specifies the type of select (ADF or GDF) to use in the session
 * @param origoKeysPrivacyKeySet           the privacy credential to use to protect privacy during ADF/GDF selection
 * @return                                  an instance of `OrigoKeysSessionParameters`
 */
- (instancetype)initWithOrigoKeysSelectionType:(OrigoKeysSelectionType)origoKeysSelectionType origoKeysPrivacyKeySet:(OrigoKeysPrivacyKeySet *)origoKeysPrivacyKeySet;

/**
 * Constructs a `OrigoKeysSessionParameters` using the selected parameters
 *
 * @param origoKeysSelectionType           the type of Selection to be performed. Specifies the type of select (ADF or GDF) to use in the session
 * @return                                  an instance of `OrigoKeysSessionParameters`
 */
- (instancetype)initWithOrigoKeysSelectionType:(OrigoKeysSelectionType)origoKeysSelectionType;

+ (instancetype)parametersForNoSecurityWithSelectAid:(BOOL)shouldSelectAid contacless:(BOOL)contactless;

+ (instancetype)parametersForOrigoKeysBle;

+ (instancetype)parametersForOrigoKeysTsm;

+ (instancetype)parametersForOrigoKeysCache;

+ (instancetype)parametersForOrigoKeysReadback;

- (instancetype)initWithContactless:(BOOL)contactless;

+ (instancetype)parametersWithContactless:(BOOL)contactless;


/**
 * Converts a number from it's numbered representation to the enum defined by the SDK.
 * It will return `Origo_KEYS_ENCRYPTION_ALGO_ERROR` if the encryption algorithm is unsupported or not known
 * @param number a number to convert
 * @return a OrigoKeysEncryptionAlgorithm
 */
+ (OrigoKeysEncryptionAlgorithm)encryptionAlgorithmForNumber:(Byte)number;

/**
 * Converts a number from it's numbered representation to the enum defined by the SDK.
 * It will return `Origo_KEYS_HASH_ALGO_ERROR` if the hash algorithm is unsupported or not known
 * @param number a number to convert
 * @return a OrigoKeysHashAlgorithm
 */
+ (OrigoKeysHashAlgorithm)hashAlgorithmForNumber:(Byte)number;

/**
 * Converts a OrigoKeysEncryptionAlgorithm to it's numbered representation
 * @param algorithm a OrigoKeysEncryptionAlgorithm to convert
 * @return a number
 */
+ (Byte)numberForEncryptionAlgorithm:(OrigoKeysEncryptionAlgorithm)algorithm;

/**
 * Converts a OrigoKeysHashAlgorithm to it's numbered representation
 * @param algorithm a OrigoKeysHashAlgorithm to convert
 * @return a number
 */
+ (Byte)numberForHashAlgorithm:(OrigoKeysHashAlgorithm)algorithm;

/**
 * Convenience method for setting a single oid to select during Seos Session opening
 * @param singleOid NSData
 * @since 6.0.0
 */
-(void) setSingleOidSelection:(NSData *) singleOid;

@end
