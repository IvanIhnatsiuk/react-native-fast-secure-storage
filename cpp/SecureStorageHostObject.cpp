#import "SecureStorageHostObject.h"
#include <iostream>
#include <thread>
#import "macros.h"

namespace securestorageHostObject {
using namespace facebook;
using namespace jsi;
using namespace std;

function<bool(const string , const string , const string )> _set;
function<const string(const string )> _get;
function<bool(const string )> _del;
function<void()> _clearStorage;
function<string()> _getAllKeys;
function<string()> _getAllItems;
function<bool(const string )> _hasItem;

jsi::Value generateJSError(jsi::Runtime &runtime, string errorMessage) {
  auto errorCtr = runtime.global().getPropertyAsFunction(runtime, "Error");
  return errorCtr.callAsConstructor(
      runtime, jsi::String::createFromUtf8(runtime, errorMessage));
}

struct KeyValue {
  string key;
  string value;
  string accessibleValue;
};

void install(
    jsi::Runtime &runtime,
    shared_ptr<react::CallInvoker> jsCallInvoker,
    function<bool(const string , const string , const string )> setItemFn,
    function<string(const string )> getItemFn,
    function<bool(const string )> delItemFn,
    function<void()> clearStorageFn,
    function<string()> getAllkeysFn,
    function<string()> getAllItemsFn,
    function<bool(const string )> hasItemFn)

{
  _set = setItemFn;
  _get = getItemFn;
  _del = delItemFn;
  _clearStorage = clearStorageFn;
  _getAllKeys = getAllkeysFn;
  _getAllItems = getAllItemsFn;
  _hasItem = hasItemFn;
  auto setItem = CREATE_HOST_FN("setItem", 3) {
    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "setItem: key must be a string value!");
    }

    if (!arguments[1].isString()) {
      throw jsi::JSError(runtime, "setItem: value must be a string value!");
    }

    const std::string key = arguments[0].getString(runtime).utf8(runtime);
    const std::string value = arguments[1].getString(runtime).utf8(runtime);
    const std::string accessible = arguments[2].getString(runtime).utf8(runtime);
    auto promise = runtime.global().getPropertyAsFunction(runtime, "Promise");
    return promise.callAsConstructor(
        runtime,
        Function::createFromHostFunction(
            runtime,
            PropNameID::forAscii(runtime, "executor"),
            2,
            [key, value, accessible, jsCallInvoker = jsCallInvoker](
                Runtime &runtime,
                const Value &thisValue,
                const Value *args,
                size_t) -> Value {
              auto resolve = make_shared<Value>(runtime, args[0]);
              auto reject = make_shared<Value>(runtime, args[1]);
              std::thread([key,
                           value,
                           accessible,
                           resolve = std::move(resolve),
                           reject = std::move(reject),
                           jsCallInvoker = jsCallInvoker,
                           &runtime]() {
                bool result =
                    _set(key, value, accessible);
                jsCallInvoker->invokeAsync(
                    [resolve, reject, result, &runtime]() {
                      if (result) {
                        resolve->asObject(runtime).asFunction(runtime).call(
                            runtime, result);
                      } else {
                        reject->asObject(runtime).asFunction(runtime).call(
                            runtime, result);
                      }
                    });
              }).detach();
              return Value();
            }));
  });

  auto setItems = CREATE_HOST_FN("setItems", 1) {
    if (!arguments[0].getObject(runtime).isArray(runtime)) {
      throw jsi::JSError(runtime, "setitems: items must be an array");
    }
    auto items = arguments[0].getObject(runtime).getArray(runtime);
    std::vector<KeyValue> itemsArray;
    size_t size = items.size(runtime);
    for (int i = 0; i < size; i++) {
      auto item = items.getValueAtIndex(runtime, i).getObject(runtime);
      auto key =
          item.getProperty(runtime, "key").asString(runtime).utf8(runtime);
      auto value =
          item.getProperty(runtime, "value").asString(runtime).utf8(runtime);
      auto accessibleValue = item.getProperty(runtime, "accessibleValue")
                                 .asString(runtime)
                                 .utf8(runtime);
      itemsArray.push_back({key, value, accessibleValue});
    }

    auto promise = runtime.global().getPropertyAsFunction(runtime, "Promise");
    return promise.callAsConstructor(
        runtime,
        Function::createFromHostFunction(
            runtime,
            PropNameID::forAscii(runtime, "executor"),
            2,
            [itemsArray, jsCallInvoker = jsCallInvoker](
                Runtime &runtime,
                const Value &thisValue,
                const Value *args,
                size_t) -> Value {
              auto resolve = make_shared<Value>(runtime, args[0]);
              auto reject = make_shared<Value>(runtime, args[1]);
              std::thread([itemsArray,
                           resolve = std::move(resolve),
                           reject = std::move(reject),
                           jsCallInvoker = jsCallInvoker,
                           &runtime]() {
                try {
                  for (const auto &item : itemsArray) {
                    const auto key = item.key;
                    const auto value = item.value;
                    const auto accessibleValue = item.accessibleValue;
                    _set(key, value, accessibleValue);
                  }
                  jsCallInvoker->invokeAsync([resolve, reject, &runtime]() {
                    resolve->asObject(runtime).asFunction(runtime).call(
                        runtime);
                  });
                } catch (const std::exception &exception) {
                  jsCallInvoker->invokeAsync([reject, &runtime, &exception]() {
                    auto error = generateJSError(runtime, exception.what());
                    reject->asObject(runtime).asFunction(runtime).call(
                        runtime, error);
                  });
                }
              }).detach();
              return Value();
            }));

    return Value();
  });

  auto getItem = CREATE_HOST_FN("getItem", 1) {
    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "getItem: key must be a string value!");
    }

    std::string key = arguments[0].getString(runtime).utf8(runtime);

    auto promise = runtime.global().getPropertyAsFunction(runtime, "Promise");
    return promise.callAsConstructor(
        runtime,
        Function::createFromHostFunction(
            runtime,
            PropNameID::forAscii(runtime, "executor"),
            4,
            [key, jsCallInvoker = jsCallInvoker](
                Runtime &runtime,
                const Value &thisValue,
                const Value *args,
                size_t) -> Value {
              auto resolve = make_shared<Value>(runtime, args[0]);
              auto reject = make_shared<Value>(runtime, args[1]);
              std::thread([key,
                           resolve = std::move(resolve),
                           reject = std::move(reject),
                           jsCallInvoker,
                           &runtime]() {
                try {
                  auto result = _get(key);
                  jsCallInvoker->invokeAsync(
                      [resolve, reject, result, &runtime]() {
                        if (!result.empty()) {
                          resolve->asObject(runtime).asFunction(runtime).call(
                              runtime, result);
                        } else {
                          auto error =
                              generateJSError(runtime, "Value does not exist");
                          reject->asObject(runtime).asFunction(runtime).call(
                              runtime, error);
                        }
                      });
                } catch (const std::exception &exception) {
                  jsCallInvoker->invokeAsync([reject, &runtime, &exception]() {
                    auto error = generateJSError(runtime, exception.what());
                    reject->asObject(runtime).asFunction(runtime).call(
                        runtime, error);
                  });
                }
              }).detach();
              return jsi::Value();
            }));
  });

  auto getAllKeys = CREATE_HOST_FN("getAllKeys", 1) {
    auto promise = runtime.global().getPropertyAsFunction(runtime, "Promise");
    return promise.callAsConstructor(
        runtime,
        Function::createFromHostFunction(
            runtime,
            PropNameID::forAscii(runtime, "executor"),
            1,
            [jsCallInvoker = jsCallInvoker](
                Runtime &runtime,
                const Value &thisValue,
                const Value *args,
                size_t) -> Value {
              auto resolve = make_shared<Value>(runtime, args[0]);
              auto reject = make_shared<Value>(runtime, args[1]);
              std::thread([resolve = std::move(resolve),
                           reject = std::move(reject),
                           jsCallInvoker,
                           &runtime]() {
                try {
                  auto result = _getAllKeys();
                  jsCallInvoker->invokeAsync(
                      [resolve, reject, result, &runtime]() {
                        if (!result.empty()) {
                          resolve->asObject(runtime).asFunction(runtime).call(
                              runtime, result);
                        } else {
                          auto error =
                              generateJSError(runtime, "Value does not exist");
                          reject->asObject(runtime).asFunction(runtime).call(
                              runtime, error);
                        }
                      });
                } catch (const std::exception &exception) {
                  jsCallInvoker->invokeAsync([reject, &runtime, &exception]() {
                    auto error = generateJSError(runtime, exception.what());
                    reject->asObject(runtime).asFunction(runtime).call(
                        runtime, error);
                  });
                }
              }).detach();
              return jsi::Value();
            }));
  });

  auto getAllItems = CREATE_HOST_FN("getAllItems", 0) {
    auto promise = runtime.global().getPropertyAsFunction(runtime, "Promise");
    return promise.callAsConstructor(
        runtime,
        Function::createFromHostFunction(
            runtime,
            PropNameID::forAscii(runtime, "executor"),
            1,
            [jsCallInvoker = jsCallInvoker](
                Runtime &runtime,
                const Value &thisValue,
                const Value *args,
                size_t) -> Value {
              auto resolve = make_shared<Value>(runtime, args[0]);
              auto reject = make_shared<Value>(runtime, args[1]);
              std::thread([resolve = std::move(resolve),
                           reject = std::move(reject),
                           jsCallInvoker,
                           &runtime]() {
                try {
                  auto result = _getAllItems();
                  jsCallInvoker->invokeAsync(
                      [resolve, reject, result, &runtime]() {
                        resolve->asObject(runtime).asFunction(runtime).call(
                            runtime, result);
                      });
                } catch (const std::exception &exception) {
                  jsCallInvoker->invokeAsync([reject, &runtime, &exception]() {
                    auto error = generateJSError(runtime, exception.what());
                    reject->asObject(runtime).asFunction(runtime).call(
                        runtime, error);
                  });
                }
              }).detach();
              return jsi::Value();
            }));
  });

  auto hasValue = CREATE_HOST_FN("hasValue", 1) {
    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "hasValue: key must be a string!");
    }
    auto key = arguments[0].getString(runtime).utf8(runtime);
    auto promise = runtime.global().getPropertyAsFunction(runtime, "Promise");
    return promise.callAsConstructor(
        runtime,
        Function::createFromHostFunction(
            runtime,
            PropNameID::forAscii(runtime, "executor"),
            2,
            [key, jsCallInvoker = jsCallInvoker](
                Runtime &runtime,
                const Value &thisValue,
                const Value *args,
                size_t) -> Value {
              auto resolve = make_shared<Value>(runtime, args[0]);
              auto reject = make_shared<Value>(runtime, args[1]);
              std::thread([key,
                           resolve = std::move(resolve),
                           reject = std::move(reject),
                           jsCallInvoker,
                           &runtime]() {
                try {
                  bool result = _hasItem(key);
                  jsCallInvoker->invokeAsync([resolve, result, &runtime]() {
                    resolve->asObject(runtime).asFunction(runtime).call(
                        runtime, result);
                  });
                } catch (const std::exception &exception) {
                  jsCallInvoker->invokeAsync([reject, &runtime, &exception]() {
                    reject->asObject(runtime).asFunction(runtime).call(
                        runtime, exception.what());
                  });
                }
              }).detach();
              return jsi::Value();
            }));

    return Value();
  });

  auto removeItem = CREATE_HOST_FN("removeItem", 1) {
    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "removeItem: key must be a string value");
    }
    std::string key = arguments[0].getString(runtime).utf8(runtime);
    auto promise = runtime.global().getPropertyAsFunction(runtime, "Promise");
    return promise.callAsConstructor(
        runtime,
        Function::createFromHostFunction(
            runtime,
            PropNameID::forAscii(runtime, "executor"),
            2,
            [key, jsCallInvoker = jsCallInvoker](
                Runtime &runtime,
                const Value &thisValue,
                const Value *args,
                size_t) -> Value {
              auto resolve = make_shared<Value>(runtime, args[0]);
              auto reject = make_shared<Value>(runtime, args[1]);
              std::thread([key,
                           resolve = std::move(resolve),
                           reject = std::move(reject),
                           jsCallInvoker,
                           &runtime]() {
                try {
                  bool result = _del(key);
                  jsCallInvoker->invokeAsync([resolve, result, &runtime]() {
                    resolve->asObject(runtime).asFunction(runtime).call(
                        runtime, result);
                  });
                } catch (const std::exception &exception) {
                  jsCallInvoker->invokeAsync([reject, &runtime, &exception]() {
                    reject->asObject(runtime).asFunction(runtime).call(
                        runtime, exception.what());
                  });
                }
              }).detach();
              return jsi::Value();
            }));

    return Value();
  });

  auto clearStorage = CREATE_HOST_FN("clearStorage", 0) {
    auto promise = runtime.global().getPropertyAsFunction(runtime, "Promise");
    return promise.callAsConstructor(
        runtime,
        Function::createFromHostFunction(
            runtime,
            PropNameID::forAscii(runtime, "executor"),
            1,
            [jsCallInvoker = jsCallInvoker](
                Runtime &runtime,
                const Value &thisValue,
                const Value *args,
                size_t) -> Value {
              auto resolve = make_shared<Value>(runtime, args[0]);
              auto reject = make_shared<Value>(runtime, args[1]);
              std::thread([resolve = std::move(resolve),
                           jsCallInvoker,
                           &runtime]() {
                _clearStorage();
                jsCallInvoker->invokeAsync([resolve, &runtime]() {
                  resolve->asObject(runtime).asFunction(runtime).call(runtime);
                });
              }).detach();
              return jsi::Value();
            }));

    return Value();
  });

  jsi::Object module = jsi::Object(runtime);

  module.setProperty(runtime, "setItem", setItem);
  module.setProperty(runtime, "getItem", getItem);
  module.setProperty(runtime, "hasValue", hasValue);
  module.setProperty(runtime, "removeItem", removeItem);
  module.setProperty(runtime, "clearStorage", clearStorage);
  module.setProperty(runtime, "setItems", setItems);
  module.setProperty(runtime, "getAllKeys", getAllKeys);
  module.setProperty(runtime, "getAllItems", getAllItems);

  runtime.global().setProperty(runtime, "__SecureStorage", std::move(module));
}
} // namespace securestorageHostObject
