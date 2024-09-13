#include <ReactCommon/CallInvoker.h>
#include <jsi/jsilib.h>

using namespace facebook;
using namespace facebook::react;
using namespace std;

namespace securestorage {
void install(
    jsi::Runtime &rt,
    function<bool(const char *, const char *)> setItemFn,
    function<string(const char *)> getItemFn,
    function<bool(const char *)> deleteItemFn,
    function<bool()> clearStorageFn,
    function<string()> getAllKeysFn,
    function<string()> getAllItems);
}
