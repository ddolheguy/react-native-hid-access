import Foundation
import OrigoSDK
import SeosMobileKeysSDK

let LOCK_SERVICE_CODE_HID = 2

var origoKeysManager: OrigoKeysManager?

@objc(OrigoManager)
class OrigoManager: RCTEventEmitter, OrigoKeysManagerDelegate {
  var seosId: UInt = 0
  var origoKeys: [AnyHashable] = []

  private var applicationSetup = false;
  private var registrationCodeRequired = true;
  private var applicationIsStarting = false

  private var lockServiceCodes: [Any] = []
  private var currentlyEnabledOpeningModes: [Any] = []

  private var openingModesWithSeamless: [Any] = []
  private var openingModesWithoutSeamless: [Any] = []

  /*
   =============================================
   EVENTS / LISTENERS
   =============================================
   */
  func origoKeysDidStartup() {
    sendEvent(withName: "origoKeysDidStartup", body: [])

    if applicationIsStarting {
      // 1. Called when Token added
      sendEvent(withName: "applicationStarting", body: [])
      setupEndpoint()
      applicationIsStarting = false
    }
  }

  func origoKeysDidSetupEndpoint() {
    // Called when inially entering a token
    sendEvent(withName: "origoKeysDidSetupEndpoint", body: [])

    cacheSeosId()
    setupEndpoint();
    if !origoKeysManager!.isScanning() {
      toggleScanning(currentlyEnabledOpeningModes, scanMode: OrigoKeysScanMode.optimizePerformance)
    }
  }

  func origoKeysDidFail(toStartup error: Error) {
    sendEvent(withName: "origoKeysDidFail", body: ["error": error.localizedDescription])
  }

  func origoKeysDidFail(toSetupEndpoint error: Error) {
    sendEvent(withName: "origoKeysDidFail", body: ["error": error.localizedDescription])
    toggleScanning(currentlyEnabledOpeningModes, scanMode: OrigoKeysScanMode.optimizePerformance)
  }

  func origoKeysDidUpdateEndpoint() {
    // 2. Called when Token added
    sendEvent(withName: "origoKeysDidUpdateEndpoint", body: [])
    cacheSeosId()
    toggleScanning(currentlyEnabledOpeningModes, scanMode: OrigoKeysScanMode.optimizePerformance)
  }

  func reloadKeys() {
    getKeysFromSeos()
  }

  override init() {
    super.init()

    lockServiceCodes = [LOCK_SERVICE_CODE_HID]

    // A list of opening modes without seamless
    openingModesWithoutSeamless = [
      OrigoKeysOpeningType.motion.rawValue,
      OrigoKeysOpeningType.proximity.rawValue,
      OrigoKeysOpeningType.applicationSpecific.rawValue,
      OrigoKeysOpeningType.enhancedTap.rawValue
    ]

    // A list of opening modes with seamless
//    openingModesWithSeamless = [
//      OrigoKeysOpeningType.motion.rawValue,
//      OrigoKeysOpeningType.proximity.rawValue,
//      OrigoKeysOpeningType.applicationSpecific.rawValue,
//      OrigoKeysOpeningType.seamless.rawValue,
//      OrigoKeysOpeningType.enhancedTap.rawValue
//    ]

    // Opening modes enabled on startup
    currentlyEnabledOpeningModes = openingModesWithoutSeamless
  }

  /*
   =============================================
   REACT NATIVE EXPOSED
   =============================================
   */

  @objc
  func listAllReaders(
    _ resolve: RCTPromiseResolveBlock,
    rejecter reject: RCTPromiseRejectBlock
  ) {
      let readers = origoKeysManager?.listReaders()
      for reader: OrigoKeysReader in readers ?? [] {
          var typeSupported = false
          for openinngType in reader.supportedOpeningTypes {
              let openinngTypeCasted = openinngType as! Int

              typeSupported = openinngTypeCasted == OrigoKeysOpeningType.enhancedTap.rawValue
              typeSupported = openinngTypeCasted == OrigoKeysOpeningType.proximity.rawValue
              typeSupported = openinngTypeCasted == OrigoKeysOpeningType.motion.rawValue
              typeSupported = openinngTypeCasted == OrigoKeysOpeningType.applicationSpecific.rawValue
              break
        }
      }
  }

  /* Initialise - Start setup for HID */
  @objc
  func initialise(
    _ applicationId: String,
    resolver resolve: RCTPromiseResolveBlock,
    rejecter reject: RCTPromiseRejectBlock
  ) -> Void {

    if (applicationSetup == true) {
      resolve(true);
      return;
    }

    let group = DispatchGroup()
    group.enter()

    DispatchQueue.main.async {
      let config: [String : Any] = [
        OrigoKeysOptionApplicationId: applicationId,
        OrigoKeysOptionVersion: "1.0",
        OrigoKeysOptionSuppressApplePay : true,
      ]
      if (origoKeysManager == nil) {
        origoKeysManager = OrigoKeysManager(delegate: self, options: config);
      }

      if (self.isEndpointSetup()) {
        self.registrationCodeRequired = false;
        self.applicationIsStarting = true
        self.cacheSeosId();
        self.toggleScanning(self.currentlyEnabledOpeningModes, scanMode: OrigoKeysScanMode.optimizePerformance)
      } else {
        self.registrationCodeRequired = true;
      }

      origoKeysManager?.startup();

      group.leave()
    }

//    let waitResult = group.wait(timeout: .now() + 10)
//    if (waitResult != DispatchTimeoutResult.success) {
//      return reject("E_COUNT", "count cannot be negative", nil);
//    }
    group.wait()
    applicationSetup = true;
    resolve(registrationCodeRequired);
  }

  @objc
  func setRegistrationCode(_ code: String) {
    // The setup will either give a callback to "OrigoKeysDidSetup" or "OrigoKeysDidFailToSetup"
    DispatchQueue.main.async {
      if (origoKeysManager != nil) {
        origoKeysManager?.setupEndpoint(code)
      }
    }
  }

  @objc
  func refreshEndpoint(
    _ resolve: RCTPromiseResolveBlock,
    rejecter reject: RCTPromiseRejectBlock) {
      if (applicationSetup == false) {
        resolve(false);
        return;
      }

      let group = DispatchGroup()
      group.enter()

      DispatchQueue.main.async {
        if (self.isEndpointSetup()) {
          origoKeysManager?.updateEndpoint()

          var error: NSError? = nil
          let origoKeys = origoKeysManager!.listMobileKeys(&error)
          self.sendEvent(withName: "hasMobileKey", body: ["name": origoKeys.count])

        } else {
          self.sendEvent(withName: "refreshEndpoint", body: ["name": false])
        }
        group.leave()
      }

      group.wait()
      resolve(true);
  }

  @objc
  func hasMobileKey(
    _ resolve: RCTPromiseResolveBlock,
    rejecter reject: RCTPromiseRejectBlock) {
      let group = DispatchGroup()
      group.enter()

      DispatchQueue.main.async {
        var error: NSError? = nil
        let origoKeys = origoKeysManager!.listMobileKeys(&error)
        self.sendEvent(withName: "hasMobileKey", body: ["name": origoKeys.count])
        group.leave()
      }

      group.wait()
      resolve(true);
    }

  @objc
  func isApplicationStarted(
    _ resolve: RCTPromiseResolveBlock,
    rejecter reject: RCTPromiseRejectBlock
  ) {
    resolve(self.applicationIsStarting)
  }

  @objc
  func openClosestReader(
    _ resolve: RCTPromiseResolveBlock,
    rejecter reject: RCTPromiseRejectBlock
  ) {
    let reader: OrigoKeysReader? = origoKeysManager?.closestReader(withinRangeOf: OrigoKeysOpeningType(rawValue: OrigoKeysOpeningType.applicationSpecific.rawValue)!)
    if reader != nil && reader?.uuid != nil {
        var error: NSError? = nil
        origoKeysManager?.connect(to: reader!, openingType: OrigoKeysOpeningType(rawValue: OrigoKeysOpeningType.applicationSpecific.rawValue)!, error: &error)
        if error != nil {
            if let aDescription = error?.debugDescription {
              reject("ERROR", aDescription, error)
            }
        }

        resolve(true);
    } else {
      resolve(false);
    }
  }

  /*
   =============================================
   PRIVATE METHODS
   =============================================
   */
  func getKeysFromSeos() {
    var error: NSError? = nil
    origoKeys = origoKeysManager!.listMobileKeys(&error)
  }

  func setupEndpoint() {
    if isEndpointSetup() {
      // The update will either give a callback to "OrigoKeysDidUpdate" or "OrigoKeysDidFailToUpdate"
      origoKeysManager?.updateEndpoint()
    }
  }

  func isEndpointSetup() -> Bool {
    do {
      try origoKeysManager?.isEndpointSetup()
      // handleError()
    } catch {
      return false
    }
    return true
  }

  func cacheSeosId() {
    var error: NSError? = nil
    let endpointInfo: OrigoKeysEndpointInfo? = origoKeysManager?.endpointInfo(&error)
    seosId = (endpointInfo?.seosId)!
    endpointInfo?.getEnvironmentName()
  }

  func isMobileKey() -> Bool {
    var error: NSError? = nil
    origoKeys = origoKeysManager!.listMobileKeys(&error)
    if origoKeys.count > 0 {
      return true
    }
    return false
  }

  func toggleScanning(_ openingTypes: [Any]?, scanMode: OrigoKeysScanMode) {
    if (isMobileKey()) {
      if (origoKeysManager?.isScanning())! {
        sendEvent(withName: "origoStopScanning", body: ["error": "restarting scanning"])
        origoKeysManager?.stopReaderScan()
      }

      var error: NSError? = nil
      if isMobileKey() {
        origoKeysManager?.startReaderScan(in: scanMode, supportedOpeningTypes: openingTypes!, lockServiceCodes: lockServiceCodes, error: &error)
        sendEvent(withName: "origoStartScanning", body: [])
      } else {
        print("Nothing")
      }
      if error != nil {
        sendEvent(withName: "origoStartScanning", body: ["error": error?.localizedDescription])
      }
    } else {
      origoKeysManager?.stopReaderScan()
      sendEvent(withName: "origoStopScanning", body: [])
    }
  }

  // MARK: Origo Keys Bluetooth Protocol example callback handlers
  func origoKeysDidConnect(to reader: OrigoKeysReader, openingType type: OrigoKeysOpeningType) {
    sendEvent(withName: "origoKeysDidConnect", body: ["name": type.rawValue ])
   }

  func origoKeysDidFailToConnect(to reader: OrigoKeysReader, openingType type: OrigoKeysOpeningType, openingStatus status: OrigoKeysOpeningStatusType) {
    sendEvent(withName: "origoKeysDidFailToConnect", body: ["name": reader.name])
  }

  func origoKeysDidDisconnect(from reader: OrigoKeysReader, openingType type: OrigoKeysOpeningType, openingResult result: OrigoKeysOpeningResult) {

    sendEvent(withName: "origoKeysDidDisconnect", body: ["name": result.status.rawValue])
  }

  // MARK: Reader management
  func origoKeysShouldAttempt(toOpen reader: OrigoKeysReader, openingType type: OrigoKeysOpeningType) -> Bool {
    return true;
  }

  // we need to override this method and
  // return an array of event names that we can listen to
  override func supportedEvents() -> [String]! {
    return ["origoKeysDidStartup", "registrationRequired", "applicationStarting", "origoKeysDidFail", "origoKeysDidSetupEndpoint", "origoKeysDidConnect", "origoKeysDidFailToConnect", "origoKeysDidDisconnect", "origoStartScanning", "origoStopScanning", "origoKeysDidUpdateEndpoint", "refreshEndpoint", "hasMobileKey", "origoLockInRange"]
  }

  override func constantsToExport() -> [AnyHashable : Any]! {
    return ["initialCount": 0]
  }

  override static func requiresMainQueueSetup() -> Bool {
    return true
  }
}

