import { NativeModules, Platform } from "react-native";

import { ACCESSIBLE } from "./enums";
import SecureStorage from "./SecureStorage";

const LINKING_ERROR =
  `The package 'react-native-fast-secure-storage' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: "" }) +
  "- You rebuilt the app after installing the package\n" +
  "- You are not using Expo Go\n";

// @ts-expect-error turboModuleProxy is not yet in the types
const isTurboModuleEnabled = global.__turboModuleProxy !== null;

const FastSecureStorageModule = isTurboModuleEnabled
  ? // eslint-disable-next-line @typescript-eslint/no-var-requires, @typescript-eslint/no-require-imports
    require("./spec/NativeFastSecureStorage").default
  : NativeModules.FastSecureStorage;

const FastSecureStorage = FastSecureStorageModule
  ? FastSecureStorageModule
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      },
    );

FastSecureStorage.install();

export { ACCESSIBLE };

export default new SecureStorage();
