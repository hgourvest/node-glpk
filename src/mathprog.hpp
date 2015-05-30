
#include <node.h>
#include <node_object_wrap.h>
#include "glpk/glpk.h"
#include "common.h"


namespace NodeGLPK {
    
    using namespace v8;
    
    class Mathprog : public node::ObjectWrap {
    public:
        static void Init(Handle<Object> exports){
            // Prepare constructor template
            Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
            tpl->SetClassName(NanNew<String>("Mathprog"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
 
            // prototypes
            NODE_SET_PROTOTYPE_METHOD(tpl, "readModel", ReadModel);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readData", ReadData);
            NODE_SET_PROTOTYPE_METHOD(tpl, "generate", Generate);
            NODE_SET_PROTOTYPE_METHOD(tpl, "delete", Delete);
            NODE_SET_PROTOTYPE_METHOD(tpl, "buildProb", BuildProb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "postsolve", Postsolve);
            
            NanAssignPersistent(constructor, tpl);
            exports->Set(NanNew<String>("Mathprog"), tpl->GetFunction());
        }
    private:
        explicit Mathprog(): node::ObjectWrap(){
            handle = glp_mpl_alloc_wksp();
        };
        ~Mathprog(){
            if(handle) glp_mpl_free_wksp(handle);
        };
        
        static NAN_METHOD(New){
            NanScope();
            
            V8CHECK(!args.IsConstructCall(), "Constructor Mathprog requires 'new'");
            
            GLP_CATCH_RET(
                Mathprog* obj = new Mathprog();
                obj->Wrap(args.This());
                      NanReturnValue(args.This());
            );
        }
        
        GLP_BIND_VALUE_STR_INT32(Mathprog, ReadModel, glp_mpl_read_model);
        
        GLP_BIND_VALUE_STR(Mathprog, ReadData, glp_mpl_read_data);
        
        static NAN_METHOD(Generate) {
            NanScope();
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");

            Mathprog* host = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!host->handle, "object deleted");
            GLP_CATCH_RET(
                if ((args.Length() == 1) && (!args[0]->IsString()))
                    NanReturnValue(glp_mpl_generate(host->handle, V8TOCSTRING(args[0])));
                else
                    NanReturnValue(glp_mpl_generate(host->handle, NULL));
            )
        }
        
        static NAN_METHOD(BuildProb){
            NanScope();
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsObject(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args[0]->ToObject());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            
            GLP_CATCH_RET(glp_mpl_build_prob(mp->handle, lp->handle);)
        }

        static NAN_METHOD(Postsolve){
            NanScope();
            
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!args[0]->IsObject() || !args[1]->IsInt32(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args[0]->ToObject());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            
            GLP_CATCH_RET(NanReturnValue(glp_mpl_postsolve(mp->handle, lp->handle, args[1]->Int32Value()));)
        }
        
        GLP_BIND_DELETE(Mathprog, Delete, glp_mpl_free_wksp);
        
        static Persistent<FunctionTemplate> constructor;
        glp_tran *handle;
    };
    
    Persistent<FunctionTemplate> Mathprog::constructor;
}

