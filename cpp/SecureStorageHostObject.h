#include <ReactCommon/CallInvoker.h>
#include <jsi/jsilib.h>

namespace securestorageHostObject {
namespace react = facebook::react;
namespace jsi = facebook::jsi;
using namespace std;

void install(
    jsi::Runtime &rt,
    shared_ptr<react::CallInvoker> jsCallInvoker,
    function<bool(const string, const string, const string)> setItemFn,
    function<string(const string)> getItemFn,
    function<bool(const string)> deleteItemFn,
    function<void()> clearStorageFn,
    function<string()> getAllKeysFn,
    function<string()> getAllItemsFn,
    function<bool(string)> hasItemFn);
} // namespace securestorageHostObject
