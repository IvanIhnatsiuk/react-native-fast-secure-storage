#include <jni.h>
#include <ReactCommon/CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <jni.h>
#include <jsi/jsi.h>
#include <typeinfo>
#include "bindings.h"
#include <iostream>

namespace jni = facebook::jni;
namespace react = facebook::react;
namespace jsi = facebook::jsi;

JavaVM *java_vm;
jclass java_class;
jobject java_object;

#define HOSTFN(name, basecount) \
jsi::Function::createFromHostFunction( \
runtime, \
jsi::PropNameID::forAscii(runtime, name), \
basecount, \
[=](jsi::Runtime &runtime, const jsi::Value &thisValue, const jsi::Value *arguments, size_t count) -> jsi::Value


/**
 * A simple callback function that allows us to detach current JNI Environment
 * when the thread
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */

void DeferThreadDetach(JNIEnv *env)
{
    static pthread_key_t thread_key;

    // Set up a Thread Specific Data key, and a callback that
    // will be executed when a thread is destroyed.
    // This is only done once, across all threads, and the value
    // associated with the key for any given thread will initially
    // be NULL.
    static auto run_once = []
    {
        const auto err = pthread_key_create(&thread_key, [](void *ts_env)
        {
            if (ts_env) {
                java_vm->DetachCurrentThread();
            } });
        if (err)
        {
            // Failed to create TSD key. Throw an exception if you want to.
        }
        return 0;
    }();

    // For the callback to actually be executed when a thread exits
    // we need to associate a non-NULL value with the key on that thread.
    // We can use the JNIEnv* as that value.
    const auto ts_env = pthread_getspecific(thread_key);
    if (!ts_env)
    {
        if (pthread_setspecific(thread_key, env))
        {
            // Failed to set thread-specific value for key. Throw an exception if you want to.
        }
    }
}

/**
 * Get a JNIEnv* valid for this thread, regardless of whether
 * we're on a native thread or a Java thread.
 * If the calling thread is not currently attached to the JVM
 * it will be attached, and then automatically detached when the
 * thread is destroyed.
 *
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */
JNIEnv *GetJniEnv()
{
    JNIEnv *env = nullptr;
    // We still call GetEnv first to detect if the thread already
    // is attached. This is done to avoid setting up a DetachCurrentThread
    // call on a Java thread.

    // g_vm is a global.
    auto get_env_result = java_vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (get_env_result == JNI_EDETACHED)
    {
        if (java_vm->AttachCurrentThread(&env, NULL) == JNI_OK)
        {
            DeferThreadDetach(env);
        }
        else
        {
            // Failed to attach thread. Throw an exception if you want to.
        }
    }
    else if (get_env_result == JNI_EVERSION)
    {
        // Unsupported JNI version. Throw an exception if you want to.
    }
    return env;
}

jstring string2jstring(JNIEnv *env, const char *str)
{
    return (*env).NewStringUTF(str);
}

bool setItem(const char* key, const char* value)
{
    JNIEnv *jniEnv = GetJniEnv();
    java_class = jniEnv->GetObjectClass(java_object);
    jmethodID setMethodID = jniEnv->GetMethodID(java_class, "setItem", "(Ljava/lang/String;Ljava/lang/String;)Z");
    jvalue params[2];
    params[0].l = string2jstring(jniEnv, key);
    params[1].l = string2jstring(jniEnv, value);


    auto result = jniEnv->CallBooleanMethodA(java_object, setMethodID, params);

    jthrowable exObj = jniEnv->ExceptionOccurred();

    if(exObj) {
        jclass clazz = jniEnv->GetObjectClass(exObj);
        jniEnv->ExceptionClear();jmethodID getMessage = jniEnv->GetMethodID(clazz,
                                                                            "toString",
                                                                            "()Ljava/lang/String;");

        auto message = (jstring)jniEnv->CallObjectMethod(exObj, getMessage);
        const char *mstr = jniEnv->GetStringUTFChars(message, NULL);
        throw std::runtime_error(std::string(mstr));
    }

    return result;
}

std::string getItem(const char* key)
{
    JNIEnv *jniEnv = GetJniEnv();
    java_class = jniEnv->GetObjectClass(java_object);
    jmethodID getItemMethodID = jniEnv->GetMethodID(java_class, "getItem", "(Ljava/lang/String;)Ljava/lang/String;");
    jvalue params[1];
    params[0].l = string2jstring(jniEnv,key);


    auto result = (jstring)jniEnv->CallObjectMethodA(java_object, getItemMethodID, params);
    jthrowable exObj = jniEnv->ExceptionOccurred();
    if(exObj) {
        jniEnv->ExceptionClear();

        jclass clazz = jniEnv->GetObjectClass(exObj);
        jmethodID getMessage = jniEnv->GetMethodID(clazz,
                                                   "toString",
                                                   "()Ljava/lang/String;");
        auto message = (jstring)jniEnv->CallObjectMethod(exObj, getMessage);
        const char *mstr = jniEnv->GetStringUTFChars(message, NULL);
        throw std::runtime_error(std::string(mstr));
    }

    std::string str = jniEnv->GetStringUTFChars((jstring)result, NULL);

    if(str.empty()){
        return "";
    }

    return str;
}

bool deleteItem(const char* key)
{
    JNIEnv *jniEnv = GetJniEnv();
    java_class = jniEnv->GetObjectClass(java_object);
    jmethodID deleteMethodID = jniEnv->GetMethodID(java_class, "removeItem", "(Ljava/lang/String;)Z");
    jvalue params[1];
    params[0].l = string2jstring(jniEnv, key);

    bool result = jniEnv->CallBooleanMethodA(java_object, deleteMethodID, params);
    jthrowable exObj = jniEnv->ExceptionOccurred();

    if(exObj) {
        jniEnv->ExceptionClear();

        jclass clazz = jniEnv->GetObjectClass(exObj);
        jmethodID getMessage = jniEnv->GetMethodID(clazz,
                                                   "toString",
                                                   "()Ljava/lang/String;");
        auto message = (jstring) jniEnv->CallObjectMethod(exObj, getMessage);
        const char *mstr = jniEnv->GetStringUTFChars(message, NULL);
        throw std::runtime_error(std::string(mstr));
        return result;
    }
    return result;
}

bool clearStorage()
{
    JNIEnv  *jniEnv = GetJniEnv();
    java_class = jniEnv->GetObjectClass(java_object);
    jmethodID clearMethodID = jniEnv->GetMethodID(java_class, "clearStorage", "()Z");
    jvalue params[0];
    jniEnv->CallBooleanMethodA(java_object, clearMethodID, params);
    jthrowable exObj = jniEnv->ExceptionOccurred();

    if(exObj){
        jniEnv->ExceptionClear();

        jclass clazz = jniEnv->GetObjectClass(exObj);
        jmethodID getMessage = jniEnv->GetMethodID(clazz,
                                                   "toString",
                                                   "()Ljava/lang/String;");
        auto message = (jstring)jniEnv->CallObjectMethod(exObj, getMessage);
        const char *mstr = jniEnv->GetStringUTFChars(message, NULL);
        throw std::runtime_error(std::string(mstr));

        return false;
    }

    return true;
}

std::string getAllKeys(){
    JNIEnv  *jniEnv = GetJniEnv();
    java_class = jniEnv->GetObjectClass(java_object);
    jmethodID getAllKeysMethodId = jniEnv->GetMethodID(java_class, "getAllKeys", "()Ljava/lang/String;");
    jvalue params[0];
    auto result = (jstring)jniEnv->CallObjectMethodA(java_object, getAllKeysMethodId, params);
    jthrowable exObj = jniEnv->ExceptionOccurred();

    if(exObj){
        jniEnv->ExceptionClear();

        jclass clazz = jniEnv->GetObjectClass(exObj);
        jmethodID getMessage = jniEnv->GetMethodID(clazz,
                                                   "toString",
                                                   "()Ljava/lang/String;");
        auto message = (jstring)jniEnv->CallObjectMethod(exObj, getMessage);
        const char *mstr = jniEnv->GetStringUTFChars(message, NULL);
        throw std::runtime_error(std::string(mstr));
    }
    std::string str = jniEnv->GetStringUTFChars((jstring)result, NULL);

    if(str.empty()){
        return "";
    }

    return str;
}

std::string getAllItems() {
    JNIEnv  *jniEnv = GetJniEnv();
    java_class = jniEnv->GetObjectClass(java_object);
    jmethodID getAllItemsMethodId = jniEnv->GetMethodID(java_class, "getAllItems", "()Ljava/lang/String;");
    jvalue params[0];
    auto result = (jstring)jniEnv->CallObjectMethodA(java_object, getAllItemsMethodId, params);
    jthrowable exObj = jniEnv->ExceptionOccurred();

    if(exObj){
        jniEnv->ExceptionClear();

        jclass clazz = jniEnv->GetObjectClass(exObj);
        jmethodID getMessage = jniEnv->GetMethodID(clazz,
                                                   "toString",
                                                   "()Ljava/lang/String;");
        auto message = (jstring)jniEnv->CallObjectMethod(exObj, getMessage);
        const char *mstr = jniEnv->GetStringUTFChars(message, NULL);
        throw std::runtime_error(std::string(mstr));
    }

    std::string str = jniEnv->GetStringUTFChars((jstring)result, NULL);

    if(str.empty()){
        return "";
    }

    return str;
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    java_vm = jvm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_fastsecurestorage_FastSecureStorageBridge_initialize(JNIEnv *env, jobject thiz,
                                                              jlong jsi_ptr) {
    auto jsiPointerRuntime = reinterpret_cast<jsi::Runtime *>(jsi_ptr);

    if(jsiPointerRuntime) {
        securestorage::install(*jsiPointerRuntime, *setItem, *getItem, *deleteItem,*clearStorage,*getAllKeys, *getAllItems);
    }

    env->GetJavaVM(&java_vm);
    java_object = env->NewGlobalRef(thiz);
}
