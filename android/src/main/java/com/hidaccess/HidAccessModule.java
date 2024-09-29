package com.hidaccess;

import org.jetbrains.annotations.Nullable;

import android.Manifest;
import android.app.Activity;
import android.app.Notification;
import android.content.pm.PackageManager;
import android.util.Log;

import androidx.core.content.ContextCompat;

import com.hidaccess.notification.UnlockNotification;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.modules.core.DeviceEventManagerModule;
import com.hid.origo.OrigoKeysApiFacade;
import com.hid.origo.OrigoKeysApiFactory;
import com.hid.origo.api.OrigoApiConfiguration;
import com.hid.origo.api.OrigoMobileKey;
import com.hid.origo.api.OrigoMobileKeys;
import com.hid.origo.api.OrigoMobileKeysApi;
import com.hid.origo.api.OrigoMobileKeysCallback;
import com.hid.origo.api.OrigoMobileKeysException;
import com.hid.origo.api.OrigoMobileKeysProgressCallback;
import com.hid.origo.api.OrigoProgressEvent;
import com.hid.origo.api.OrigoReaderConnectionController;
import com.hid.origo.api.OrigoReaderConnectionInfoType;
import com.hid.origo.api.ble.OrigoBluetoothMode;
import com.hid.origo.api.ble.OrigoOpeningResult;
import com.hid.origo.api.ble.OrigoOpeningStatus;
import com.hid.origo.api.ble.OrigoOpeningTrigger;
import com.hid.origo.api.ble.OrigoOpeningType;
import com.hid.origo.api.ble.OrigoReader;
import com.hid.origo.api.ble.OrigoReaderConnectionCallback;
import com.hid.origo.api.ble.OrigoReaderConnectionListener;
import com.hid.origo.api.ble.OrigoScanConfiguration;
import com.hid.origo.api.ble.OrigoScanMode;
import com.hid.origo.api.ble.OrigoSeamlessOpeningTrigger;
import com.hid.origo.api.ble.OrigoTapOpeningTrigger;
import com.hid.origo.api.ble.OrigoTwistAndGoOpeningTrigger;
import com.hid.origo.api.hce.OrigoHceConnectionCallback;
import com.hid.origo.api.hce.OrigoHceConnectionListener;
import com.hid.origo.api.hce.OrigoNfcConfiguration;
import com.hid.origo.api.hce.OrigoNfcParameters;

import java.util.Collections;
import java.util.List;

public class HidAccessModule extends ReactContextBaseJavaModule implements OrigoKeysApiFactory, OrigoMobileKeysProgressCallback, OrigoMobileKeysCallback, OrigoKeysApiFacade, OrigoReaderConnectionListener, OrigoHceConnectionListener, ClosestLockTrigger.LockInRangeListener {

    private static final String TAG = "HidAccessModule";

    private static final int ORIGO_LOCK_SERVICE_CODE = 2;
    private static final String APPLICATION_DESCRIPTION = "HID Access";

    private boolean hasInitialised = false;
    private boolean readerInRange = false;
    private ReactApplicationContext reactContext;
    private OrigoMobileKeysApi mobileKeysFactory;
    private ClosestLockTrigger closestLockTrigger = new ClosestLockTrigger(this);
    List<OrigoMobileKey> data = null;

    OrigoManager(ReactApplicationContext context) {
        super(context);

        reactContext = context;
    }

    @Override
    public String getName() {
        return "HidAccessModule";
    }

    @Override
    public OrigoMobileKeys getMobileKeys() {
        try
        {
            if (mobileKeysFactory != null) {
                return mobileKeysFactory.getMobileKeys();
            }
        }
        catch (Exception e)
        {
            Log.e(TAG, "getMobileKeys() error", e);
        }
        return null;
    }

    @Override
    public OrigoReaderConnectionController getReaderConnectionController() {
        return mobileKeysFactory.getOrigiReaderConnectionController();
    }

    @Override
    public OrigoScanConfiguration getOrigoScanConfiguration() {
        return getReaderConnectionController().getScanConfiguration();
    }

    @Override
    public void handleMobileKeysTransactionCompleted() {
        // Mobile keys transaction success/completed callback
        // sendEvent("origoKeysDidSetupEndpoint", null);
        loadKeys();
    }

    @Override
    public void handleMobileKeysTransactionFailed(OrigoMobileKeysException e) {
        // Mobile keys transaction failure callback
        sendEvent("origoKeysDidFail", null);
        loadKeys();
    }

    @Override
    public void onStartUpComplete() {
        sendEvent("applicationStarting", null);
    }

    @Override
    public void onEndpointSetUpComplete() {
        // sendEvent("origoKeysDidSetupEndpoint", null);
    }

    @Override
    public void endpointNotPersonalized() {
        // sendEvent("origoKeysDidSetupEndpoint", null);
    }

    @Override
    public void handleMobileKeysTransactionProgress(OrigoProgressEvent origoProgressEvent) {
        // sendEvent("origoKeysDidSetupEndpoint", null);
    }

    @Override
    public boolean isEndpointSetUpComplete() {
        boolean isEndpointSetup = false;
        try
        {
            OrigoMobileKeys mobileKeys = this.getMobileKeys();
            if (mobileKeys != null) {
                isEndpointSetup = mobileKeys.isEndpointSetupComplete();
            }
        }
        catch (OrigoMobileKeysException e)
        {
            Log.e(TAG, "isEndpointSetUpComplete() error", e);
        }
        return isEndpointSetup;
    }

    @Override
    public void onReaderConnectionOpened(OrigoReader origoReader, OrigoOpeningType origoOpeningType) {
        try
        {
            // Callback method when a reader session is started.
            WritableMap params = Arguments.createMap();
            if (origoReader != null) {
                params.putString("name", origoReader.getName());
            } else {
                params.putString("name", "Not found");
            }
            sendEvent("origoKeysDidConnect", params);
        }
        catch (Exception e)
        {
            Log.e(TAG, "onReaderConnectionOpened() error", e);
        }
    }

    @Override
    public void onReaderConnectionClosed(OrigoReader origoReader, OrigoOpeningResult origoOpeningResult) {
        try
        {
            //Callback when a connection could be initialized.
            WritableMap params = Arguments.createMap();
            if (origoReader != null) {
                params.putString("name", origoReader.getName());
            } else {
                params.putString("name", "Not found");
            }
            sendEvent("origoKeysDidDisconnect", params);
        }
        catch (Exception e)
        {
            Log.e(TAG, "onReaderConnectionClosed() error", e);
        }
    }

    @Override
    public void onReaderConnectionFailed(OrigoReader origoReader, OrigoOpeningType origoOpeningType, OrigoOpeningStatus origoOpeningStatus) {
        try
        {
            //Callback when a connection could not be initialized.
            WritableMap params = Arguments.createMap();
            if (origoReader != null) {
                params.putString("name", origoReader.getName());
            } else {
                params.putString("name", "Not found");
            }
            sendEvent("origoKeysDidFailToConnect", params);
        }
        catch (Exception e)
        {
            Log.e(TAG, "onReaderConnectionFailed() error", e);
        }
    }

    @Override
    public void onHceSessionOpened() {
        //Callback to the implementing service when a HCE session with a reader has been initialized.
    }

    @Override
    public void onHceSessionClosed(int i) {
        //Callback to the implementing service when a HCE session with a reader has been closed.
    }

    @Override
    public void onHceSessionInfo(OrigoReaderConnectionInfoType origoReaderConnectionInfoType) {
        //Callback when a potentially interesting event happens on the connection, that is not sessionOpened or sessionClosed.
    }

    @ReactMethod
    public void initialise(String applicationId, Promise promise) {
        try {
            if (mobileKeysFactory != null) {
                promise.resolve(true);
                return;
            }
            OrigoScanConfiguration origoScanConfiguration = new OrigoScanConfiguration.Builder(
                    new OrigoOpeningTrigger[]{
                            new OrigoTapOpeningTrigger(reactContext),
                            new OrigoTwistAndGoOpeningTrigger(reactContext),
                            new OrigoSeamlessOpeningTrigger()
                    }, ORIGO_LOCK_SERVICE_CODE)
                    .setAllowBackgroundScanning(false)
//                    .setScanMode(OrigoScanMode.OPTIMIZE_POWER_CONSUMPTION)
                    .setBluetoothModeIfSupported(OrigoBluetoothMode.DUAL)
                    .build();

            OrigoApiConfiguration origoApiConfiguration = new OrigoApiConfiguration.Builder().setApplicationId(applicationId)
                    .setApplicationDescription(APPLICATION_DESCRIPTION)
                    .setNfcParameters(new OrigoNfcConfiguration.Builder().build())
                    .build();

            mobileKeysFactory = OrigoMobileKeysApi.getInstance();
            mobileKeysFactory.initialize(reactContext, origoApiConfiguration, origoScanConfiguration, applicationId);
            if (!mobileKeysFactory.isInitialized()) {
                throw new IllegalStateException();
            }

            OrigoMobileKeys mobileKeys = this.getMobileKeys();
            mobileKeys.applicationStartup(this);
            hasInitialised = true;

            if (isEndpointSetUpComplete()) {
                mobileKeys.endpointUpdate(this);
            } else {
                sendEvent("registrationRequired", null);
            }

            OrigoReaderConnectionCallback readerConnectionCallback = new OrigoReaderConnectionCallback(reactContext);
            readerConnectionCallback.registerReceiver(this);

            OrigoHceConnectionCallback hceConnectionCallback = new OrigoHceConnectionCallback(reactContext);
            hceConnectionCallback.registerReceiver(this);

            this.getOrigoScanConfiguration().getRootOpeningTrigger().add(closestLockTrigger);

            closestLockTrigger.checkReaders();
            promise.resolve(true);
        } catch(Exception ex) {
            promise.reject("Error starting HID", ex);
        }
    }

    /**
     * Load mobile keys api
     */
    private void loadKeys() {
        try {
            data = this.getMobileKeys().listMobileKeys();
        } catch (OrigoMobileKeysException e) {
            Log.e(TAG, "Failed to list keys", e);
        }
        if (data == null) {
            data = Collections.emptyList();
        }

        //Update scanning based if we have keys
        if (data.isEmpty()) {
            stopScanning();
        } else {
            startScanning();
        }
    }

    @ReactMethod
    public void setRegistrationCode(String code) {
        this.getMobileKeys().endpointSetup(this, code);
    }

    @ReactMethod
    public void openClosestReader(Promise promise) {
        if (!readerInRange) {
            closestLockTrigger.checkReaders();
        }

        if (readerInRange) {
            closestLockTrigger.openClosestReader();
        }
        promise.resolve(true);
    }

    @ReactMethod
    public void refreshEndpoint(Promise promise) {
        WritableMap params = Arguments.createMap();
        if (isEndpointSetUpComplete()) {
            this.getMobileKeys().endpointUpdate(this);
            this.checkMobileKeys();
        } else {
            params.putBoolean("name", false);
            sendEvent("refreshEndpoint", params);
        }
        promise.resolve(true);
    }

    @ReactMethod
    public void hasMobileKey(Promise promise) {
        this.checkMobileKeys();
        promise.resolve(true);
    }

    private void checkMobileKeys() {
        WritableMap params = Arguments.createMap();
        try {
            int size = this.getMobileKeys().listMobileKeys().size();
            params.putInt("name", size);
            sendEvent("hasMobileKey", params);
        } catch (OrigoMobileKeysException e) {
            params.putInt("name", 0);
            sendEvent("hasMobileKey", params);
        }
    }

    /**
     * Check if app has location permission
     */
    private boolean hasLocationPermissions() {
        return (ContextCompat.checkSelfPermission(reactContext,
                Manifest.permission.ACCESS_COARSE_LOCATION) == PackageManager.PERMISSION_GRANTED ||
                ContextCompat.checkSelfPermission(reactContext,
                        Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED);
    }

    /**
     * Start BLE scanning or request permission
     */
    private void startScanning() {
        try {
            if (hasLocationPermissions()) {
                Log.d(TAG, "Starting BLE service and enabling HCE");
                OrigoReaderConnectionController controller = OrigoMobileKeysApi.getInstance().getOrigiReaderConnectionController();
                controller.enableHce();

                Notification notification = UnlockNotification.create(reactContext);
                controller.startForegroundScanning(notification);
            }
        } catch (Exception e) {
            Log.e(TAG, "startScanning() error", e);
        }
    }

    /**
     * Stop BLE scanning or
     */
    private void stopScanning() {
        OrigoReaderConnectionController controller = OrigoMobileKeysApi.getInstance().getOrigiReaderConnectionController();
        controller.stopScanning();
    }

    private void sendEvent(String eventName,
                           @Nullable WritableMap params) {
        this.reactContext
                .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
                .emit(eventName, params);
    }

    @Override
    public void onLockInRange(boolean lockInRange) {
        if (readerInRange != lockInRange) {
            readerInRange = lockInRange;

            WritableMap params = Arguments.createMap();
            params.putBoolean("inRange", lockInRange);
            sendEvent("origoLockInRange", params);
        }
    }
}
