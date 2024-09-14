#import "SecureStorage.h"
#import <React/RCTBridgeModule.h>
#import <UIKit/UIKit.h>
#import "JSIUtils.h"
#import "macros.h"

namespace secureStorage {
using namespace facebook;
using namespace jsi;
using namespace std;

CFStringRef _accessibleValue(NSString *accessible)
{
  NSDictionary *list = @{
    @"AccessibleWhenUnlocked" : (__bridge id)kSecAttrAccessibleWhenUnlocked,
    @"AccessibleAfterFirstUnlock" : (__bridge id)kSecAttrAccessibleAfterFirstUnlock,
    @"AccessibleAlways" : (__bridge id)kSecAttrAccessibleAlways,
    @"AccessibleWhenPasscodeSetThisDeviceOnly" :
        (__bridge id)kSecAttrAccessibleWhenPasscodeSetThisDeviceOnly,
    @"AccessibleWhenUnlockedThisDeviceOnly" :
        (__bridge id)kSecAttrAccessibleWhenUnlockedThisDeviceOnly,
    @"AccessibleAfterFirstUnlockThisDeviceOnly" :
        (__bridge id)kSecAttrAccessibleAfterFirstUnlockThisDeviceOnly,
    @"AccessibleAlwaysThisDeviceOnly" : (__bridge id)kSecAttrAccessibleAlwaysThisDeviceOnly
  };

  return (__bridge CFStringRef)list[accessible];
}

NSString *serviceName = nil;

NSString *getServiceName()
{
  if (serviceName == nil) {
    serviceName = [[NSBundle mainBundle] bundleIdentifier];
  }

  return serviceName;
}

NSMutableDictionary *generateBaseQueryDictionary(NSString *key)
{
  NSMutableDictionary *baseQueryDictionary = [NSMutableDictionary new];
  [baseQueryDictionary setObject:(id)kSecClassGenericPassword forKey:(id)kSecClass];
  NSData *encodedIdentifier = [key dataUsingEncoding:NSUTF8StringEncoding];
  [baseQueryDictionary setObject:encodedIdentifier forKey:(id)kSecAttrGeneric];
  [baseQueryDictionary setObject:encodedIdentifier forKey:(id)kSecAttrAccount];
  [baseQueryDictionary setObject:getServiceName() forKey:(id)kSecAttrService];

  return baseQueryDictionary;
}

void clearSecureStorage()
{
  NSArray *secItemClasses = @[
    (__bridge id)kSecClassGenericPassword,
    (__bridge id)kSecAttrGeneric,
    (__bridge id)kSecAttrAccount,
    (__bridge id)kSecClassKey,
    (__bridge id)kSecAttrService
  ];
  for (id secItemClass in secItemClasses) {
    NSDictionary *spec = @{(__bridge id)kSecClass : secItemClass};
    SecItemDelete((__bridge CFDictionaryRef)spec);
  }
}

void handleAppUninstall()
{
  if (![[NSUserDefaults standardUserDefaults] boolForKey:@"hasRunBefore"]) {
    clearSecureStorage();
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"hasRunBefore"];
    [[NSUserDefaults standardUserDefaults] synchronize];
  }
}

void setServiceName(NSString *_serviceName)
{
  serviceName = _serviceName;
}

bool setSecureStorageItem(NSString *key, NSString *value, CFStringRef accessibilityLevel)
{
  NSMutableDictionary *dictionary = generateBaseQueryDictionary(key);

  NSData *valueData = [value dataUsingEncoding:NSUTF8StringEncoding];
  [dictionary setObject:valueData forKey:(id)kSecValueData];
  dictionary[(__bridge NSString *)kSecAttrAccessible] = (__bridge id)accessibilityLevel;

  OSStatus status = SecItemAdd((CFDictionaryRef)dictionary, NULL);

  if (status == errSecSuccess) {
    return true;
  } else {
    NSMutableDictionary *updateDictionary = [[NSMutableDictionary alloc] init];
    [updateDictionary setObject:valueData forKey:(id)kSecValueData];
    updateDictionary[(__bridge NSString *)kSecAttrAccessible] = (__bridge id)accessibilityLevel;
    NSMutableDictionary *searchDictionary = generateBaseQueryDictionary(key);
    OSStatus status =
        SecItemUpdate((CFDictionaryRef)searchDictionary, (CFDictionaryRef)updateDictionary);

    return status == errSecSuccess;
  }
}

void install(jsi::Runtime &runtime, std::shared_ptr<react::CallInvoker> jsCallInvoker)
{
  auto setItem = CREATE_HOST_FN("setItem", 3)
  {
    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "setItem: key must be a string value!");
    }

    if (!arguments[1].isString()) {
      throw jsi::JSError(runtime, "setItem: value must be a string value!");
    }

    NSString *key = convertJSIStringToNSString(runtime, arguments[0].getString(runtime));
    NSString *value = convertJSIStringToNSString(runtime, arguments[1].getString(runtime));
    NSString *accessible = convertJSIStringToNSString(runtime, arguments[2].getString(runtime));
    CFStringRef accessibleValue = _accessibleValue(accessible);

    @try {
      bool result = setSecureStorageItem(key, value, accessibleValue);
      return Value(result);
    } @catch (NSException *exception) {
      return Value(false);
    }
  });

  auto getItem = CREATE_HOST_FN("getItem", 1)
  {
    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "getItem: key must be a string value!");
    }

    NSString *key = convertJSIStringToNSString(runtime, arguments[0].getString(runtime));

    @try {
      NSMutableDictionary *query = generateBaseQueryDictionary(key);
      [query setObject:(id)kSecMatchLimitOne forKey:(id)kSecMatchLimit];
      [query setObject:(id)kCFBooleanTrue forKey:(id)kSecReturnData];

      CFDataRef result = nil;
      OSStatus status = SecItemCopyMatching((CFDictionaryRef)query, (CFTypeRef *)&result);

      if (status == errSecSuccess) {
        NSData *value = (__bridge NSData *)result;
        if (value != nil) {
          NSString *result = [[NSString alloc] initWithData:value encoding:NSUTF8StringEncoding];
          return Value(convertNSStringToJSIString(runtime, result));
        } else {
          return Value();
        }
      } else {
        return Value();
      }
    } @catch (NSException *exception) {
      return Value();
    }
  });

  auto setItems = CREATE_HOST_FN("setItems", 1)
  {
    NSDictionary *items = convertJSIObjectToNSDictionary(runtime, arguments[0].getObject(runtime));
    try {
      for (NSDictionary *item in items.objectEnumerator) {
        NSString *key = item[(id) @"key"];
        NSString *value = item[(id) @"value"];
        NSString *accessibleValue = item[(id) @"accessible"];
        setSecureStorageItem(key, value, _accessibleValue(accessibleValue));
      }
      return Value(true);
    } catch (NSException *exception) {
      return Value(false);
    }

    return Value();
  });

  auto getAllKeys = CREATE_HOST_FN("getAllKeys", 0)
  {
    NSMutableArray<NSString *> *keys = [NSMutableArray array];
    NSDictionary *query = @{
      (id)kSecClass : (id)kSecClassGenericPassword,
      (id)kSecReturnData : (id)kCFBooleanTrue,
      (id)kSecReturnAttributes : (id)kCFBooleanTrue,
      (id)kSecMatchLimit : (id)kSecMatchLimitAll
    };

    CFTypeRef result = NULL;

    OSStatus statusCode = SecItemCopyMatching((__bridge CFDictionaryRef)query, &result);

    if (statusCode == noErr) {
      NSArray<NSDictionary *> *array = (__bridge_transfer NSArray<NSDictionary *> *)result;
      for (NSDictionary *item in array) {
        NSString *key = [[NSString alloc] initWithData:item[(id)kSecAttrAccount]
                                              encoding:NSUTF8StringEncoding];
        [keys addObject:key];
      }

      return Value(convertNSArrayToJSIArray(runtime, keys));
    }
    return Value(convertNSArrayToJSIArray(runtime, keys));
  });

  auto getAllItems = CREATE_HOST_FN("getAllItems", 0)
  {
    NSMutableArray<NSDictionary *> *items = [NSMutableArray array];
    NSDictionary *query = @{
      (id)kSecClass : (id)kSecClassGenericPassword,
      (id)kSecReturnData : (id)kCFBooleanTrue,
      (id)kSecReturnAttributes : (id)kCFBooleanTrue,
      (id)kSecMatchLimit : (id)kSecMatchLimitAll
    };
    CFTypeRef result = NULL;

    OSStatus statusCode = SecItemCopyMatching((__bridge CFDictionaryRef)query, &result);

    if (statusCode == noErr) {
      NSArray<NSDictionary *> *array = (__bridge_transfer NSArray<NSDictionary *> *)result;
      for (NSDictionary *item in array) {
        NSString *key = [[NSString alloc] initWithData:item[(id)kSecAttrAccount]
                                              encoding:NSUTF8StringEncoding];
        NSString *value = [[NSString alloc] initWithData:item[(id)kSecValueData]
                                                encoding:NSUTF8StringEncoding];
        [items addObject:@{@"key" : key, @"value" : value}];
      }

      return Value(convertNSArrayToJSIArray(runtime, items));
    }

    return Value(convertNSArrayToJSIArray(runtime, items));
  });

  auto hasValue = CREATE_HOST_FN("hasValue", 1)
  {
    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "hasValue: key must be a string!");
    }

    NSString *key = convertJSIStringToNSString(runtime, arguments[0].getString(runtime));

    @try {
      NSMutableDictionary *queryDictionary = generateBaseQueryDictionary(key);
      [queryDictionary setObject:(id)kSecMatchLimitOne forKey:(id)kSecMatchLimit];
      [queryDictionary setObject:(id)kCFBooleanTrue forKey:(id)kSecReturnData];

      CFTypeRef result = NULL;
      OSStatus status = SecItemCopyMatching((CFDictionaryRef)queryDictionary, (CFTypeRef *)&result);

      if (status != errSecItemNotFound) {
        return Value(true);
      }
      return Value(false);
    } @catch (NSException *exception) {
      return Value(false);
    }
  });

  auto removeItem = CREATE_HOST_FN("removeItem", 1)
  {
    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "removeItem: key must be a string value");
    }

    NSString *key = convertJSIStringToNSString(runtime, arguments[0].getString(runtime));

    @try {
      NSMutableDictionary *queryDictionary = generateBaseQueryDictionary(key);
      OSStatus status = SecItemDelete((CFDictionaryRef)queryDictionary);
      if (status == errSecSuccess) {
        return Value(true);
      }
      return Value(false);
    } @catch (NSException *exception) {
      return Value(false);
    }
  });

  auto clearStorage = CREATE_HOST_FN("clearStorage", 0)
  {
    clearSecureStorage();

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
} // namespace secureStorage
