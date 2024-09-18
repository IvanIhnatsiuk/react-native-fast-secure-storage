#include <ReactCommon/CallInvoker.h>
#include <jsi/jsilib.h>

namespace securestorageHostObject {
namespace react = facebook::react;
namespace jsi = facebook::jsi;
using namespace std;

void install(
    jsi::Runtime &rt,
    shared_ptr<react::CallInvoker> jsCallInvoker,
    function<bool(const char *, const char *, const char *)> setItemFn,
    function<string(const char *)> getItemFn,
    function<bool(const char *)> deleteItemFn,
    function<void()> clearStorageFn,
    function<string()> getAllKeysFn,
    function<string()> getAllItemsFn,
    function<bool(const char *)> hasItemFn);
} // namespace securestorageHostObject
