#import "FastSecureStorage.h"
#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <ReactCommon/RCTTurboModule.h>
#import <jsi/jsi.h>
#import "SecureStorage.h"
#import "SecureStorageHostObject.h"
#import "RNFastSecureStorageSpecJSI.h"

using namespace facebook;

static void performInstall(jsi::Runtime &runtime, std::shared_ptr<react::CallInvoker> callInvoker) {
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

#ifdef RCT_NEW_ARCH_ENABLED

class FastSecureStorageModule : public react::NativeFastSecureStorageCxxSpec<FastSecureStorageModule> {
public:
  FastSecureStorageModule(std::shared_ptr<react::CallInvoker> jsInvoker)
    : NativeFastSecureStorageCxxSpec(jsInvoker) {}

  bool install(jsi::Runtime &rt) {
    performInstall(rt, jsInvoker_);
    return true;
  }
};

#endif

@implementation FastSecureStorage

RCT_EXPORT_MODULE()

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install)
{
  RCTBridge *bridge = [RCTBridge currentBridge];
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)bridge;
  if (cxxBridge == nil) {
    return @false;
  }

  auto jsiRuntime = (jsi::Runtime *)cxxBridge.runtime;
  if (jsiRuntime == nil) {
    return @false;
  }

  performInstall(*(jsi::Runtime *)jsiRuntime, bridge.jsCallInvoker);
  return @true;
}

#ifdef RCT_NEW_ARCH_ENABLED
- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params
{
  return std::make_shared<FastSecureStorageModule>(params.jsInvoker);
}
#endif

@end
