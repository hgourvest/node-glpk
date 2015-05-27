
#include <node.h>
#include <node_object_wrap.h>
#include "glpk/glpk.h"
#include "common.h"

namespace NodeGLPK {
    
    using namespace v8;
    
    class Mathprog : public node::ObjectWrap {
    public:
        static void Init(Handle<Object> exports){
            Isolate* isolate = Isolate::GetCurrent();
            
            // Prepare constructor template
            Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
            tpl->SetClassName(String::NewFromUtf8(isolate, "Mathprog"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            
            // prototypes
            NODE_SET_PROTOTYPE_METHOD(tpl, "readModel", ReadModel);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readData", ReadData);
            NODE_SET_PROTOTYPE_METHOD(tpl, "generate", Generate);
            NODE_SET_PROTOTYPE_METHOD(tpl, "delete", Delete);
            NODE_SET_PROTOTYPE_METHOD(tpl, "buildProb", BuildProb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "postsolve", Postsolve);
            
            constructor.Reset(isolate, tpl->GetFunction());
            exports->Set(String::NewFromUtf8(isolate, "Mathprog"), tpl->GetFunction());
        }
    private:
        explicit Mathprog(){
            handle = glp_mpl_alloc_wksp();
        };
        ~Mathprog(){
            if(handle) glp_mpl_free_wksp(handle);
        };
        
        static void New(const FunctionCallbackInfo<Value>& args){
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(!args.IsConstructCall(), "Constructor Mathprog requires 'new'");
            
            Mathprog* obj = new Mathprog();
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        }
        
        GLP_BIND_VALUE_STR_INT32(Mathprog, ReadModel, glp_mpl_read_model);
        
        GLP_BIND_VALUE_STR(Mathprog, ReadData, glp_mpl_read_data);
        
        //GLP_BIND_VALUE_STR(Mathprog, Generate, glp_mpl_generate);
        
        static void Generate(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");

            Mathprog* host = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!host->handle, "object deleted");

            if ((args.Length() == 1) && (!args[0]->IsString()))
              args.GetReturnValue().Set(glp_mpl_generate(host->handle, V8TOCSTRING(args[0])));
            else
              args.GetReturnValue().Set(glp_mpl_generate(host->handle, NULL));
        }
        
        static void BuildProb(const FunctionCallbackInfo<Value>& args){
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsObject(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args[0]->ToObject());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            
            glp_mpl_build_prob(mp->handle, lp->handle);
        }

        static void Postsolve(const FunctionCallbackInfo<Value>& args){
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!args[0]->IsObject() || !args[1]->IsInt32(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args[0]->ToObject());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            
            args.GetReturnValue().Set(glp_mpl_postsolve(mp->handle, lp->handle, args[1]->Int32Value()));
        }
        
        GLP_BIND_DELETE(Mathprog, Delete, glp_mpl_free_wksp);
        
        static Persistent<Function> constructor;
        glp_tran *handle;
    };
    
    Persistent<Function> Mathprog::constructor;
}

