import { useEffect, useState } from 'react';
import { StyleSheet, View, Text, Button } from 'react-native';
import SecureStorage from 'react-native-fast-secure-storage';
import { MMKV } from 'react-native-mmkv';

export const storage = new MMKV();

const testItems = new Array(100).fill(0).map((_, index) => {
  return {
    key: `key_${index}`,
    value: `test_key_${index}`,
    accessibleValue: 'AccessibleWhenUnlocked',
  };
});

export default function App() {
  const [result, setResult] = useState<string | undefined>();

  const setTestValue = async () => {
    SecureStorage.setItem('test', 'test value');
    setResult(SecureStorage.getItem('test'));
    const started = new Date().getTime();
    console.log(new Date().getTime() - started);
  };

  useEffect(() => {
    setResult(SecureStorage.getItem('test'));
  }, []);

  return (
    <View style={styles.container}>
      <Text>{result}</Text>
      <Button title="set value" onPress={setTestValue} />
      <Button
        title="set multiple items"
        onPress={() => SecureStorage.setItems(testItems)}
      />
      <Button
        title="get value"
        onPress={() => {
          setResult(SecureStorage.getItem('test'));
        }}
      />
      <Button
        title="get all keys"
        onPress={() => console.log(SecureStorage.getAllKeys())}
      />
      <Button
        title="get all items"
        onPress={() => console.log(SecureStorage.getAllItems())}
      />
      <Button
        title="clear storage"
        onPress={() => SecureStorage.clearStorage()}
      />
      <Button
        title="delete value"
        onPress={() => {
          SecureStorage.removeItem('test');
          setResult(SecureStorage.getItem('test'));
        }}
      />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
