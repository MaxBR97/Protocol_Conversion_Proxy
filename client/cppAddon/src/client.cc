// #include <stdlib.h>
#include <v8.h>
#include <node.h>
#include <uv.h>
#include <string>
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
using namespace v8;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

struct Baton {
  uv_work_t request;
  Persistent<Function> callback;
  std::string result;
};

void AsyncAfter(uv_work_t* req, int status) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  Baton* baton = static_cast<Baton*>(req->data);
  Local<Function> callback = Local<Function>::New(isolate, baton->callback);

  if (status == 0) {
    // Call the JavaScript callback with the result
    const unsigned argc = 1;
    Local<Value> argv[argc] = {String::NewFromUtf8(isolate, baton->result.c_str()).ToLocalChecked()};
    callback->Call(isolate->GetCurrentContext(), Null(isolate), argc, argv);
  }

  delete baton;
}

void InitializeClient(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  initializeIntegration();
}

void GetOutputAsync(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check if a callback function was provided as an argument
  if (args.Length() < 1 || !args[0]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Callback function required").ToLocalChecked()));
    return;
  }

  // Extract the JavaScript callback function
  Local<Function> callback = Local<Function>::Cast(args[0]);

  // Create a baton for async work
  Baton* baton = new Baton;
  baton->request.data = baton;
  baton->callback.Reset(isolate, callback);

  // Perform the blocking operation asynchronously using libuv
  uv_queue_work(
    uv_default_loop(),
    &baton->request,
    [](uv_work_t* req) {
      // This is the async operation that may block
      Baton* baton = static_cast<Baton*>(req->data);
      baton->result = systemObserver.getKeyboardOutput();
    },
    AsyncAfter); // Use the AsyncAfter function for completion

  args.GetReturnValue().SetUndefined();
}


void SetInput(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  
  String::Utf8Value str(isolate, args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked());
  std::string input(*str);
  systemObserver.setKeyboardInput(input);
}

void GetFrameOutAsync(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check if a callback function was provided as an argument
  if (args.Length() < 1 || !args[0]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Callback function required").ToLocalChecked()));
    return;
  }

  // Extract the JavaScript callback function
  Local<Function> callback = Local<Function>::Cast(args[0]);

  // Create a baton for async work
  Baton* baton = new Baton;
  baton->request.data = baton;
  baton->callback.Reset(isolate, callback);

  // Perform the blocking operation asynchronously using libuv
  uv_queue_work(
    uv_default_loop(),
    &baton->request,
    [](uv_work_t* req) {
      // This is the async operation that may block
      Baton* baton = static_cast<Baton*>(req->data);
      baton->result = systemObserver.getFrameOut();
    },
    AsyncAfter); // Use the AsyncAfter function for completion

  args.GetReturnValue().SetUndefined();
}

void GetFrameInAsync(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check if a callback function was provided as an argument
  if (args.Length() < 1 || !args[0]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Callback function required").ToLocalChecked()));
    return;
  }

  // Extract the JavaScript callback function
  Local<Function> callback = Local<Function>::Cast(args[0]);

  // Create a baton for async work
  Baton* baton = new Baton;
  baton->request.data = baton;
  baton->callback.Reset(isolate, callback);

  // Perform the blocking operation asynchronously using libuv
  uv_queue_work(
    uv_default_loop(),
    &baton->request,
    [](uv_work_t* req) {
      // This is the async operation that may block
      Baton* baton = static_cast<Baton*>(req->data);
      baton->result = systemObserver.getFrameIn();
    },
    AsyncAfter); // Use the AsyncAfter function for completion

  args.GetReturnValue().SetUndefined();
}


void SetSleepTimes(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
  //   isolate->ThrowException(Exception::TypeError(
  //       String::NewFromUtf8(isolate, "Both arguments must be integers")));
  //   return;
  // }
  
  int nano = args[1]->Int32Value(isolate->GetCurrentContext()).ToChecked();
  int secs = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
  
  systemObserver.setSleepDelays(secs, nano);
}


void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "getOutput", GetOutputAsync);
  NODE_SET_METHOD(exports, "setInput", SetInput);
  NODE_SET_METHOD(exports, "getFrameOut", GetFrameOutAsync);
  NODE_SET_METHOD(exports, "getFrameIn", GetFrameInAsync);
  NODE_SET_METHOD(exports, "initialize", InitializeClient);
  NODE_SET_METHOD(exports, "setSleepTimes", SetSleepTimes);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace demo 








// // #include <stdlib.h>
// #include <v8.h>
// #include <node.h>
// #include <uv.h>
// #include <string>
// // #include <iostream>
// // #include <vector>
// // #include <map>
// // #include <unordered_map>
// // #include "../include/event.h"
// // #include "../include/StompFrame.h"
// // //#include "../include/ConnectionHandler.h"
// // #include "../include/StompProtocol.h"
// // #include "../include/InputProtocol.h"
// // //#include "ConnectionHandler.cpp"
// #include "StompClient.cpp"


// namespace demo {
// using namespace v8;
// using v8::FunctionCallbackInfo;
// using v8::Isolate;
// using v8::Local;
// using v8::Object;
// using v8::String;
// using v8::Value;

// void InitializeClient(const FunctionCallbackInfo<Value>& args) {
//   Isolate* isolate = args.GetIsolate();
//   initializeIntegration();
// }

// void GetOutput(const FunctionCallbackInfo<Value>& args) {
//   Isolate* isolate = args.GetIsolate();
  
//   std::string result ( systemObserver.getKeyboardOutput());
//   args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
// }

// void SetInput(const FunctionCallbackInfo<Value>& args) {
//   Isolate* isolate = args.GetIsolate();
  
//   String::Utf8Value str(isolate, args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked());
//   std::string input(*str);
//   systemObserver.setKeyboardInput(input);
// }

// struct Baton {
//   uv_work_t request;
//   Persistent<Function> callback;
//   std::string result;
// };

// std::string getFrameOutSync() {
//   // Simulate a blocking operation
//   return systemObserver.getFrameOut();
// }

// void getFrameOutAsyncWorker(uv_work_t* req) {
//   Baton* baton = static_cast<Baton*>(req->data);
//   baton->result = getFrameOutSync();
// }

// void getFrameOutAsyncAfter(uv_work_t* req, int status) {
//   Isolate* isolate = Isolate::GetCurrent();
//   HandleScope scope(isolate);
//   Baton* baton = static_cast<Baton*>(req->data);
//   Local<Function> callback = Local<Function>::New(isolate, baton->callback);

//   if (status == 0) {
//     // Call the JavaScript callback with the result
//     const unsigned argc = 1;
//     Local<Value> argv[argc] = {String::NewFromUtf8(isolate, baton->result.c_str()).ToLocalChecked()};
//     callback->Call(isolate->GetCurrentContext(), Null(isolate), argc, argv);
//   }

//   delete baton;
// }

// void GetFrameOutAsync(const FunctionCallbackInfo<Value>& args) {
//   Isolate* isolate = args.GetIsolate();

//   // Check if a callback function was provided as an argument
//   if (args.Length() < 1 || !args[0]->IsFunction()) {
//     isolate->ThrowException(Exception::TypeError(
//         String::NewFromUtf8(isolate, "Callback function required").ToLocalChecked()));
//     return;
//   }

//   // Extract the JavaScript callback function
//   Local<Function> callback = Local<Function>::Cast(args[0]);

//   // Create a baton for async work
//   Baton* baton = new Baton;
//   baton->request.data = baton;
//   baton->callback.Reset(isolate, callback);

//   // Perform the blocking operation asynchronously using libuv
//   uv_queue_work(
//     uv_default_loop(),
//     &baton->request,
//     getFrameOutAsyncWorker,
//     getFrameOutAsyncAfter);

//   args.GetReturnValue().SetUndefined();
// }


// void GetFrameIn(const FunctionCallbackInfo<Value>& args) {
//   Isolate* isolate = args.GetIsolate();
  
//   std::string result (systemObserver.getFrameIn());
//   args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
// }

// void SetSleepTimes(const FunctionCallbackInfo<Value>& args) {
//   Isolate* isolate = args.GetIsolate();

//   // if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
//   //   isolate->ThrowException(Exception::TypeError(
//   //       String::NewFromUtf8(isolate, "Both arguments must be integers")));
//   //   return;
//   // }
  
//   int nano = args[1]->Int32Value(isolate->GetCurrentContext()).ToChecked();
//   int secs = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
  
//   systemObserver.setSleepDelays(secs, nano);
// }


// void Initialize(Local<Object> exports) {
//   NODE_SET_METHOD(exports, "getOutput", GetOutput);
//   NODE_SET_METHOD(exports, "setInput", SetInput);
//   NODE_SET_METHOD(exports, "getFrameOut", GetFrameOutAsync);
//   NODE_SET_METHOD(exports, "getFrameIn", GetFrameIn);
//   NODE_SET_METHOD(exports, "initialize", InitializeClient);
//   NODE_SET_METHOD(exports, "setSleepTimes", SetSleepTimes);
// }

// NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

// }  // namespace demo 


