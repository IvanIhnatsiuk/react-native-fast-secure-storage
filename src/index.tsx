import { NativeModules, Platform } from 'react-native';
import SecureStorage from './SecureStorage';

const LINKING_ERROR =
  `The package 'react-native-fast-secure-storage' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

// @ts-expect-error
const isTurboModuleEnabled = global.__turboModuleProxy != null;

const FastSecureStorageModule = isTurboModuleEnabled
  ? require('./NativeFastSecureStorage').default
  : NativeModules.FastSecureStorage;

const FastSecureStorage = FastSecureStorageModule
  ? FastSecureStorageModule
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

FastSecureStorage.install();

export default SecureStorage;
