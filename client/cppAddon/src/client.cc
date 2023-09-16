// #include <stdlib.h>
#include <node.h>
// #include <iostream>
// #include <vector>
// #include <map>
// #include <unordered_map>
// #include "../include/event.h"
// #include "../include/StompFrame.h"
// //#include "../include/ConnectionHandler.h"
// #include "../include/StompProtocol.h"
// #include "../include/InputProtocol.h"
// //#include "ConnectionHandler.cpp"
#include "StompClient.cpp"

namespace demo {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void InitializeClient(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  initializeIntegration();
}

void GetOutput(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  
  std::string result ( systemObserver.getKeyboardOutput());
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
}

void SetInput(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  
  String::Utf8Value str(isolate, args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked());
  std::string input(*str);
  systemObserver.setKeyboardInput(input);
}

void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "getOutput", GetOutput);
  NODE_SET_METHOD(exports, "setInput", SetInput);
  NODE_SET_METHOD(exports, "initialize", InitializeClient);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace demo 












// #include <node_api.h>
// #include <StompClient.cpp>

// namespace client {

// napi_value Get(napi_env env, napi_callback_info info) {
//   size_t argc=1;
//   napi_value args[1];
//   int64_t upper_limit;
//   int64_t largest_prime;
//   napi_value output;

//   napi_get_cb_info(env,info, &argc, args, NULL, NULL);

//   napi_get_value_int64(env, args[0],&upper_limit);

//   largest_prime = upper_limit; // enter here the API of my program. upper_limit represents the arg given to the program.

//   napi_create_double(env, largest_prime, &output);

//   return output;
// }

// napi_value init(napi_env env, napi_value exports) {
//   napi_status status;
//   napi_value get;
//   napi_value set;

//   status = napi_create_function(env, nullptr, 0, Get, nullptr, &get);
//   if (status != napi_ok) return nullptr;

//   status = napi_set_named_property(env, exports, "get", get);
//   if (status != napi_ok) return nullptr;

//   status = napi_create_function(env, nullptr, 0, Set, nullptr, &set);
//   if (status != napi_ok) return nullptr;

//   status = napi_set_named_property(env, exports, "set", set);
//   if (status != napi_ok) return nullptr;
//   return exports;
// }

// NAPI_MODULE(NODE_GYP_MODULE_NAME, init)

// }  // namespace demo 