#ifndef macros_h
#define macros_h

#define CREATE_HOST_FN(name, basecount)           \
jsi::Function::createFromHostFunction( \
runtime, \
jsi::PropNameID::forAscii(runtime, name), \
basecount, \
[=](jsi::Runtime &runtime, const jsi::Value &thisValue, const jsi::Value *arguments, size_t count) -> jsi::Value

#endif /* macros_h */