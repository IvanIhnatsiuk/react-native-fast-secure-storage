import type { ACCESSIBLE } from './enums';

declare global {
  var __SecureStorage: ISecureStorageNativeInstance | undefined;
}

export type StoredSecureStorageItem = {
  key: string;
  value: string;
};

export type SecureStorageItem = StoredSecureStorageItem & {
  accessibleValue: ACCESSIBLE;
};

export interface ISecureStorage {
  setItem(
    key: string,
    value: string,
    accessible?: ACCESSIBLE
  ): Promise<boolean>;
  getItem(key: string): Promise<string>;
  clearStorage(): Promise<void>;
  setItems(items: SecureStorageItem[]): Promise<boolean>;
  getAllKeys(): Promise<StoredSecureStorageItem>;
  getAllItems(): Promise<StoredSecureStorageItem>;
  removeItem(key: string): Promise<boolean>;
  hasItem(key: string): Promise<boolean>;
}

export interface ISecureStorageNativeInstance
  extends Omit<ISecureStorage, 'getAllItems' | 'getAllKeys'> {
  getAllKeys(): Promise<string>;
  getAllItems(): Promise<string>;
}
