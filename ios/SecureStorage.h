//
//  Header.h
//  Pods
//
//  Created by Ivan Ignathuk on 18/09/2024.
//
#import <Foundation/Foundation.h>
#import <Security/Security.h>
#import <iostream>

CFStringRef _accessibleValue(const std::string accessible);
NSString *getServiceName();
NSMutableDictionary *generateBaseQueryDictionary(const std::string key);
void clearSecureStorage();
void handleAppUninstall();
void setServiceName(NSString *_serviceName);
std::string getAllKeys();
std::string getAllItems();
std::string getSecureStorageItem(const std::string key);
bool secureStorageHasItem(const std::string key);
bool setSecureStorageItem(
    const std::string key,
    const std::string value,
    const std::string accessibleValue);
bool deleteSecureStorageItem(const std::string key);
