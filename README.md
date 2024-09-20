# react-native-fast-secure-storage

`react-native-fast-secure-storage` is a fast, secure, and extensible key-value storage solution for React Native applications. 
This library provides a simple API to securely store sensitive data, leveraging the best practices for data encryption and performance.

## Features

- Fast and efficient key-value storage
- Secure data encryption
- Simple asynchronous API
- Cross-platform support (iOS and Android)
- Supports both strings and binary data

## Installation

```sh
npm install react-native-fast-secure-storage
```
or
```sh
yarn add react-native-fast-secure-storage
```

### API

- [setItem](#setitem)
- [getItem](#getitem)
- [clearStorage](#clearstorage)
- [setItems](#setitems)
- [getAllKeys](#getallkeys)
- [getAllItems](#getallitems)
- [removeItem](#removeitem)
- [clearStorage](#clearStorage)
- [hasItem](#hasitem)

## Methods

### setItem

Stores an item in the secure storage.

- key: The key of the item.
- value: The value of the item.
- accessible: (Optional) The accessibility level of the item. Default is ACCESSIBLE.WHEN_UNLOCKED.

```ts
import SecureStorage, { ACCESSIBLE } from "react-native-fast-secure-storage";

const result = await SecureStorage.setItem("key1", "value1", ACCESSIBLE.WHEN_UNLOCKED)
```

### getItem
Retrieves an item from the secure storage.

- key: The key of the item to retrieve.

```ts
import SecureStorage from "react-native-fast-secure-storage";

const result = await SecureStorage.getItem("key1")

console.log(result); // output: value1
```

This method throws an error if there is no stored value for this key or the value is empty.

### clearStorage

Clears all items from the secure storage.


```ts
import SecureStorage from "react-native-fast-secure-storage";

await SecureStorage.clearStorage()
```

### setItems

Stores multiple items in the secure storage.

- items: An array of items to set.

```ts
import SecureStorage, { ACCESSIBLE } from "react-native-fast-secure-storage";
const testItems = [
    {
        key: "key1",
        value: "value1",
        accessibleValue: ACCESSIBLE.WHEN_UNLOCKED
    },
    {
        key: "key2",
        value: "value2",
        accessibleValue: ACCESSIBLE.WHEN_UNLOCKED
    },
];

await SecureStorage.setItems(testItems);
```

### getAllKeys

Retrieves all keys from the secure storage.

```ts
import SecureStorage, { ACCESSIBLE } from "react-native-fast-secure-storage";
const testItems = [
    {
        key: "key1",
        value: "value1",
        accessibleValue: ACCESSIBLE.WHEN_UNLOCKED
    },
    {
        key: "key2",
        value: "value2",
        accessibleValue: ACCESSIBLE.WHEN_UNLOCKED
    },
];

await SecureStorage.setItems(testItems);

const keys = SecureStorage.getAllKeys();

console.log(keys); // output: ["ke1", "key2"]
```

### getAllItems

Retrieves all items from the secure storage.

```ts
import SecureStorage, { ACCESSIBLE } from "react-native-fast-secure-storage";
const testItems = [
    {
        key: "key1",
        value: "value1",
        accessibleValue: ACCESSIBLE.WHEN_UNLOCKED
    },
    {
        key: "key2",
        value: "value2",
        accessibleValue: ACCESSIBLE.WHEN_UNLOCKED
    },
];

await SecureStorage.setItems(testItems);

const secureStorageItems = SecureStorage.getAllKeys();

console.log(secureStorageItems); // output: [{ key: "key1", value: "value1" }, {key: "key2", value: "value2" }]
```

### removeItem

Removes an item from the secure storage.

- key: The key of the item to remove.

```ts
import SecureStorage from "react-native-fast-secure-storage";

await SecureStorage.setItem("key1", "value1");

await SecureStorage.deleteItem("key1");

try { 
  await SecureStorage.getItem("key1")
} cath(e) {
  console.log(e) // output: value does not exist
}

```

### hasItem

- key: The key of the item to check.

```ts
import SecureStorage from "react-native-fast-secure-storage";

await SecureStorage.setItem("key1", "value1");

const result = await SecureStorage.hasItem("key1");

console.log(result) // output: true

```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
# react-native-fast-secure-storage
