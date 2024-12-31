package com.fastsecurestorage

import android.util.Log
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.module.annotations.ReactModule

@ReactModule(name = FastSecureStorageImpl.NAME)
class FastSecureStorageModule(
  reactContext: ReactApplicationContext,
) : ReactContextBaseJavaModule(
    reactContext,
  ) {
  private val fastSecureStorage = FastSecureStorageImpl(reactContext)

  @ReactMethod(isBlockingSynchronousMethod = true)
  fun install(): Boolean {
    try {
      System.loadLibrary("react-native-fast-secure-storage")
      fastSecureStorage.install(reactApplicationContext)
      return true
    } catch (exception: Exception) {
      Log.e(FastSecureStorageImpl.NAME, "Failed to install JSI Bindings!", exception)
      return false
    }
  }

  override fun getName(): String = FastSecureStorageImpl.NAME
}
