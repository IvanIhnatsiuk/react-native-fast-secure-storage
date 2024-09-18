import type { ACCESSIBLE } from './enums';

export type SecureStorageItem = {
  key: string;
  value: string;
  accessibleValue: ACCESSIBLE;
};

export interface ISecureStorage {
  setItem(key: string, value: string, accessible?: ACCESSIBLE): boolean;
  getItem(key: string): string;
  clearStorage(): boolean;
  setItems(items: SecureStorageItem[]): boolean;
  getAllKeys(): Promise<string>;
  getAllItems(): Promise<string>;
  removeItem(key: string): boolean;
}
