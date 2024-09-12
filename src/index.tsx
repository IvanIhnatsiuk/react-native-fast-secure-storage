import { NativeModules, Platform } from 'react-native';

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

export class SecureStorage {
  private functionCache = {};

  private instance = global.__SecureStorage;

  constructor() {
    this.functionCache = {};
  }

  private getFunctionFromCache(functionName: string) {
    if (!this.functionCache[functionName]) {
      this.functionCache[functionName] = this.instance[functionName];
    }
    return this.functionCache[functionName];
  }

  public async setItem(
    key: string,
    value: string,
    accessible = 'AccessibleWhenUnlocked'
  ) {
    const func = this.getFunctionFromCache('setItem');
    const result = func(key, value, accessible);

    return result;
  }

  public getItem(key: string) {
    const func = this.getFunctionFromCache('getItem');

    const result = func(key);
    return result;
  }

  public clearStorage = () => {
    const func = this.getFunctionFromCache('clearStorage');
    const result = func();
    return result;
  };

  public setItems = (
    items: { key: string; value: string; accessibleValue: string }[]
  ) => {
    console.log(this.instance);
    const func = this.getFunctionFromCache('setItems');
    const result = func(items);
    return result;
  };

  public getAllKeys = () => {
    const func = this.getFunctionFromCache('getAllKeys');
    const result = func();
    return result;
  };

  public getAllItems = () => {
    const func = this.getFunctionFromCache('getAllItems');
    const result = func();

    console.log(result);
    return result;
  };

  public removeItem = (key: string) => {
    const func = this.getFunctionFromCache('removeItem');
    const result = func(key);
    return result;
  };
}
