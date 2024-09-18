//
//  SecureStorage.mm
//  react-native-fast-secure-storage
//
//  Created by Ivan Ignathuk on 18/09/2024.
//

#import "SecureStorage.h"
#import <Foundation/Foundation.h>
#import <Security/Security.h>

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

std::string getAllKeys()
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

    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:keys
                                                       options:NSJSONWritingPrettyPrinted
                                                         error:nil];
    NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];

    return [jsonString UTF8String];
  }
  return "[]";
}

std::string getAllItems()
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

    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:items
                                                       options:NSJSONWritingPrettyPrinted
                                                         error:nil];
    NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];

    return [jsonString UTF8String];
  }

  return "[]";
}

std::string getSecureStorageItem(const char *key)
{
  NSString *_key = [NSString stringWithUTF8String:key];
  NSMutableDictionary *query = generateBaseQueryDictionary(_key);
  [query setObject:(id)kSecMatchLimitOne forKey:(id)kSecMatchLimit];
  [query setObject:(id)kCFBooleanTrue forKey:(id)kSecReturnData];
  CFDataRef result = nil;
  OSStatus status = SecItemCopyMatching((CFDictionaryRef)query, (CFTypeRef *)&result);
  if (status == errSecSuccess) {
    NSData *value = (__bridge NSData *)result;
    if (value != nil) {
      return [[[NSString alloc] initWithData:value encoding:NSUTF8StringEncoding] UTF8String];
    } else {
      return "";
    }
  } else {
    return "";
  }
}

bool secureStorageHasItem(const char *key)
{
  NSString *_key = [NSString stringWithUTF8String:key];
  NSMutableDictionary *queryDictionary = generateBaseQueryDictionary(_key);
  [queryDictionary setObject:(id)kSecMatchLimitOne forKey:(id)kSecMatchLimit];
  [queryDictionary setObject:(id)kCFBooleanTrue forKey:(id)kSecReturnData];

  CFTypeRef result = NULL;
  OSStatus status = SecItemCopyMatching((CFDictionaryRef)queryDictionary, (CFTypeRef *)&result);

  return status != errSecItemNotFound;
}

bool setSecureStorageItem(const char *key, const char *value, const char *accessibilityLevel)
{
  NSString *_key = [NSString stringWithUTF8String:key];
  NSString *_value = [NSString stringWithUTF8String:value];
  CFStringRef accessibleValue =
      _accessibleValue([NSString stringWithUTF8String:accessibilityLevel]);
  NSMutableDictionary *dictionary = generateBaseQueryDictionary(_key);

  NSData *valueData = [_value dataUsingEncoding:NSUTF8StringEncoding];
  [dictionary setObject:valueData forKey:(id)kSecValueData];
  dictionary[(__bridge NSString *)kSecAttrAccessible] = (__bridge id)accessibleValue;

  OSStatus status = SecItemAdd((CFDictionaryRef)dictionary, NULL);

  if (status == errSecSuccess) {
    return true;
  } else {
    NSMutableDictionary *updateDictionary = [[NSMutableDictionary alloc] init];
    [updateDictionary setObject:valueData forKey:(id)kSecValueData];
    updateDictionary[(__bridge NSString *)kSecAttrAccessible] = (__bridge id)accessibleValue;
    NSMutableDictionary *searchDictionary = generateBaseQueryDictionary(_key);
    OSStatus status =
        SecItemUpdate((CFDictionaryRef)searchDictionary, (CFDictionaryRef)updateDictionary);

    return status == errSecSuccess;
  }
}

bool deleteSecureStorageItem(const char *key)
{
  NSString *_key = [NSString stringWithUTF8String:key];
  NSMutableDictionary *queryDictionary = generateBaseQueryDictionary(_key);
  OSStatus status = SecItemDelete((CFDictionaryRef)queryDictionary);
  return status == errSecSuccess;
}
