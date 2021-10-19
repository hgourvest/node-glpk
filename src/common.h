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

#define V8TOCSTRING(S) (*Nan::Utf8String(S))

#define V8CHECK(T, E) if (T) { \
    Nan::ThrowTypeError(E);\
    return; \
 }

#define V8CHECKBOOL(T, E) if (T) { \
    Nan::ThrowTypeError(E);\
    return false; \
}

//catch (std::string s){Nan::ThrowError(s.c_str());return;}

#define GLP_CATCH_RET(X) try{X}catch (std::string s) {Nan::ThrowError(s.c_str());return;}

#define GLP_CATCH(X) try{X}catch (std::string s) {Nan::ThrowError(s.c_str());}

#define GLP_DEFINE_CONSTANT(target, constant, name)\
do {\
Isolate* isolate = Isolate::GetCurrent();\
Local<String> constant_name =\
String::NewFromUtf8(isolate, #name, NewStringType::kNormal).ToLocalChecked();\
Local<Number> constant_value =\
Number::New(isolate, static_cast<double>(constant));\
PropertyAttribute constant_attributes =\
static_cast<PropertyAttribute>(ReadOnly | DontDelete);\
Nan::DefineOwnProperty(target, constant_name, constant_value, constant_attributes);\
}\
while (0)

#define GLP_BIND_VOID_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsString(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(API(host->handle, V8TOCSTRING(info[0]));)\
}

#define GLP_BIND_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(const char* name = API(host->handle);\
    if (name)\
        info.GetReturnValue().Set(Nan::New<String>(name).ToLocalChecked());\
    else\
        info.GetReturnValue().Set(Nan::New<String>("").ToLocalChecked());)\
}

#define GLP_BIND_VOID_INT32(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(API(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust());)\
}

#define GLP_BIND_VALUE(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(info.GetReturnValue().Set(API(host->handle));)\
}

#define GLP_BIND_VOID_INT32_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32() || !info[1]->IsString(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(API(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), V8TOCSTRING(info[1]));)\
}

#define GLP_BIND_STR_INT32(CLASS, NAME, API);\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(const char* name = API(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust());\
    if (name)\
        info.GetReturnValue().Set(Nan::New<String>(name).ToLocalChecked());\
    else\
        info.GetReturnValue().Set(Nan::New<String>("").ToLocalChecked());)\
}

#define GLP_BIND_VOID_INT32_INT32_DOUBLE_DOUBLE(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 4, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32() || !info[1]->IsInt32()\
            || !info[2]->IsNumber() || !info[3]->IsNumber(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(API(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), info[1]->Int32Value(Nan::GetCurrentContext()).FromJust(),\
                     info[2]->NumberValue(Nan::GetCurrentContext()).FromJust(), info[3]->NumberValue(Nan::GetCurrentContext()).FromJust());)\
}

#define GLP_BIND_VOID_INT32_DOUBLE(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32() || !info[1]->IsNumber(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(API(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());)\
}

#define GLP_BIND_VALUE_INT32(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(info.GetReturnValue().Set(API(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust()));)\
}

#define GLP_BIND_VOID_INT32_INT32ARRAY_FLOAT64ARRAY(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 3, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsUint32() || !info[1]->IsInt32Array() || !info[2]->IsFloat64Array(), "Wrong arguments");\
    \
    Local<Int32Array> ja = Local<Int32Array>::Cast(info[1]);\
    Local<Float64Array> ar = Local<Float64Array>::Cast(info[2]);\
    \
    uint32_t count = ja->Length();\
    V8CHECK(ar->Length() != count, "the tow arrays must have the same length");\
    count--;\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    int* pja = (int*)malloc(ja->Length() * sizeof(int));\
    double* par = (double*)malloc(ar->Length() * sizeof(double));\
    \
    for (size_t i = 0; i < ja->Length(); i++) pja[i] = ja->Get(i)->Int32Value(Nan::GetCurrentContext()).FromJust();\
    for (size_t i = 0; i < ar->Length(); i++) par[i] = ar->Get(i)->NumberValue(Nan::GetCurrentContext()).FromJust();\
    \
    GLP_CATCH_RET(API(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), count, pja, par);)\
    \
    free(pja);\
    free(par);\
}

#define GLP_BIND_VALUE_INT32_CALLBACK(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK((info.Length() < 1) || (info.Length() > 2), "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    try{\
        int row = info[0]->Int32Value(Nan::GetCurrentContext()).FromJust();\
        int count = API(host->handle, row, NULL, NULL);\
        \
        if ((info.Length() == 2) && (info[1]->IsFunction())) {\
            int* idx = (int*)malloc((count + 1) * sizeof(int));\
            double* val = (double*)malloc((count + 1) * sizeof(double));\
            API(host->handle, row, idx, val);\
            Local<Int32Array> ja = Int32Array::New(ArrayBuffer::New(Isolate::GetCurrent(), sizeof(int) * (count+1)), 0, count + 1);\
            Local<Float64Array> ar = Float64Array::New(ArrayBuffer::New(Isolate::GetCurrent(), sizeof(double) * (count+1)), 0, count + 1);\
        	for (int i = 1; i <= count; i++) ja->Set(Nan::GetCurrentContext(), (uint32_t)i, Int32::New(Isolate::GetCurrent(), idx[i])).Check();\
        	for (int i = 1; i <= count; i++) ar->Set(Nan::GetCurrentContext(), (uint32_t)i, Number::New(Isolate::GetCurrent(), val[i])).Check();\
        	free(idx);\
        	free(val);\
            \
            Local<Function> cb = Local<Function>::Cast(info[1]);\
            const unsigned argc = 2;\
            Local<Value> argv[argc] = {ja, ar};\
            Nan::Call(cb, Isolate::GetCurrent()->GetCurrentContext()->Global(), argc, argv);\
            info.GetReturnValue().Set(count);\
        }\
    } catch (std::string s) {\
        Nan::ThrowError(s.c_str());\
    }\
}

#define GLP_BIND_VOID(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 0, "Wrong number of arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(API(host->handle);)\
}

#define GLP_BIND_VOID_INT32ARRAY(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32Array(), "Wrong arguments");\
    Local<Int32Array> num = Local<Int32Array>::Cast(info[0]);\
    \
    int count = num->Length();\
    V8CHECK(count <= 1, "Invalid Array size");\
    \
    int* idx = (int*)malloc(count * sizeof(int));\
    for (int i = 0; i < count; i++) idx[i] = num->Get(i)->Int32Value(Nan::GetCurrentContext()).FromJust();\
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    count--;\
    GLP_CATCH_RET(API(host->handle, count, idx);)\
    free(idx);\
}

#define GLP_BIND_VALUE_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsString(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(info.GetReturnValue().Set(API(host->handle, V8TOCSTRING(info[0])));)\
}

#define GLP_BIND_VOID_INT32_INT32(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32() || !info[1]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(API(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), info[1]->Int32Value(Nan::GetCurrentContext()).FromJust());)\
}

#define GLP_BIND_VALUE_INT32_STR(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32() || !info[1]->IsString(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(info.GetReturnValue().Set(API(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), V8TOCSTRING(info[1])));)\
}

#define GLP_BIND_VALUE_STR_INT32(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsString() || !info[1]->IsInt32(), "Wrong arguments");\
    \
    CLASS* host = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!host->handle, "object deleted");\
    V8CHECK(host->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(info.GetReturnValue().Set(API(host->handle, V8TOCSTRING(info[0]), info[1]->Int32Value(Nan::GetCurrentContext()).FromJust()));)\
}

#define GLP_BIND_DELETE(CLASS, NAME, API)\
static NAN_METHOD(NAME) {\
    CLASS* obj = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!obj->handle, "object already deleted");\
    V8CHECK(obj->thread, "an async operation is inprogress")\
    \
    GLP_CATCH_RET(API(obj->handle);)\
    obj->handle = NULL;\
}

#define GLP_SET_FIELD_INT32(OBJ, KEY, VALUE)\
OBJ->Set(Nan::GetCurrentContext(), Nan::New<String>(KEY).ToLocalChecked(), Nan::New<Int32>(VALUE)).Check();

#define GLP_SET_FIELD_DOUBLE(OBJ, KEY, VALUE)\
OBJ->Set(Nan::GetCurrentContext(), Nan::New<String>(KEY).ToLocalChecked(), Nan::New<Number>(VALUE)).Check();


#define GLP_ASYNC_INT32_STR(CLASS, NAME, API)\
class NAME##Worker : public Nan::AsyncWorker {\
public:\
NAME##Worker(Nan::Callback *callback, CLASS *lp, std::string file)\
: Nan::AsyncWorker(callback), lp(lp), file(file){\
    \
}\
\
void Execute () {\
    try {\
        ret = API(lp->handle, file.c_str());\
    } catch (std::string s) {\
        Nan::ThrowError(s.c_str());\
    }\
}\
void WorkComplete() {\
    lp->thread = false;\
    Nan::AsyncWorker::WorkComplete();\
}\
virtual void HandleOKCallback() {\
    Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};\
    callback->Call(2, info, async_resource);\
}\
\
public:\
    int ret;\
    CLASS *lp;\
    std::string file;\
};\
\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 2, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsString() || !info[1]->IsFunction(), "Wrong arguments");\
    \
    CLASS* lp = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!lp->handle, "object deleted");\
    V8CHECK(lp->thread, "an async operation is inprogress")\
    \
    Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());\
    NAME##Worker *worker = new NAME##Worker(callback, lp, V8TOCSTRING(info[0]));\
    lp->thread = true;\
    Nan::AsyncQueueWorker(worker);\
}\

#define GLP_ASYNC_INT32_INT32_STR(CLASS, NAME, API)\
class NAME##Worker : public Nan::AsyncWorker {\
public:\
    NAME##Worker(Nan::Callback *callback, CLASS *lp, int flags, std::string file)\
    : Nan::AsyncWorker(callback), flags(flags), lp(lp), file(file){\
        \
    }\
    void WorkComplete() {\
        lp->thread = false;\
        Nan::AsyncWorker::WorkComplete();\
    }\
    void Execute () {\
        try {\
            ret = API(lp->handle, flags, file.c_str());\
        } catch (std::string s) {\
            Nan::ThrowError(s.c_str());\
        }\
    }\
    \
    virtual void HandleOKCallback() {\
        Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};\
        callback->Call(2, info, async_resource);\
    }\
    \
public:\
    int ret, flags;\
    CLASS *lp;\
    std::string file;\
};\
\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 3, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsInt32() || !info[1]->IsString() || !info[2]->IsFunction(), "Wrong arguments");\
    \
    CLASS* lp = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!lp->handle, "object deleted");\
    V8CHECK(lp->thread, "an async operation is inprogress")\
    \
    Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());\
    NAME##Worker *worker = new NAME##Worker(callback, lp, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), V8TOCSTRING(info[1]));\
    lp->thread = true;\
    Nan::AsyncQueueWorker(worker);\
}\

#define GLP_ASYNC_VOID(CLASS, NAME, API)\
class NAME##Worker : public Nan::AsyncWorker {\
public:\
    NAME##Worker(Nan::Callback *callback, CLASS *lp)\
    : Nan::AsyncWorker(callback), lp(lp) {\
    }\
    void WorkComplete() {\
        lp->thread = false;\
        Nan::AsyncWorker::WorkComplete();\
    }\
    void Execute () {\
        try {\
            API(lp->handle);\
        } catch (std::string s) {\
            Nan::ThrowError(s.c_str());\
        }\
    }\
public:\
    CLASS *lp;\
};\
\
static NAN_METHOD(NAME) {\
    V8CHECK(info.Length() != 1, "Wrong number of arguments");\
    V8CHECK(!info[0]->IsFunction(), "Wrong arguments");\
    \
    CLASS* lp = ObjectWrap::Unwrap<CLASS>(info.Holder());\
    V8CHECK(!lp->handle, "object deleted");\
    V8CHECK(lp->thread, "an async operation is inprogress")\
    \
    Nan::Callback *callback = new Nan::Callback(info[0].As<Function>());\
    NAME##Worker *worker = new NAME##Worker(callback, lp);\
    lp->thread = true;\
    Nan::AsyncQueueWorker(worker);\
}\


#endif
