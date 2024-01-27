// addon.cc

#include <napi.h>

class MyObject : public Napi::ObjectWrap<MyObject>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports); // Method to initialize the type
    MyObject(const Napi::CallbackInfo &info);                      // Constructor
    Napi::Value PlusOne(const Napi::CallbackInfo &info);           // Example Method

private:
    double value_;
};

class PranoUtf8String : public Napi::ObjectWrap<PranoUtf8String>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports); // Method to initialize the type
    PranoUtf8String(const Napi::CallbackInfo &info);               // Constructor
    Napi::Value PlusOne(const Napi::CallbackInfo &info);           // Example Method
private:
    std::string value_;
};

/**
 * @brief Construct a construct
 *
 *
 * @param info
 */

PranoUtf8String::PranoUtf8String(const Napi::CallbackInfo &info) : Napi::ObjectWrap<PranoUtf8String>(info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString())
    {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }
    Napi::String value = info[0].As<Napi::String>();
    this->value_ = value.Utf8Value();
}

MyObject::MyObject(const Napi::CallbackInfo &info) : Napi::ObjectWrap<MyObject>(info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }
    Napi::Number value = info[0].As<Napi::Number>();
    this->value_ = value.DoubleValue();
}

Napi::Value MyObject::PlusOne(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    value_ += 1;
    return Napi::Number::New(env, value_);
}

Napi::Object MyObject::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "MyObject", {InstanceMethod("plusOne", &MyObject::PlusOne)});

    auto constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("MyObject", func);
    return exports;
}

/*MyObject::MyObject(const Napi::CallbackInfo &info) : Napi::ObjectWrap<MyObject>(info)
{
    value_ = info[0].As<Napi::Number>().DoubleValue();
}*/

// This is the implementation of the "add" method
// Input arguments are passed using the
// const Napi::CallbackInfo& info struct
Napi::Number Add(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    // Check the number of arguments passed.
    if (info.Length() < 2)
    {
        // Throw an Error that is passed back to JavaScript
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return Napi::Number::New(env, 0);
    }

    // Check the argument types
    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return Napi::Number::New(env, 0);
    }

    double arg0 = info[0].As<Napi::Number>().DoubleValue();
    double arg1 = info[1].As<Napi::Number>().DoubleValue();
    Napi::Number sum = Napi::Number::New(env, arg0 + arg1);

    // printf("arg0: %d \n", info[0]);

    return sum;
    // ... rest of your function implementation ...
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("add", Napi::Function::New(env, Add));
    return exports;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    exports.Set("add", Napi::Function::New(env, Add));
    MyObject::Init(env, exports);
    return exports;
}

NODE_API_MODULE(addon, InitAll)

// NODE_API_MODULE(addon, Init)

/*#include <node.h>
#include <napi.h>

namespace demo
{

    using v8::Exception;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::String;
    using v8::Value;

    // This is the implementation of the "add" method
    // Input arguments are passed using the
    // const FunctionCallbackInfo<Value>& args struct
    void Add(const FunctionCallbackInfo<Value> &args)
    {
        Isolate *isolate = args.GetIsolate();

        // Check the number of arguments passed.
        if (args.Length() < 2)
        {
            // Throw an Error that is passed back to JavaScript
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate,
                                    "Wrong number of arguments")
                    .ToLocalChecked()));
            return;
        }

        // Check the argument types
        if (!args[0]->IsNumber() || !args[1]->IsNumber())
        {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate,
                                    "Wrong arguments")
                    .ToLocalChecked()));
            return;
        }

        // Perform the operation
        double value =
            args[0].As<Number>()->Value() + args[1].As<Number>()->Value();
        Local<Number> num = Number::New(isolate, value);

        // Set the return value (using the passed in
        // FunctionCallbackInfo<Value>&)
        args.GetReturnValue().Set(num);
    }

    void Init(Local<Object> exports)
    {
        NODE_SET_METHOD(exports, "add", Add);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

} // namespace demo
*/