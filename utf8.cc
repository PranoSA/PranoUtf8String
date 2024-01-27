#include <napi.h>

void Cleanup(napi_env env, void *data)
{
    delete static_cast<Napi::FunctionReference *>(data);
    // delete static_cast<Napi::FunctionReference *>(arg);
}

class PranoUtf8String : public Napi::ObjectWrap<PranoUtf8String>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports); // Method to initialize the type
    PranoUtf8String(const Napi::CallbackInfo &info);               // Constructor           // Example Method
    Napi::Value Split(const Napi::CallbackInfo &info);
    Napi::Value ToString(const Napi::CallbackInfo &info);
    static Napi::Object NewInstance(Napi::Env env, Napi::String value);
    static Napi::FunctionReference constructor;
    Napi::Value UnicodeValues(const Napi::CallbackInfo &info);

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
    // this->value_ = value.Utf16Value();
    // this->value_ = value;

    /*Napi::String str = value.As<Napi::String>();
    std::u16string utf16_value = str.Utf16Value();
    std::string utf16_bytes(reinterpret_cast<const char *>(utf16_value.data()), utf16_value.size() * sizeof(char16_t));

    this->value_ = utf16_bytes;*/
}

Napi::Object PranoUtf8String::NewInstance(Napi::Env env, Napi::String value)
{
    Napi::EscapableHandleScope scope(env);

    Napi::Object obj = PranoUtf8String::constructor.New({value});
    return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value PranoUtf8String::UnicodeValues(const Napi::CallbackInfo &info)
{
    /**
     * @brief
     * For Each Codepoint, Get the Unicode Value
     * This can be variable bytes,
     *
     * The Length of the return array is unknown, since the bytes can differ from number of codepoints
     */

    // Create a new array
    Napi::Array array = Napi::Array::New(info.Env());

    // Iterate over the string
    int iter = 0;
    int current_codepoint = 0;
    while (iter < this->value_.length())
    {
        // Find How Many bytes the codepoint is
        int numberOfBytes = 0;
        switch (this->value_[iter] & 0xF0)
        {
        case 0xF0:
            numberOfBytes = 3;
            // 4 Bytes
            break;
        case 0xE0:
            numberOfBytes = 2;
            // 3 Bytes
            break;
        case 0xC0:
            numberOfBytes = 1;
            // 2 Bytes
            break;
        case 0x80:

            // 1 Byte
            break;
        default:
            // 1 Byte
            break;
        }
        numberOfBytes++;

        // Now find the unicode value of the codepoint

        // If 1 Byte, then the unicode value is the same as the codepoint
        if (numberOfBytes == 1)
        {
            array.Set(current_codepoint, this->value_[iter]);
        }

        if (numberOfBytes == 2)
        {
            array.Set(current_codepoint, (this->value_[iter] & 0x1F) * 64 + (this->value_[iter + 1] & 0x3F));
        }

        if (numberOfBytes == 3)
        {
            array.Set(current_codepoint, (this->value_[iter] & 0x0F) * 4096 + (this->value_[iter + 1] & 0x3F) * 64 + (this->value_[iter + 2] & 0x3F));
        }

        if (numberOfBytes == 4)
        {
            array.Set(current_codepoint, (this->value_[iter] & 0x07) * 262144 + (this->value_[iter + 1] & 0x3F) * 4096 + (this->value_[iter + 2] & 0x3F) * 64 + (this->value_[iter + 3] & 0x3F));
        }

        iter += numberOfBytes;

        current_codepoint++;
    }

    return array;
}

Napi::Value PranoUtf8String::Split(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }

    int start = info[0].As<Napi::Number>().Int32Value();
    int end = info[1].As<Napi::Number>().Int32Value();

    // Ensure start and end are within the string length
    if (start < 0 || end > this->value_.length() || start > end)
    {
        Napi::RangeError::New(env, "Invalid range").ThrowAsJavaScriptException();
    }

    // Extract the substring
    // find starting and ending byte index of the substring in utf8 string

    // find startByteIndex and endByteIndex, count is the number of codepoints
    // For finding the endByteIndex, go to the end
    int startByteIndex = 0, endByteIndex = 0, count = 0;

    for (int i = 0; i < this->value_.length(); i++)
    {

        // Condition for start of new index -> starts with 01 or 11, otherwise continuation bit
        // This would mean either ASCII Compliant Character or start of new codepoint
        if ((this->value_[i] & 0xC0) != 0x80)
        {
            printf("New Count %d, index %d\n", count, i);
            count++;
        }

        if (count == (start + 1))
        {
            startByteIndex = i;
            break;
        }
    }

    // Now Scoot over to the end of the codepoint
    // Find Number of Bytes from the startByteIndex

    count = 0;
    // std::cout << "The value of startByteIndex is: " << startByteIndex << std::endl;
    for (int i = 0; i < this->value_.length(); i++)
    {
        // Condition for start of new index -> starts with 01 or 11, otherwise continuation bit
        // This would mean either ASCII Compliant Character or start of new codepoint

        if ((this->value_[i] & 0xC0) != 0x80)
        {
            printf("New Count %d, index %d\n", count, i);
            count++;
        }
        if (count == (end + 1))
        {
            endByteIndex = i;
            break;
        }

        // If either starts with 11 or 01, then it is a new codepoint
        //  This is because all continuation bits start with 10
        /* if (((this->value_[i] & 0xB0) == 0xB0) or (this->value_[i] & 0x40 == 0x40)) //
         {
             printf("New Count %d, index %d\n", count, i);
             count++;
         }*/
    }

    // Now Scoot over to the end of the codepoint
    // Find Number of Bytes from the endByteIndex
    //  This is the number of 1s before a 0 in the first byte
    int numberOfBytes = 0;
    switch (this->value_[endByteIndex - 1] & 0xF0)
    {
    case 0xF0:
        numberOfBytes = 3;
        // 4 Bytes
        break;
    case 0xE0:
        numberOfBytes = 2;
        // 3 Bytes
        break;
    case 0xC0:
        numberOfBytes = 1;
        // 2 Bytes
        break;
    case 0x80:
        // 1 Byte
        break;
    default:
        // 1 Byte
        break;
    }

    // std::string substring = this->value_.substr(start, end - start);
    printf("The value of startByteIndex is: %d\n", startByteIndex);
    printf("The value of endByteIndex is: %d\n", endByteIndex + numberOfBytes);
    std::string substring = this->value_.substr(startByteIndex, endByteIndex - startByteIndex + numberOfBytes);

    // std::cout << "The value of substring is: " << substring << std::endl;
    // Create a new PranoUtf8String object with the substring

    Napi::Object instance = PranoUtf8String::NewInstance(env, Napi::String::New(env, substring));

    return instance;

    Napi::FunctionReference *constructor = env.GetInstanceData<Napi::FunctionReference>();
    if (constructor != nullptr)
    {

        constructor->New({Napi::String::New(env, substring)});
        Napi::Object instance = constructor->New({Napi::String::New(env, substring)});
        return instance;

        // Napi::Object obj = constructor->New({Napi::String::New(env, substring)});
        // return new PranoUtf8String(substring);
        // return obj;
    }
    else
    {
        Napi::Error::New(env, "Constructor is not initialized").ThrowAsJavaScriptException();
        return env.Null();
    }
    Napi::Object obj = constructor->New({Napi::String::New(env, substring)});

    return obj;
}

Napi::FunctionReference PranoUtf8String::constructor;
/*
Napi::Object PranoUtf8String::Init(Napi::Env env, Napi::Object exports)
{

    Napi::Function func = DefineClass(env, "PranoUtf8String", {
                                                                  InstanceMethod("split", &PranoUtf8String::Split),
                                                              });

    auto constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("MyObject", func);
    return exports;
}
*/
Napi::Object PranoUtf8String::Init(Napi::Env env, Napi::Object exports)
{
    // Define the class and its methods
    Napi::Function func = DefineClass(env, "PranoUtf8String",
                                      {InstanceMethod("split", &PranoUtf8String::Split),
                                       InstanceMethod("toString", &PranoUtf8String::ToString),
                                       InstanceMethod("UnicodeValues", &PranoUtf8String::UnicodeValues)});

    // Create a persistent reference to the constructor
    constructor = Napi::Persistent(func);
    // constructor.SuppressDestruct();

    env.SetInstanceData<Napi::FunctionReference>(&constructor);

    // Register the cleanup hook to remove the reference to the constructor
    napi_add_env_cleanup_hook(env, reinterpret_cast<napi_cleanup_hook>(Cleanup), nullptr);

    // Set the constructor as a property of the exports object
    exports.Set("PranoUtf8String", func);

    return exports;
}

/**
 * @brief
 * What This Do?
 *
 * @param env
 * @param exports
 * @return Napi::Object
 */
Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    // PranoUtf8String::Init(env, exports);
    // exports.Set("MyObject", PranoUtf8String::Init(env, exports));
    // exports.Set("PranoUtf8String", PranoUtf8String::Init(env, exports));
    PranoUtf8String::Init(env, exports);
    return exports;
}

/**
 * @brief
 * Define a Function That Splits by Code POint of a current PranoUtf8String
 * For Example (3,6) returns codepoints 3 to 6
 * As a new PranoUtf8String
 */

Napi::Value PranoUtf8String::ToString(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    // Return the value of the string -> utf-16 or what??
    return Napi::String::New(env, this->value_);
}

NODE_API_MODULE(addon, InitAll)
