
#ifdef RCT_NEW_ARCH_ENABLED
#import "RNFastSecureStorageSpec.h"

@interface FastSecureStorage : NSObject <NativeFastSecureStorageSpec>
#else
#import <React/RCTBridgeModule.h>

@interface FastSecureStorage : NSObject <RCTBridgeModule>
#endif

@end
