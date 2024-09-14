#include "bindings.h"
#include <jni.h>
#include <iostream>
#include <utility>
#include "macros.h"

using namespace facebook::jsi;
using namespace std;

namespace securestorage {
function<bool(const char *, const char *)> _set;
function<std::string(const char *)> _get;
function<bool(const char *)> _del;
function<bool()> _clear;
function<string()> _getAllItems;
function<string()> _getAllKeys;

void install(
    jsi::Runtime &runtime,
    function<bool(const char *, const char *)> setItemFn,
    function<std::string(const char *)> getItemFn,
    function<bool(const char *)> delItemFn,
    function<bool()> clearStorageFn,
    function<string()> getAllItemsFn,
    function<string()> getAllKeysFn) {
  _set = setItemFn;
  _get = getItemFn;
  _del = delItemFn;
  _clear = clearStorageFn;
  _getAllItems = getAllItemsFn;
  _getAllKeys = getAllKeysFn;

  auto setItem = CREATE_HOST_FN("setItem", 1) {
    string key = arguments[0].getString(runtime).utf8(runtime);
    string value = arguments[1].getString(runtime).utf8(runtime);

    try {
      _set(key.c_str(), value.c_str());
      return {true};
    } catch (std::exception &e) {
      auto errorStr = jsi::String::createFromUtf8(
          runtime,
          "secure storage could not set value, error code: " +
              std::string(e.what()));
      return {false};
    }

    return {true};
  });

  auto getItem = CREATE_HOST_FN("getItem", 1) {
    string key = arguments[0].getString(runtime).utf8(runtime);

    try {
      std::string val = _get(key.c_str());
      if (val.empty()) {
        return {};
      }

      jsi::String result = jsi::String::createFromUtf8(runtime, val);
      return jsi::Value(runtime, result);
    } catch (std::exception &e) {
      auto errorStr = jsi::String::createFromUtf8(
          runtime,
          "secure storage could not set value, error code: " +
              std::string(e.what()));
      return {};
    }

    return {};
  });

  auto getAllItems = CREATE_HOST_FN("getAllItems", 0) {
    try {
      std::string val = _getAllItems();

      if (val.empty()) {
        return {};
      }

      jsi::String result = jsi::String::createFromUtf8(runtime, val);

      return jsi::Value(runtime, result);
    } catch (exception &e) {
      return {};
    }
  });

  auto removeItem = CREATE_HOST_FN("removeItem", 1) {
    string key = arguments[0].asString(runtime).utf8(runtime);
    try {
      _del(key.c_str());
      return jsi::Value(true);
    } catch (exception &e) {
      return jsi::Value(false);
    }

    return jsi::Value(false);
  });

  auto clearStorage = CREATE_HOST_FN("clearStorage", 0) {
    try {
      _clear();

      return jsi::Value(true);
    } catch (exception &e) {
      return jsi::Value(false);
    }
  });

  auto getAllKeys = CREATE_HOST_FN("getAllKeys", 0) {
    try {
      auto keys = _getAllKeys();

      return jsi::Value(runtime, jsi::String::createFromUtf8(runtime, keys));
    } catch (exception &e) {
      return jsi::Value(runtime, jsi::String::createFromUtf8(runtime, "[]"));
    }

    return jsi::Value(runtime, jsi::String::createFromUtf8(runtime, "[]"));
  });

  auto setItems = CREATE_HOST_FN("setItems", 1) {
    try {
      auto items =
          arguments[0].getObject(runtime).asArray(runtime).asArray(runtime);
      for (int i = 0; i < items.size(runtime); i++) {
        auto item = items.getValueAtIndex(runtime, i).getObject(runtime);
        auto key =
            item.getProperty(runtime, "key").asString(runtime).utf8(runtime);
        auto value =
            item.getProperty(runtime, "value").asString(runtime).utf8(runtime);
        _set(key.c_str(), value.c_str());
        return jsi::Value(true);
      }
    } catch (exception &e) {
      return jsi::Value(false);
    }

    return jsi::Value(false);
  });

  auto module = jsi::Object(runtime);

  module.setProperty(runtime, "setItem", std::move(setItem));
  module.setProperty(runtime, "getItem", std::move(getItem));
  module.setProperty(runtime, "removeItem", std::move(removeItem));
  module.setProperty(runtime, "clearStorage", std::move(clearStorage));
  module.setProperty(runtime, "getAllKeys", std::move(getAllKeys));
  module.setProperty(runtime, "getAllItems", std::move(getAllItems));
  module.setProperty(runtime, "setItems", std::move(setItems));

  runtime.global().setProperty(runtime, "__SecureStorage", std::move(module));
}
} // namespace securestorage
