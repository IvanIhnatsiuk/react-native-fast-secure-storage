package com.fastsecurestorage

import android.util.Log
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.module.annotations.ReactModule

@ReactModule(name = FastSecureStorageModule.NAME)
class FastSecureStorageModule(private val reactContext: ReactApplicationContext) :
  ReactContextBaseJavaModule(
    reactContext
  ) {
  private val bridge = FastSecureStorageBridge(reactContext)

  @ReactMethod(isBlockingSynchronousMethod = true)
  fun install(): Boolean {
    try {
      System.loadLibrary("react-native-fast-secure-storage")
      bridge.install(reactApplicationContext)
      return true
    } catch (exception: Exception) {
      Log.e(NAME, "Failed to install JSI Bindings!", exception)
      return false
    }
  }

  override fun getName(): String {
    return NAME
  }

  companion object {
    const val NAME: String = "FastSecureStorage"
  }
}
