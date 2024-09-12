package com.fastsecurestorage;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;

@ReactModule(name = FastSecureStorageModule.NAME)
public class FastSecureStorageModule extends ReactContextBaseJavaModule {
  public static final String NAME = "FastSecureStorage";

  private FastSecureStorageBridge bridge;
  private final ReactApplicationContext reactContext;

  public FastSecureStorageModule(ReactApplicationContext reactContext) {
    super(reactContext);
    this.reactContext = reactContext;
    bridge = new FastSecureStorageBridge(reactContext);
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      System.loadLibrary("react-native-fast-secure-storage");
      bridge.install(getReactApplicationContext());
      return true;
    } catch (Exception exception) {
      Log.e(NAME, "Failed to install JSI Bindings!", exception);
      return false;
    }
  }

  @NonNull
  @Override
  public String getName() {
    return NAME;
  }
}
