package com.fastsecurestorage

import android.content.SharedPreferences
import androidx.security.crypto.EncryptedSharedPreferences
import androidx.security.crypto.MasterKey
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContext
import com.facebook.react.common.annotations.FrameworkAPI
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl
import org.json.JSONArray
import org.json.JSONObject
import java.io.FileNotFoundException
import java.io.IOException
import java.security.GeneralSecurityException

@OptIn(FrameworkAPI::class)
class FastSecureStorageBridge(
  private val context: ReactApplicationContext,
) {
  private var sharedPreferences: SharedPreferences? = null

  private external fun installNativeJsi(
    jsContextNativePointer: Long,
    jsCallInvokerHolder: CallInvokerHolderImpl,
  )

  init {
    try {
      this.sharedPreferences = this.secureSharedPreferences
    } catch (e: Exception) {
      e.printStackTrace()
    }
  }

  fun setItem(
    alias: String,
    value: String,
  ): Boolean {
    try {
      sharedPreferences?.edit()?.putString(alias, value)?.apply()
      return true
    } catch (e: Exception) {
      return false
    }
  }

  fun getAllKeys(): String {
    val keys = this.sharedPreferences?.all?.keys ?: return JSONArray().toString()

    return JSONArray(keys).toString()
  }

  fun getAllItems(): String {
    val entries = this.sharedPreferences?.all?.entries ?: return JSONArray().toString()

    val keyValueList =
      entries.map { element ->
        val jsonObject = JSONObject()
        jsonObject.put("key", element.key)
        jsonObject.put("value", element.value)
      }

    return JSONArray(keyValueList).toString()
  }

  fun hasItem(key: String): Boolean = this.sharedPreferences?.contains(key) ?: false

  @get:Throws(GeneralSecurityException::class, IOException::class)
  private val secureSharedPreferences: SharedPreferences
    get() {
      val key =
        MasterKey
          .Builder(context)
          .setKeyScheme(MasterKey.KeyScheme.AES256_GCM)
          .build()

      return EncryptedSharedPreferences.create(
        context,
        "secret_shared_prefs",
        key,
        EncryptedSharedPreferences.PrefKeyEncryptionScheme.AES256_SIV,
        EncryptedSharedPreferences.PrefValueEncryptionScheme.AES256_GCM,
      )
    }

  fun getItem(key: String): String {
    try {
      val value =
        sharedPreferences!!.getString(key, null)
      if (value == null) {
        throw FileNotFoundException("$key has not been set")
      } else {
        return value
      }
    } catch (e: Exception) {
      e.printStackTrace()
      return ""
    }
  }

  fun removeItem(key: String): Boolean {
    try {
      if (!hasItem(key)) {
        return true
      }
      sharedPreferences?.edit()?.remove(key)?.apply()
      return true
    } catch (e: Exception) {
      return false
    }
  }

  fun clearStorage() {
    try {
      if (this.sharedPreferences
          ?.all
          ?.keys
          ?.size == 0
      ) {
        return
      }

      this.sharedPreferences
        ?.edit()
        ?.clear()
        ?.apply()
    } catch (e: Exception) {
      e.printStackTrace()
    }
  }

  fun install(context: ReactContext) {
    val jsContextPointer = context.javaScriptContextHolder!!.get()
    val jsCallInvokerHolder =
      context.catalystInstance.jsCallInvokerHolder
    installNativeJsi(jsContextPointer, jsCallInvokerHolder as CallInvokerHolderImpl)
  }
}
