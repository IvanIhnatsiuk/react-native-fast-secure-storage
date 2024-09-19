import { ACCESSIBLE } from './enums';
import type {
  ISecureStorage,
  SecureStorageItem,
  StoredSecureStorageItem,
  ISecureStorageNativeInstance,
} from './types';

declare global {
  var __SecureStorage: ISecureStorageNativeInstance | undefined;
}

class SecureStorage {
  private functionCache: Partial<ISecureStorageNativeInstance> = {};

  private nativeInstance = globalThis.__SecureStorage;

  private getFunctionFromCache<T extends keyof ISecureStorage>(
    functionName: T
  ): ISecureStorageNativeInstance[T] {
    if (!this.functionCache[functionName]) {
      this.functionCache[functionName] = this.nativeInstance?.[functionName];
    }

    return this.functionCache[functionName] as ISecureStorageNativeInstance[T];
  }

  public setItem = async (
    key: string,
    value: string,
    accessible = ACCESSIBLE.WHEN_UNLOCKED
  ): Promise<boolean> => {
    const func = this.getFunctionFromCache('setItem');
    const result = await func(key, value, accessible);

    return result;
  };

  public getItem = async (key: string): Promise<string> => {
    const func = this.getFunctionFromCache('getItem');
    const result = await func(key);

    return result;
  };

  public clearStorage = async (): Promise<void> => {
    const func = this.getFunctionFromCache('clearStorage');
    await func();
  };

  public setItems = async (items: SecureStorageItem[]): Promise<boolean> => {
    const func = this.getFunctionFromCache('setItems');
    const result = func(items);

    return result;
  };

  public getAllKeys = async (): Promise<StoredSecureStorageItem> => {
    const func = this.getFunctionFromCache('getAllKeys');
    const result = await func();

    return JSON.parse(result);
  };

  public getAllItems = async (): Promise<StoredSecureStorageItem> => {
    const func = this.getFunctionFromCache('getAllItems');
    const result = await func();

    return JSON.parse(result);
  };

  public removeItem = async (key: string): Promise<boolean> => {
    const func = this.getFunctionFromCache('removeItem');
    const result = await func(key);

    return result;
  };
}

export default SecureStorage;
