//
//  Header.h
//  Pods
//
//  Created by Ivan Ignathuk on 18/09/2024.
//
#import <Foundation/Foundation.h>
#import <Security/Security.h>
#import <iostream>

CFStringRef _accessibleValue(NSString *accessible);
NSString *getServiceName();
NSMutableDictionary *generateBaseQueryDictionary(NSString *key);
void clearSecureStorage();
void handleAppUninstall();
void setServiceName(NSString *_serviceName);
std::string getAllKeys();
std::string getAllItems();
std::string getSecureStorageItem(const char *key);
bool secureStorageHasItem(const char *key);
bool setSecureStorageItem(const char *key, const char *value, const char *accessibilityLevel);
bool deleteSecureStorageItem(const char *key);
