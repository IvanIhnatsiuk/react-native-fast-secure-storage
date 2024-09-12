#include <jsi/jsilib.h>
#include <ReactCommon/CallInvoker.h>

using namespace facebook;
using namespace facebook::react;
using namespace std;

namespace securestorage {
     void install(jsi::Runtime &rt,
                 function<bool(const char *, const char *)> setItemFn,
                 function<string(const char *)> getItemFn,
                 function<bool(const char *)> deleteItemFn,
                 function<bool()> clearStorageFn,
                 function<string()> getAllKeysFn,
                 function<string()> getAllItems
                 );
}
