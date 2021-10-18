
#include <node.h>
#include <node_object_wrap.h>
#include "glpk/glpk.h"
#include "common.h"


namespace NodeGLPK {
    
    using namespace v8;

    class Mathprog : public node::ObjectWrap {
    public:
        static void Init(Local<Object> exports){
            // Prepare constructor template
            Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
            tpl->SetClassName(Nan::New<String>("Mathprog").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
 
            // prototypes
            Nan::SetPrototypeMethod(tpl, "readModelSync", ReadModelSync);
            Nan::SetPrototypeMethod(tpl, "readModel", ReadModel);
            Nan::SetPrototypeMethod(tpl, "readDataSync", ReadDataSync);
            Nan::SetPrototypeMethod(tpl, "readData", ReadData);
            Nan::SetPrototypeMethod(tpl, "generateSync", GenerateSync);
            Nan::SetPrototypeMethod(tpl, "generate", Generate);
            Nan::SetPrototypeMethod(tpl, "delete", Delete);
            Nan::SetPrototypeMethod(tpl, "buildProbSync", BuildProbSync);
            Nan::SetPrototypeMethod(tpl, "buildProb", BuildProb);
            Nan::SetPrototypeMethod(tpl, "postsolveSync", PostsolveSync);
            Nan::SetPrototypeMethod(tpl, "postsolve", Postsolve);
            Nan::SetPrototypeMethod(tpl, "getLine", getLine);
            Nan::SetPrototypeMethod(tpl, "getLastError", getLastError);
            
            constructor.Reset(tpl);

            exports->Set(Nan::GetCurrentContext(), Nan::New<String>("Mathprog").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked()).Check();
        }
    private:
        explicit Mathprog(): node::ObjectWrap(){
            handle = glp_mpl_alloc_wksp();
            thread = false;
        };
        ~Mathprog(){
            if(handle) glp_mpl_free_wksp(handle);
        };
        
        static NAN_METHOD(New){
            V8CHECK(!info.IsConstructCall(), "Constructor Mathprog requires 'new'");
            
            GLP_CATCH_RET(
                Mathprog* obj = new Mathprog();
                obj->Wrap(info.This());
                      info.GetReturnValue().Set(info.This());
            );
        }
        
        
        class ReadModelWorker : public Nan::AsyncWorker {
        public:
            ReadModelWorker(Nan::Callback *callback, Mathprog *mp, char *file, int parm)
            : Nan::AsyncWorker(callback), mp(mp), file(file), parm(parm){
                
            }
            void WorkComplete() {
                mp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_mpl_read_model(mp->handle, file.c_str(), parm);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            virtual void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
            public:
                Mathprog *mp;
                std::string file;
                int ret, parm;
            };

        static NAN_METHOD(ReadModel) {
            V8CHECK(info.Length() != 3, "Wrong number of arguments");
            V8CHECK(!info[0]->IsString() || !info[1]->IsInt32() || !info[2]->IsFunction(), "Wrong arguments");
        
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());
            ReadModelWorker *worker = new ReadModelWorker(callback, mp, V8TOCSTRING(info[0]), info[1]->Int32Value(Nan::GetCurrentContext()).FromJust());
            mp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
  
        GLP_BIND_VALUE_STR_INT32(Mathprog, ReadModelSync, glp_mpl_read_model);
        
        class ReadDataWorker : public Nan::AsyncWorker {
        public:
            ReadDataWorker(Nan::Callback *callback, Mathprog *mp, char *file)
            : Nan::AsyncWorker(callback), mp(mp), file(file){
                
            }
            void WorkComplete() {
                mp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_mpl_read_data(mp->handle, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            virtual void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
        public:
            Mathprog *mp;
            std::string file;
            int ret;
        };
        
        static NAN_METHOD(ReadData) {
            V8CHECK(info.Length() != 2, "Wrong number of arguments");
            V8CHECK(!info[0]->IsString() || !info[1]->IsFunction(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            ReadDataWorker *worker = new ReadDataWorker(callback, mp, V8TOCSTRING(info[0]));
            mp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        GLP_BIND_VALUE_STR(Mathprog, ReadDataSync, glp_mpl_read_data);
        
        class GenerateWorker : public Nan::AsyncWorker {
        public:
            GenerateWorker(Nan::Callback *callback, Mathprog *mp, char *file)
            : Nan::AsyncWorker(callback), mp(mp){
                if (file) this->file = file;
            }
            void WorkComplete() {
                mp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    if (file.length() > 0)
                        ret = glp_mpl_generate(mp->handle, file.c_str());
                    else
                        ret = glp_mpl_generate(mp->handle, NULL);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            virtual void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
            
        public:
            Mathprog *mp;
            std::string file;
            int ret;
        };
        
        static NAN_METHOD(Generate) {
            V8CHECK(info.Length() != 2, "Wrong number of arguments");
            V8CHECK(!(info[0]->IsString() || info[0]->IsNull()) || !info[1]->IsFunction(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            GenerateWorker *worker = NULL;
            if (info[0]->IsString())
                worker = new GenerateWorker(callback, mp, V8TOCSTRING(info[0]));
            else
                worker = new GenerateWorker(callback, mp, NULL);
            mp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static NAN_METHOD(GenerateSync) {
            V8CHECK(info.Length() > 1, "Wrong number of arguments");

            Mathprog* host = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!host->handle, "object deleted");
            V8CHECK(host->thread, "an async operation is inprogress");
            
            GLP_CATCH_RET(
                if ((info.Length() == 1) && (!info[0]->IsString()))
                    info.GetReturnValue().Set(glp_mpl_generate(host->handle, V8TOCSTRING(info[0])));
                else
                    info.GetReturnValue().Set(glp_mpl_generate(host->handle, NULL));
            )
        }
        
        class BuildProbWorker : public Nan::AsyncWorker {
        public:
            BuildProbWorker(Nan::Callback *callback, Mathprog *mp, Problem *lp)
            : Nan::AsyncWorker(callback), mp(mp), lp(lp){
                
            }
            void WorkComplete() {
                mp->thread = false;
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    glp_mpl_build_prob(mp->handle, lp->handle);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
        public:
            Mathprog *mp;
            Problem *lp;
        };
        
        static NAN_METHOD(BuildProb) {
            V8CHECK(info.Length() != 2, "Wrong number of arguments");
            V8CHECK(!info[0]->IsObject() || !info[1]->IsFunction(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(Nan::To<Object>(info[0]).ToLocalChecked());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            BuildProbWorker *worker = new BuildProbWorker(callback, mp, lp);
            mp->thread = true;
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static NAN_METHOD(BuildProbSync){
            V8CHECK(info.Length() != 1, "Wrong number of arguments");
            V8CHECK(!info[0]->IsObject(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(Nan::To<Object>(info[0]).ToLocalChecked());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            
            GLP_CATCH_RET(glp_mpl_build_prob(mp->handle, lp->handle);)
        }

        class PostsolveWorker : public Nan::AsyncWorker {
        public:
            PostsolveWorker(Nan::Callback *callback, Mathprog *mp, Problem *lp, int parm)
            : Nan::AsyncWorker(callback), mp(mp), lp(lp), parm(parm){
                
            }
            void WorkComplete() {
                mp->thread = false;
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_mpl_postsolve(mp->handle, lp->handle, parm);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            virtual void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
        public:
            Mathprog *mp;
            Problem *lp;
            int parm, ret;
        };
        
        static NAN_METHOD(Postsolve) {
            V8CHECK(info.Length() != 3, "Wrong number of arguments");
            V8CHECK(!info[0]->IsObject() || !info[1]->IsInt32() || !info[2]->IsFunction(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(Nan::To<Object>(info[0]).ToLocalChecked());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());
            PostsolveWorker *worker = new PostsolveWorker(callback, mp, lp, info[1]->Int32Value(Nan::GetCurrentContext()).FromJust());
            mp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static NAN_METHOD(PostsolveSync){
            V8CHECK(info.Length() != 2, "Wrong number of arguments");
            V8CHECK(!info[0]->IsObject() || !info[1]->IsInt32(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(Nan::To<Object>(info[0]).ToLocalChecked());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            
            GLP_CATCH_RET(info.GetReturnValue().Set(glp_mpl_postsolve(mp->handle, lp->handle, info[1]->Int32Value(Nan::GetCurrentContext()).FromJust()));)
        }
        
        static NAN_METHOD(getLine){
            V8CHECK(info.Length() != 0, "Wrong number of arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            info.GetReturnValue().Set(*(int*)mp->handle);
        }
        
        static NAN_METHOD(getLastError){
            V8CHECK(info.Length() != 0, "Wrong number of arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(info.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            char * msg = glp_mpl_getlasterror(mp->handle);
            if (msg)
                info.GetReturnValue().Set(Nan::New<String>(msg).ToLocalChecked());
            else
                info.GetReturnValue().Set(Nan::Null());
        }
        
        GLP_BIND_DELETE(Mathprog, Delete, glp_mpl_free_wksp);
        
        static Nan::Persistent<FunctionTemplate> constructor;
        glp_tran *handle;
        bool thread;
    };
    
    Nan::Persistent<FunctionTemplate> Mathprog::constructor;
}
