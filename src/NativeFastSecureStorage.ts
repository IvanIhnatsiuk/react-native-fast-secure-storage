import { TurboModuleRegistry } from "react-native";

import type { TurboModule } from "react-native";

export interface Spec extends TurboModule {
  install(): boolean;
}

export default TurboModuleRegistry.getEnforcing<Spec>("FastSecureStorage");
