import { useEffect, useState } from 'react';
import { StyleSheet, View, Text, Button } from 'react-native';
import { SecureStorage } from 'react-native-fast-secure-storage';
import { MMKV } from 'react-native-mmkv';

export const storage = new MMKV();
const secureStorage = new SecureStorage();

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
    secureStorage.setItem('test', 'test value');
    setResult(secureStorage?.getItem('test'));
    const started = new Date().getTime();
    console.log(new Date().getTime() - started);
  };

  useEffect(() => {
    setResult(secureStorage?.getItem('test'));
  }, []);

  return (
    <View style={styles.container}>
      <Text>{result}</Text>
      <Button title="set value" onPress={setTestValue} />
      <Button
        title="set multiple items"
        onPress={() => secureStorage.setItems(testItems)}
      />
      <Button
        title="get value"
        onPress={() => {
          setResult(secureStorage.getItem('test'));
        }}
      />
      <Button
        title="get all keys"
        onPress={() => console.log(secureStorage.getAllKeys())}
      />
      <Button
        title="get all items"
        onPress={() => console.log(secureStorage.getAllItems())}
      />
      <Button
        title="clear storage"
        onPress={() => secureStorage.clearStorage()}
      />
      <Button
        title="delete value"
        onPress={() => {
          secureStorage.removeItem('test');
          setResult(secureStorage.getItem('test'));
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
