#import "FastSecureStorage.h"
#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <ReactCommon/RCTTurboModule.h>
#import <jsi/jsi.h>
#import "SecureStorage.h"
#import "SecureStorageHostObject.h"

using namespace facebook;

@implementation FastSecureStorage

RCT_EXPORT_MODULE()

#ifdef RCT_NEW_ARCH_ENABLED
- (void)installJSIBindingsWithRuntime:(facebook::jsi::Runtime &)runtime
                           callInvoker:(const std::shared_ptr<facebook::react::CallInvoker> &)callInvoker
{
  handleAppUninstall();
  securestorageHostObject::install(
      runtime,
      callInvoker,
      &setSecureStorageItem,
      &getSecureStorageItem,
      &deleteSecureStorageItem,
      &clearSecureStorage,
      &getAllKeys,
      &getAllItems,
      &secureStorageHasItem);
}
#endif

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install)
{
#ifdef RCT_NEW_ARCH_ENABLED
  return @true;
#else
  RCTBridge *bridge = [RCTBridge currentBridge];
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)bridge;
  if (cxxBridge == nil) {
    return @false;
  }

  auto jsiRuntime = (jsi::Runtime *)cxxBridge.runtime;
  if (jsiRuntime == nil) {
    return @false;
  }

  auto callInvoker = bridge.jsCallInvoker;
  handleAppUninstall();
  securestorageHostObject::install(
      *(jsi::Runtime *)jsiRuntime,
      callInvoker,
      &setSecureStorageItem,
      &getSecureStorageItem,
      &deleteSecureStorageItem,
      &clearSecureStorage,
      &getAllKeys,
      &getAllItems,
      &secureStorageHasItem);

  return @true;
#endif
}

// Don't compile this code when we build for the old architecture.
#ifdef RCT_NEW_ARCH_ENABLED
- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params
{
  return std::make_shared<facebook::react::NativeFastSecureStorageSpecJSI>(params);
}
#endif

@end
