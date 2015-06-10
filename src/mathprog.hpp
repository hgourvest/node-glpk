
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
            NODE_SET_PROTOTYPE_METHOD(tpl, "readModelSync", ReadModelSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readModel", ReadModel);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readDataSync", ReadDataSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readData", ReadData);
            NODE_SET_PROTOTYPE_METHOD(tpl, "generateSync", GenerateSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "generate", Generate);
            NODE_SET_PROTOTYPE_METHOD(tpl, "delete", Delete);
            NODE_SET_PROTOTYPE_METHOD(tpl, "buildProbSync", BuildProbSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "buildProb", BuildProb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "postsolveSync", PostsolveSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "postsolve", Postsolve);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getLine", getLine);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getLastError", getLastError);
            
            NanAssignPersistent(constructor, tpl);
            exports->Set(NanNew<String>("Mathprog"), tpl->GetFunction());
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
            NanScope();
            
            V8CHECK(!args.IsConstructCall(), "Constructor Mathprog requires 'new'");
            
            GLP_CATCH_RET(
                Mathprog* obj = new Mathprog();
                obj->Wrap(args.This());
                      NanReturnValue(args.This());
            );
        }
        
        
        class ReadModelWorker : public NanAsyncWorker {
        public:
            ReadModelWorker(NanCallback *callback, Mathprog *mp, char *file, int parm)
            : NanAsyncWorker(callback), mp(mp), file(file), parm(parm){
                
            }
            void WorkComplete() {
                mp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_mpl_read_model(mp->handle, file.c_str(), parm);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            virtual void HandleOKCallback() {
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
            public:
                Mathprog *mp;
                std::string file;
                int ret, parm;
            };

        static NAN_METHOD(ReadModel) {
            NanScope();
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!args[0]->IsString() || !args[1]->IsInt32() || !args[2]->IsFunction(), "Wrong arguments");
        
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[2].As<Function>());
            ReadModelWorker *worker = new ReadModelWorker(callback, mp, V8TOCSTRING(args[0]), args[1]->Int32Value());
            mp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
  
        GLP_BIND_VALUE_STR_INT32(Mathprog, ReadModelSync, glp_mpl_read_model);
        
        class ReadDataWorker : public NanAsyncWorker {
        public:
            ReadDataWorker(NanCallback *callback, Mathprog *mp, char *file)
            : NanAsyncWorker(callback), mp(mp), file(file){
                
            }
            void WorkComplete() {
                mp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_mpl_read_data(mp->handle, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            virtual void HandleOKCallback() {
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
        public:
            Mathprog *mp;
            std::string file;
            int ret;
        };
        
        static NAN_METHOD(ReadData) {
            NanScope();
            
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!args[0]->IsString() || !args[1]->IsFunction(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            ReadDataWorker *worker = new ReadDataWorker(callback, mp, V8TOCSTRING(args[0]));
            mp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        GLP_BIND_VALUE_STR(Mathprog, ReadDataSync, glp_mpl_read_data);
        
        class GenerateWorker : public NanAsyncWorker {
        public:
            GenerateWorker(NanCallback *callback, Mathprog *mp, char *file)
            : NanAsyncWorker(callback), mp(mp){
                if (file) this->file = file;
            }
            void WorkComplete() {
                mp->thread = false;
                NanAsyncWorker::WorkComplete();
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
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
            
        public:
            Mathprog *mp;
            std::string file;
            int ret;
        };
        
        static NAN_METHOD(Generate) {
            NanScope();
            
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsString() || args[0]->IsNull()) || !args[1]->IsFunction(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            GenerateWorker *worker = NULL;
            if (args[0]->IsString())
                worker = new GenerateWorker(callback, mp, V8TOCSTRING(args[0]));
            else
                worker = new GenerateWorker(callback, mp, NULL);
            mp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static NAN_METHOD(GenerateSync) {
            NanScope();
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");

            Mathprog* host = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!host->handle, "object deleted");
            V8CHECK(host->thread, "an async operation is inprogress");
            
            GLP_CATCH_RET(
                if ((args.Length() == 1) && (!args[0]->IsString()))
                    NanReturnValue(glp_mpl_generate(host->handle, V8TOCSTRING(args[0])));
                else
                    NanReturnValue(glp_mpl_generate(host->handle, NULL));
            )
        }
        
        class BuildProbWorker : public NanAsyncWorker {
        public:
            BuildProbWorker(NanCallback *callback, Mathprog *mp, Problem *lp)
            : NanAsyncWorker(callback), mp(mp), lp(lp){
                
            }
            void WorkComplete() {
                mp->thread = false;
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
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
            NanScope();
            
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!args[0]->IsObject() || !args[1]->IsFunction(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args[0]->ToObject());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            BuildProbWorker *worker = new BuildProbWorker(callback, mp, lp);
            mp->thread = true;
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static NAN_METHOD(BuildProbSync){
            NanScope();
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsObject(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args[0]->ToObject());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            
            GLP_CATCH_RET(glp_mpl_build_prob(mp->handle, lp->handle);)
        }

        class PostsolveWorker : public NanAsyncWorker {
        public:
            PostsolveWorker(NanCallback *callback, Mathprog *mp, Problem *lp, int parm)
            : NanAsyncWorker(callback), mp(mp), lp(lp), parm(parm){
                
            }
            void WorkComplete() {
                mp->thread = false;
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_mpl_postsolve(mp->handle, lp->handle, parm);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            virtual void HandleOKCallback() {
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
        public:
            Mathprog *mp;
            Problem *lp;
            int parm, ret;
        };
        
        static NAN_METHOD(Postsolve) {
            NanScope();
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!args[0]->IsObject() || !args[1]->IsInt32() || !args[2]->IsFunction(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args[0]->ToObject());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[2].As<Function>());
            PostsolveWorker *worker = new PostsolveWorker(callback, mp, lp, args[1]->Int32Value());
            mp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static NAN_METHOD(PostsolveSync){
            NanScope();
            
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!args[0]->IsObject() || !args[1]->IsInt32(), "Wrong arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args[0]->ToObject());
            V8CHECK(!lp || !lp->handle, "invalid problem");
            
            GLP_CATCH_RET(NanReturnValue(glp_mpl_postsolve(mp->handle, lp->handle, args[1]->Int32Value()));)
        }
        
        static NAN_METHOD(getLine){
            NanScope();
            
            V8CHECK(args.Length() != 0, "Wrong number of arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            
            NanReturnValue(*(int*)mp->handle);
        }
        
        static NAN_METHOD(getLastError){
            NanScope();
            
            V8CHECK(args.Length() != 0, "Wrong number of arguments");
            
            Mathprog* mp = ObjectWrap::Unwrap<Mathprog>(args.Holder());
            V8CHECK(!mp->handle, "object deleted");
            V8CHECK(mp->thread, "an async operation is inprogress");
            char * msg = glp_mpl_getlasterror(mp->handle);
            if (msg)
                NanReturnValue(NanNew<String>(msg));
            else
                NanReturnValue(NanNull());
        }
        
        GLP_BIND_DELETE(Mathprog, Delete, glp_mpl_free_wksp);
        
        static Persistent<FunctionTemplate> constructor;
        glp_tran *handle;
        bool thread;
    };
    
    Persistent<FunctionTemplate> Mathprog::constructor;
}