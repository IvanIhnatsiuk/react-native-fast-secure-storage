type SecureStorageItem = {
  key: string;
  value: string;
};

export interface ISecureStorage {
  setItem(key: string, value: string, accessible?: string): boolean;
  getItem(key: string): string;
  clearStorage(): boolean;
  setItems(items: SecureStorageItem[]): boolean;
  getAllKeys(): Promise<string>;
  getAllItems(): Promise<string>;
  removeItem(key: string): boolean;
}
