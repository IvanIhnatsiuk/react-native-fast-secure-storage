
#ifdef RCT_NEW_ARCH_ENABLED
#import "RNFastSecureStorageSpec.h"
#import <ReactCommon/RCTTurboModuleWithJSIBindings.h>

@interface FastSecureStorage : NSObject <NativeFastSecureStorageSpec, RCTTurboModuleWithJSIBindings>
#else
#import <React/RCTBridgeModule.h>

@interface FastSecureStorage : NSObject <RCTBridgeModule>
#endif

@end
