import { useCallback, useEffect, useState } from "react";
import { Button, ScrollView, StyleSheet, Text, TextInput } from "react-native";
import SecureStorage, { ACCESSIBLE } from "react-native-fast-secure-storage";

import { Rectangle } from "./Reactangle";

const testItems = new Array(100).fill(0).map((_, index) => {
  return {
    key: `key_${index}`,
    value: `test_key_${index}`,
    accessibleValue: ACCESSIBLE.WHEN_UNLOCKED,
  };
});

export default function App() {
  const [result, setResult] = useState<string | undefined>();
  const [hasItem, setHasItem] = useState(false);
  const [text, setText] = useState("");

  const getTestValue = useCallback(async () => {
    const startTime = new Date().getTime();

    try {
      const value = await SecureStorage.getItem("test");

      console.log(value);

      setResult(value);
    } catch (error) {
      if (error instanceof Error) {
        setResult(error.message);
      }
    }
    console.log("Time taken:", new Date().getTime() - startTime);
  }, []);

  const setTestValue = useCallback(async () => {
    const startTime = new Date().getTime();

    await SecureStorage.setItem("test", "test value");
    console.log("Time taken:", new Date().getTime() - startTime);
    const value = await SecureStorage.getItem("test");

    setResult(value);
  }, []);

  const setMultipleItems = useCallback(async () => {
    const startTime = new Date().getTime();

    await SecureStorage.setItems(testItems);
    console.log("Time taken:", new Date().getTime() - startTime);
  }, []);

  const getAllKeys = useCallback(async () => {
    console.log(await SecureStorage.getAllKeys());
  }, []);

  const getAllItems = useCallback(async () => {
    console.log(await SecureStorage.getAllItems());
  }, []);

  const deleteValue = useCallback(async () => {
    await SecureStorage.removeItem("test");
    getTestValue();
  }, [getTestValue]);

  const checkHasItem = useCallback(async () => {
    setHasItem(await SecureStorage.hasItem("test"));
  }, []);

  const setItemSync = useCallback(() => {
    SecureStorage.setItemSync("test", "test value");
    getTestValue();
  }, [getTestValue]);

  const removeItemSync = useCallback(() => {
    SecureStorage.removeItemSync("test");
    getTestValue();
  }, [getTestValue]);

  const getItemSync = useCallback(() => {
    const value = SecureStorage.getItemSync("test");

    setResult(value as string);
  }, []);

  useEffect(() => {
    getTestValue();
  }, [getTestValue]);

  return (
    <ScrollView contentContainerStyle={styles.container}>
      <Rectangle />
      <Text>{result}</Text>
      <Text>{hasItem ? "Item exists" : "Item does not exist"}</Text>
      <TextInput
        defaultValue={text}
        style={{ height: 50, width: "100%" }}
        onChangeText={setText}
      />
      <Button title="set value" onPress={setTestValue} />
      <Button title="set multiple items" onPress={setMultipleItems} />
      <Button title="get value" onPress={getTestValue} />
      <Button title="get all keys" onPress={getAllKeys} />
      <Button title="get all items" onPress={getAllItems} />
      <Button title="clear storage" onPress={SecureStorage.clearStorage} />
      <Button title="delete value" onPress={deleteValue} />
      <Button title="has item" onPress={checkHasItem} />
      <Button title="set item sync" onPress={setItemSync} />
      <Button title="remove item sync" onPress={removeItemSync} />
      <Button title="get item sync" onPress={getItemSync} />
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    flexGrow: 1,
    alignItems: "center",
    justifyContent: "center",
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
