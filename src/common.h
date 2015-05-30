//
//  common.h
//  
//
//  Created by Henri Gourvest on 20/05/2015.
//
//

#ifndef _common_h
#define _common_h

#include <nan.h>

#define V8TOCSTRING(S) (*String::Utf8Value(S->ToString()))

#define V8CHECK(T, E) if (T) { \
    NanThrowTypeError(E);\
    return; \
 }

#define GLP_CATCH_RET(X) try{X}catch(...){NanThrowError("abort");return;}

#define GLP_CATCH(X) try{X}catch(...){NanThrowError("abort");}

#define GLP_DEFINE_CONSTANT(target, constant, name)\
do {\
Isolate* isolate = Isolate::GetCurrent();\
Local<String> constant_name =\
String::NewFromUtf8(isolate, #name);\
Local<Number> constant_value =\
Number::New(isolate, static_cast<double>(constant));\
PropertyAttribute constant_attributes =\
static_cast<PropertyAttribute>(ReadOnly | DontDelete);\
(target)->ForceSet(constant_name, constant_value, constant_attributes);\
}\
while (0)

#define GLP_BIND_VOID_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsString(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(API(host->handle, V8TOCSTRING(args[0]));)\
}

#define GLP_BIND_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(const char* name = API(host->handle);\
    if (name)\
        NanReturnValue(name);\
    else\
        NanReturnValue("");)\
}

#define GLP_BIND_VOID_INT32(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(API(host->handle, args[0]->Int32Value());)\
}

#define GLP_BIND_VALUE(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(NanReturnValue(API(host->handle));)\
}

#define GLP_BIND_VOID_INT32_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32() || !args[1]->IsString(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(API(host->handle, args[0]->Int32Value(), V8TOCSTRING(args[1]));)\
}

#define GLP_BIND_STR_INT32(CLASS, NAME, API);\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(const char* name = API(host->handle, args[0]->Int32Value());\
    if (name)\
        NanReturnValue(name);\
    else\
        NanReturnValue("");)\
}

#define GLP_BIND_VOID_INT32_INT32_DOUBLE_DOUBLE(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 4, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32() || !args[1]->IsInt32()\
            || !args[2]->IsNumber() || !args[3]->IsNumber(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(API(host->handle, args[0]->Int32Value(), args[1]->Int32Value(),\
                     args[2]->NumberValue(), args[3]->NumberValue());)\
}

#define GLP_BIND_VOID_INT32_DOUBLE(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32() || !args[1]->IsNumber(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(API(host->handle, args[0]->Int32Value(), args[1]->NumberValue());)\
}

#define GLP_BIND_VALUE_INT32(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(NanReturnValue(API(host->handle, args[0]->Int32Value()));)\
}

#define GLP_BIND_VOID_INT32_INT32ARRAY_FLOAT64ARRAY(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 3, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsUint32() || !args[1]->IsInt32Array() || !args[2]->IsFloat64Array(), "Wrong arguments");\
    \
    Local<Int32Array> ja = Local<Int32Array>::Cast(args[1]);\
    Local<Float64Array> ar = Local<Float64Array>::Cast(args[2]);\
    \
    uint32_t count = ja->Length();\
    V8CHECK(ar->Length() != count, "the tow arrays must have the same length");\
    count--;\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    int* pja = (int*)malloc(ja->Length() * sizeof(int));\
    double* par = (double*)malloc(ar->Length() * sizeof(double));\
    \
    for (size_t i = 0; i < ja->Length(); i++) pja[i] = ja->Get(i)->Int32Value();\
    for (size_t i = 0; i < ar->Length(); i++) par[i] = ar->Get(i)->NumberValue();\
    \
    GLP_CATCH_RET(API(host->handle, args[0]->Int32Value(), count, pja, par);)\
    \
    free(pja);\
    free(par);\
}

#define GLP_BIND_VALUE_INT32_CALLBACK(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK((args.Length() < 1) || (args.Length() > 2), "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    try{\
        int row = args[0]->Int32Value();\
        int count = API(host->handle, row, NULL, NULL);\
        \
        if ((args.Length() == 2) && (args[1]->IsFunction())) {\
            int* idx = (int*)malloc((count + 1) * sizeof(int));\
            double* val = (double*)malloc((count + 1) * sizeof(double));\
            API(host->handle, row, idx, val);\
            Local<Int32Array> ja = Int32Array::New(ArrayBuffer::New(Isolate::GetCurrent(), sizeof(int) * (count+1)), 0, count + 1);\
            Local<Float64Array> ar = Float64Array::New(ArrayBuffer::New(Isolate::GetCurrent(), sizeof(double) * (count+1)), 0, count + 1);\
        	for (int i = 1; i <= count; i++) ja->Set((uint32_t)i, Int32::New(Isolate::GetCurrent(), idx[i]));\
        	for (int i = 1; i <= count; i++) ar->Set((uint32_t)i, Number::New(Isolate::GetCurrent(), val[i]));\
        	free(idx);\
        	free(val);\
            \
            Local<Function> cb = Local<Function>::Cast(args[1]);\
            const unsigned argc = 2;\
            Local<Value> argv[argc] = {ja, ar};\
            cb->Call(Isolate::GetCurrent()->GetCurrentContext()->Global(), argc, argv);\
            NanReturnValue(count);\
        }\
    } catch(...){\
        NanThrowError("abort"); \
    }\
}

#define GLP_BIND_VOID(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 0, "Wrong number of arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(API(host->handle);)\
}

#define GLP_BIND_VOID_INT32ARRAY(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32Array(), "Wrong arguments");\
    Local<Int32Array> num = Local<Int32Array>::Cast(args[0]);\
    \
    int count = num->Length();\
    V8CHECK(count <= 1, "Invalid Array size");\
    \
    int* idx = (int*)malloc(count * sizeof(int));\
    for (int i = 0; i < count; i++) idx[i] = num->Get(i)->Int32Value();\
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    count--;\
    GLP_CATCH_RET(API(host->handle, count, idx);)\
    free(idx);\
}

#define GLP_BIND_VALUE_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsString(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(NanReturnValue(API(host->handle, V8TOCSTRING(args[0])));)\
}

#define GLP_BIND_VOID_INT32_INT32(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32() || !args[1]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(API(host->handle, args[0]->Int32Value(), args[1]->Int32Value());)\
}

#define GLP_BIND_VALUE_INT32_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsInt32() || !args[1]->IsString(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(NanReturnValue(API(host->handle, args[0]->Int32Value(), V8TOCSTRING(args[1])));)\
}

#define GLP_BIND_VALUE_STR_INT32(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    V8CHECK(args.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!args[0]->IsString() || !args[1]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    \
    GLP_CATCH_RET(args.GetReturnValue().Set(API(host->handle, V8TOCSTRING(args[0]), args[1]->Int32Value()));)\
}

#define GLP_BIND_DELETE(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    NanScope();\
    \
    CLASS* obj = ObjectWrap::Unwrap<CLASS>(args.Holder());\
    V8CHECK(!obj->handle, "object already deleted");\
    \
    GLP_CATCH_RET(API(obj->handle);)\
    obj->handle = NULL;\
}

#define GLP_SET_FIELD_INT32(OBJ, KEY, VALUE)\
OBJ->Set(NanNew<String>(KEY), NanNew<Int32>(VALUE));

#define GLP_SET_FIELD_DOUBLE(OBJ, KEY, VALUE)\
OBJ->Set(NanNew<String>(KEY), NanNew<Number>(VALUE));


#endif
