import type { ISecureStorage } from './types';

declare global {
  var __SecureStorage: ISecureStorage | undefined;
}

class SecureStorage implements ISecureStorage {
  private functionCache: Partial<ISecureStorage> = {};

  private nativeInstance?: ISecureStorage = global.__SecureStorage;

  constructor() {
    this.functionCache = {};
  }

  private getFunctionFromCache<T extends keyof ISecureStorage>(
    functionName: T
  ): ISecureStorage[T] {
    if (!this.functionCache[functionName]) {
      this.functionCache[functionName] = this.nativeInstance?.[functionName];
    }

    return this.functionCache[functionName] as ISecureStorage[T];
  }

  public setItem(
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

    return func();
  };

  public removeItem = (key: string) => {
    const func = this.getFunctionFromCache('removeItem');
    const result = func(key);
    return result;
  };
}

export default SecureStorage;
