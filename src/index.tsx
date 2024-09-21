import { NativeModules } from 'react-native';

// 1. Initialise the HID module with Application ID
export const initialise = (applicationId: string) => {
  return NativeModules.OrigoManager.initialise(applicationId);
};

export const refreshEndpoint = async () => {
  await NativeModules.OrigoManager.refreshEndpoint();
};

export const hasMobileKey = async () => {
  await NativeModules.OrigoManager.hasMobileKey();
};

// 2. Register HID registration code
export const setRegistrationCode = (code: string) => {
  NativeModules.OrigoManager.setRegistrationCode(code);
};

// 3. Open to the closest reader
export const openClosestReader = () => {
  return NativeModules.OrigoManager.openClosestReader();
};
