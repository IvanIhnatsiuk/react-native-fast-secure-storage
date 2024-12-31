package com.fastsecurestorage

import android.util.Log
import com.facebook.react.bridge.ReactApplicationContext

class FastSecureStorageModule(
  reactContext: ReactApplicationContext,
) : NativeFastSecureStorageSpec(reactContext) {
  private val fastSecureStorage = FastSecureStorageImpl(reactContext)

  override fun getName(): String = FastSecureStorageImpl.NAME

  override fun install(): Boolean {
    try {
      System.loadLibrary("react-native-fast-secure-storage")
      fastSecureStorage.install(reactApplicationContext)
      return true
    } catch (exception: Exception) {
      Log.e(FastSecureStorageImpl.NAME, "Failed to install JSI Bindings!", exception)
      return false
    }
  }
}
