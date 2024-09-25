import { useEffect, useState } from "react";
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

  const getTestValue = async () => {
    const startTime = new Date().getTime();

    try {
      const value = await SecureStorage.getItem("test");

      setResult(value);
    } catch (error) {
      if (error instanceof Error) {
        setResult(error.message);
      }
    }
    console.log("Time taken:", new Date().getTime() - startTime);
  };

  const setTestValue = async () => {
    const startTime = new Date().getTime();

    await SecureStorage.setItem("test", "test value");
    console.log("Time taken:", new Date().getTime() - startTime);
    const value = await SecureStorage.getItem("test");

    setResult(value);
  };

  useEffect(() => {
    getTestValue();
  }, []);

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
      <Button
        title="set multiple items"
        onPress={async () => {
          const startTime = new Date().getTime();

          await SecureStorage.setItems(testItems);
          console.log("Time taken:", new Date().getTime() - startTime);
        }}
      />
      <Button title="get value" onPress={getTestValue} />
      <Button
        title="get all keys"
        onPress={async () => console.log(await SecureStorage.getAllKeys())}
      />
      <Button
        title="get all items"
        onPress={async () => console.log(await SecureStorage.getAllItems())}
      />
      <Button
        title="clear storage"
        onPress={() => SecureStorage.clearStorage()}
      />
      <Button
        title="delete value"
        onPress={async () => {
          await SecureStorage.removeItem("test");
          getTestValue();
        }}
      />
      <Button
        title="has item"
        onPress={async () => {
          setHasItem(await SecureStorage.hasItem("test"));
        }}
      />
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
