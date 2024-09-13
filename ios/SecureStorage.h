#include <ReactCommon/CallInvoker.h>
#include <jsi/jsilib.h>

namespace secureStorage {
namespace react = facebook::react;
namespace jsi = facebook::jsi;

void install(
    jsi::Runtime &rt,
    std::shared_ptr<react::CallInvoker> jsCallInvoker);
void handleAppUninstall();
} // namespace secureStorage
