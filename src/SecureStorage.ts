import { ACCESSIBLE } from "./enums";

import type {
  ISecureStorage,
  ISecureStorageNativeInstance,
  SecureStorageItem,
  StoredSecureStorageItem,
} from "./types";

class SecureStorage implements ISecureStorage {
  private functionCache: Partial<ISecureStorageNativeInstance> = {};

  private nativeInstance = globalThis.__SecureStorage;

  private getFunctionFromCache<T extends keyof ISecureStorage>(
    functionName: T,
  ): ISecureStorageNativeInstance[T] {
    if (!this.functionCache[functionName]) {
      this.functionCache[functionName] = this.nativeInstance?.[functionName];
    }

    return this.functionCache[functionName] as ISecureStorageNativeInstance[T];
  }

  /**
   * Stores an item in the secure storage.
   * @param key - The key of the item.
   * @param value - The value of the item.
   * @param accessible - (Optional) The accessibility level of the item. Default is ACCESSIBLE.WHEN_UNLOCKED.
   * @returns A promise that resolves to a boolean indicating the success of the operation.
   */
  public setItem = async (
    key: string,
    value: string,
    accessible = ACCESSIBLE.WHEN_UNLOCKED,
  ): Promise<boolean> => {
    const func = this.getFunctionFromCache("setItem");
    const result = await func(key, value, accessible);

    return result;
  };

  /**
   * Retrieves an item from the secure storage.
   * @param key - The key of the item to retrieve.
   * @returns A promise that resolves to the value of the item.
   */
  public getItem = async (key: string): Promise<string> => {
    const func = this.getFunctionFromCache("getItem");
    const result = await func(key);

    return result;
  };

  /**
   * Clears all items from the secure storage.
   * @returns A promise that resolves when the operation is complete.
   */
  public clearStorage = async (): Promise<void> => {
    const func = this.getFunctionFromCache("clearStorage");

    await func();
  };

  /**
   * Stores multiple items in the secure storage.
   * @param items - An array of items to set.
   * @returns A promise that resolves to a boolean indicating the success of the operation.
   */
  public setItems = async (items: SecureStorageItem[]): Promise<boolean> => {
    const func = this.getFunctionFromCache("setItems");
    const result = func(items);

    return result;
  };

  /**
   * Retrieves all keys from the secure storage.
   * @returns A promise that resolves to an array of all keys.
   */
  public getAllKeys = async (): Promise<StoredSecureStorageItem> => {
    const func = this.getFunctionFromCache("getAllKeys");
    const result = await func();

    return JSON.parse(result);
  };

  /**
   * Retrieves all items from the secure storage.
   * @returns A promise that resolves to an array of all items.
   */
  public getAllItems = async (): Promise<StoredSecureStorageItem> => {
    const func = this.getFunctionFromCache("getAllItems");
    const result = await func();

    return JSON.parse(result);
  };

  /**
   * Removes an item from the secure storage.
   * @param key - The key of the item to remove.
   * @returns A promise that resolves to a boolean indicating the success of the operation.
   */
  public removeItem = async (key: string): Promise<boolean> => {
    const func = this.getFunctionFromCache("removeItem");
    const result = await func(key);

    return result;
  };

  /**
   * Checks if an item exists in the secure storage.
   * @param key - The key of the item to check.
   * @returns A promise that resolves to a boolean indicating whether the item exists.
   */
  public hasItem = async (key: string): Promise<boolean> => {
    const func = this.getFunctionFromCache("hasItem");
    const result = await func(key);

    return result;
  };

  public setItemSync = (
    key: string,
    value: string,
    accessible = ACCESSIBLE.WHEN_UNLOCKED,
  ): boolean => {
    const func = this.getFunctionFromCache("setItemSync");

    return func(key, value, accessible);
  };

  public getItemSync = (key: string): string | null => {
    const func = this.getFunctionFromCache("getItemSync");

    return func(key);
  };

  public removeItemSync = (key: string): boolean => {
    const func = this.getFunctionFromCache("removeItemSync");

    return func(key);
  };

  public hasItemSync = (key: string): boolean => {
    const func = this.getFunctionFromCache("hasItemSync");

    return func(key);
  };
}

export default SecureStorage;
