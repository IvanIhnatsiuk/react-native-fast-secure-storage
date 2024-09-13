type SecureStorageItem = {
  key: string;
  value: string;
};

export interface ISecureStorage {
  setItem(key: string, value: string, accessible?: string): boolean;
  getItem(key: string): string;
  clearStorage(): boolean;
  setItems(items: SecureStorageItem[]): boolean;
  getAllKeys(): string[];
  getAllItems(): SecureStorageItem[];
  removeItem(key: string): boolean;
}
