package com.fastsecurestorage

import com.facebook.react.TurboReactPackage
import com.facebook.react.bridge.NativeModule
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.module.model.ReactModuleInfo
import com.facebook.react.module.model.ReactModuleInfoProvider
import com.fastsecurestorage.FastSecureStorageModule

class FastSecureStoragePackage : TurboReactPackage() {
  override fun getModule(
    name: String,
    reactContext: ReactApplicationContext,
  ): NativeModule? =
    if (name == FastSecureStorageImpl.NAME) {
      FastSecureStorageModule(reactContext)
    } else {
      null
    }

  override fun getReactModuleInfoProvider(): ReactModuleInfoProvider =
    ReactModuleInfoProvider {
      val moduleInfos: MutableMap<String, ReactModuleInfo> =
        HashMap()
      moduleInfos[FastSecureStorageImpl.NAME] =
        ReactModuleInfo(
          FastSecureStorageImpl.NAME,
          FastSecureStorageImpl.NAME,
          false, // canOverrideExistingModule
          false, // needsEagerInit
          true, // hasConstants
          false, // isCxxModule
          true, // isTurboModule
        )
      moduleInfos
    }
}
