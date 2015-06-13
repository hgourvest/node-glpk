#include <node.h>
#include <node_object_wrap.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "glpk/glpk.h"
#include "common.h"

namespace NodeGLPK {
    
    using namespace v8;
    
    class Problem : public node::ObjectWrap {
    public:
        static void Init(Handle<Object> exports){
            // Prepare constructor template
            Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
            tpl->SetClassName(NanNew<String>("Problem"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            
            // Prototype
            NODE_SET_PROTOTYPE_METHOD(tpl, "setProbName", SetProbName);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getProbName", GetProbName);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setObjDir", SetObjDir);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getObjDir", GetObjDir);
            NODE_SET_PROTOTYPE_METHOD(tpl, "addRows", AddRows);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setRowName", SetRowName);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRowName", GetRowName);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setRowBnds", SetRowBnds);
            NODE_SET_PROTOTYPE_METHOD(tpl, "addCols", AddCols);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setColName", SetColName);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColName", GetColName);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setColBnds", SetColBnds);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setObjCoef", SetObjCoef);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getObjCoef", GetObjCoef);
            NODE_SET_PROTOTYPE_METHOD(tpl, "loadMatrix", LoadMatrix);
            NODE_SET_PROTOTYPE_METHOD(tpl, "simplexSync", SimplexSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "simplex", Simplex);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getObjVal", GetObjVal);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColPrim", GetColPrim);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setObjName", SetObjName);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getObjName", GetObjName);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setMatRow", SetMatRow);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getMatRow", GetMatRow);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setMatCol", SetMatCol);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getMatCol", GetMatCol);
            NODE_SET_PROTOTYPE_METHOD(tpl, "sortMatrix", SortMatrix);
            NODE_SET_PROTOTYPE_METHOD(tpl, "delRows", DelRows);
            NODE_SET_PROTOTYPE_METHOD(tpl, "delCols", DelCols);
            NODE_SET_PROTOTYPE_METHOD(tpl, "erase", Erase);
            NODE_SET_PROTOTYPE_METHOD(tpl, "delete", Delete);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getNumRows", GetNumRows);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getNumCols", GetNumCols);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRowType", GetRowType);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRowLb", GetRowLb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRowUb", GetRowUb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColType", GetColType);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColLb", GetColLb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColUb", GetColUb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getNumNz", GetNumNz);
            NODE_SET_PROTOTYPE_METHOD(tpl, "createIndex", CreateIndex);
            NODE_SET_PROTOTYPE_METHOD(tpl, "findRow", FindRow);
            NODE_SET_PROTOTYPE_METHOD(tpl, "findCol", FindCol);
            NODE_SET_PROTOTYPE_METHOD(tpl, "deleteIndex", DeleteIndex);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setRii", SetRii);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setSjj", SetSjj);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRii", GetRii);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getSjj", GetSjj);
            NODE_SET_PROTOTYPE_METHOD(tpl, "scaleSync", ScaleSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "scale", Scale);
            NODE_SET_PROTOTYPE_METHOD(tpl, "unscaleSync", UnscaleSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "unscale", Unscale);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setRowStat", SetRowStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setColStat", SetColStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRowStat", GetRowStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColStat", GetColStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "stdBasis", StdBasis);
            NODE_SET_PROTOTYPE_METHOD(tpl, "advBasis", AdvBasis);
            NODE_SET_PROTOTYPE_METHOD(tpl, "cpxBasis", CpxBasis);
            NODE_SET_PROTOTYPE_METHOD(tpl, "exactSync", ExactSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "exact", Exact);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getStatus", GetStatus);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getPrimStat", GetPrimStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getDualStat", GetDualStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRowPrim", GetRowPrim);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRowDual", GetRowDual);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColDual", GetColDual);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getUnbndRay", GetUnbndRay);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getItCnt", GetItCnt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setItCnt", SetItCnt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "interiorSync", InteriorSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "interior", Interior);
            NODE_SET_PROTOTYPE_METHOD(tpl, "iptStatus", IptStatus);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readMpsSync", ReadMpsSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readMps", ReadMps);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeMpsSync", WriteMpsSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeMps", WriteMps);
            NODE_SET_PROTOTYPE_METHOD(tpl, "iptObjVal", IptObjVal);
            NODE_SET_PROTOTYPE_METHOD(tpl, "iptRowPrim", IptRowPrim);
            NODE_SET_PROTOTYPE_METHOD(tpl, "iptRowDual", IptRowDual);
            NODE_SET_PROTOTYPE_METHOD(tpl, "iptColPrim", IptColPrim);
            NODE_SET_PROTOTYPE_METHOD(tpl, "iptColDual", IptColDual);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setColKind", SetColKind);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColKind", GetColKind);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getNumInt", GetNumInt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getNumBin", GetNumBin);
            NODE_SET_PROTOTYPE_METHOD(tpl, "intoptSync", IntoptSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "intopt", Intopt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readProbSync", ReadProbSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readProb", ReadProb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeProbSync", WriteProbSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeProb", WriteProb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readLp", ReadLp);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readLpSync", ReadLpSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeLpSync", WriteLpSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeLp", WriteLp);
            NODE_SET_PROTOTYPE_METHOD(tpl, "mipStatus", MipStatus);
            NODE_SET_PROTOTYPE_METHOD(tpl, "mipObjVal", MipObjVal);
            NODE_SET_PROTOTYPE_METHOD(tpl, "mipRowVal", MipRowVal);
            NODE_SET_PROTOTYPE_METHOD(tpl, "mipColVal", MipColVal);
            NODE_SET_PROTOTYPE_METHOD(tpl, "checkKkt", CheckKkt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printSolSync", PrintSolSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printSol", PrintSol);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readSolSync", ReadSolSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readSol", ReadSol);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeSolSync", WriteSolSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeSol", WriteSol);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printRangesSync", PrintRangesSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printRanges", PrintRanges);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printIptSync", PrintIptSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printIpt", PrintIpt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readIptSync", ReadIptSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readIpt", ReadIpt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeIptSync", WriteIptSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeIpt", WriteIpt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printMipSync", PrintMipSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printMip", PrintMip);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readMipSync", ReadMipSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readMip", ReadMip);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeMipSync", WriteMipSync);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeMip", WriteMip);
            NODE_SET_PROTOTYPE_METHOD(tpl, "bfExists", BfExists);
            NODE_SET_PROTOTYPE_METHOD(tpl, "factorize", Factorize);
            NODE_SET_PROTOTYPE_METHOD(tpl, "bfUpdated", BfUpdated);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getBfcp", GetBfcp);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setBfcp", SetBfcp);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getBhead", GetBhead);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRowBind", GetRowBind);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColBind", GetColBind);
            NODE_SET_PROTOTYPE_METHOD(tpl, "warmUp", WarmUp);
            
            NanAssignPersistent(constructor, tpl);
            exports->Set(NanNew<String>("Problem"), tpl->GetFunction());
        }
        
        static bool SmcpInit(glp_smcp* scmp, Local<Value> value){
            if (!value->IsObject()) return false;
            Local<Object> obj = value->ToObject();
            Local<Array> props = obj->GetPropertyNames();
            for(uint32_t i = 0; i < props->Length(); i++){
                Local<Value> key = props->Get(i);
                Local<Value> val = obj->Get(key);
                std::string keystr = std::string(V8TOCSTRING(key));
                if (keystr == "msgLev"){
                    V8CHECKBOOL(!val->IsInt32(), "msgLev: should be int32");
                    scmp->msg_lev = val->Int32Value();
                } else if (keystr == "meth"){
                    V8CHECKBOOL(!val->IsInt32(), "meth: should be int32");
                    scmp->meth = val->Int32Value();
                } else if (keystr == "pricing"){
                    V8CHECKBOOL(!val->IsInt32(), "pricing: should be int32");
                    scmp->pricing = val->Int32Value();
                } else if (keystr == "rTest"){
                    V8CHECKBOOL(!val->IsInt32(), "rTest: should be int32");
                    scmp->r_test = val->Int32Value();
                } else if (keystr == "tolBnd"){
                    V8CHECKBOOL(!val->IsNumber(), "tolBnd: should be a Number");
                    scmp->tol_bnd = val->NumberValue();
                } else if (keystr == "tolDj"){
                    V8CHECKBOOL(!val->IsNumber(), "tolDj: should be a Number");
                    scmp->tol_dj = val->NumberValue();
                } else if (keystr == "tolPiv"){
                    V8CHECKBOOL(!val->IsNumber(), "tolPiv: should be a Number");
                    scmp->tol_piv = val->NumberValue();
                } else if (keystr == "objLl"){
                    V8CHECKBOOL(!val->IsNumber(), "objLl: should be a Number");
                    scmp->obj_ll = val->NumberValue();
                } else if (keystr == "objUl"){
                    V8CHECKBOOL(!val->IsNumber(), "objUl: should be a Number");
                    scmp->obj_ul = val->NumberValue();
                } else if (keystr == "itLim"){
                    V8CHECKBOOL(!val->IsInt32(), "itLim: should be int32");
                    scmp->it_lim = val->Int32Value();
                } else if (keystr == "tmLim"){
                    V8CHECKBOOL(!val->IsInt32(), "tmLim: should be int32");
                    scmp->tm_lim = val->Int32Value();
                } else if (keystr == "outFrq"){
                    V8CHECKBOOL(!val->IsInt32(), "outFrq: should be int32");
                    scmp->out_frq = val->Int32Value();
                } else if (keystr == "outDly"){
                    V8CHECKBOOL(!val->IsInt32(), "outDly: should be int32");
                    scmp->out_dly = val->Int32Value();
                } else if (keystr == "presolve"){
                    V8CHECKBOOL(!val->IsInt32(), "presolve: should be int32");
                    scmp->presolve = val->Int32Value();
                } else {
                    std::string error("Unknow field: ");
                    error += keystr;
                    V8CHECKBOOL(true, error.c_str());
                }
            }
            return true;
        }
    private:
        explicit Problem(): node::ObjectWrap(){
            handle = glp_create_prob();
            thread = false;
        }

        ~Problem(){
            if (handle) glp_delete_prob(handle);
        }
        
        static NAN_METHOD(New) {
            NanScope();
            
            V8CHECK(!args.IsConstructCall(), "Constructor Problem requires 'new'");
            
            GLP_CATCH_RET(Problem* obj = new Problem();
                      obj->Wrap(args.This());
                      NanReturnValue(args.This());
            )
        }

        static NAN_METHOD(LoadMatrix) {
            NanScope();
            
            V8CHECK(args.Length() != 4, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !args[1]->IsInt32Array()
                    || !args[2]->IsInt32Array() || !args[3]->IsFloat64Array(), "Wrong arguments");
            
            Local<Int32Array> ia = Local<Int32Array>::Cast(args[1]);
            Local<Int32Array> ja = Local<Int32Array>::Cast(args[2]);
            Local<Float64Array> ar = Local<Float64Array>::Cast(args[3]);
            
            int* pia = new int[ia->Length()];
            int* pja = new int[ja->Length()];
            double* par = new double[ar->Length()];
            
            for (size_t i = 0; i < ia->Length(); i++) pia[i] = ia->Get(i)->Int32Value();
            for (size_t i = 0; i < ja->Length(); i++) pja[i] = ja->Get(i)->Int32Value();
            for (size_t i = 0; i < ar->Length(); i++) par[i] = ar->Get(i)->NumberValue();
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            GLP_CATCH(glp_load_matrix(lp->handle, args[0]->Int32Value(), pia, pja, par);)
            
            delete[] pia;
            delete[] pja;
            delete[] par;
        }

        static NAN_METHOD(SimplexSync) {
            NanScope();
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_smcp scmp;
                      glp_init_smcp(&scmp);
                      if (args.Length() == 1)
                          if (!SmcpInit(&scmp, args[0])) return;
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      V8CHECK(lp->thread, "an async operation is inprogress");
                      
                      glp_simplex(lp->handle, &scmp);
            )
        }
        
        class SimplexWorker : public NanAsyncWorker {
        public:
            SimplexWorker(NanCallback *callback, Problem *lp)
            : NanAsyncWorker(callback), lp(lp){
                glp_init_smcp(&smcp);
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    glp_simplex(lp->handle, &smcp);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
        public:
            Problem *lp;
            glp_smcp smcp;
        };
        
        static NAN_METHOD(Simplex) {
            NanScope();
            
            V8CHECK(args.Length() > 2, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsObject() || args[0]->IsNull()) || !args[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            SimplexWorker *worker = new SimplexWorker(callback, lp);
            if (!SmcpInit(&worker->smcp, args[0])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static NAN_METHOD(ExactSync) {
            NanScope();
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_smcp scmp;
                      glp_init_smcp(&scmp);
                      if (args.Length() == 1) {
                          if (args[0]->IsObject())
                              if(!SmcpInit(&scmp, args[0])) return;
                      }
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      V8CHECK(lp->thread, "an async operation is inprogress");
                          
                      glp_exact(lp->handle, &scmp);
            )
        }
        
        class ExactWorker : public NanAsyncWorker {
        public:
            ExactWorker(NanCallback *callback, Problem *lp)
            : NanAsyncWorker(callback), lp(lp){
                glp_init_smcp(&smcp);
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    glp_exact(lp->handle, &smcp);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
        public:
            Problem *lp;
            glp_smcp smcp;
        };
        
        static NAN_METHOD(Exact) {
            NanScope();
            
            V8CHECK(args.Length() > 2, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsObject() || args[0]->IsNull()) || !args[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            ExactWorker *worker = new ExactWorker(callback, lp);
            if (!SmcpInit(&worker->smcp, args[0])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static bool IptcpInit(glp_iptcp* iptcp, Local<Value> value){
            if (!value->IsObject()) return true;
            Local<Object> obj = value->ToObject();
            Local<Array> props = obj->GetPropertyNames();
            for(uint32_t i = 0; i < props->Length(); i++){
                Local<Value> key = props->Get(i);
                Local<Value> val = obj->Get(key);
                std::string keystr = std::string(V8TOCSTRING(key));
                if (keystr == "msgLev"){
                    V8CHECKBOOL(!val->IsInt32(), "msgLev: should be int32");
                    iptcp->msg_lev = val->Int32Value();
                } else if (keystr == "ordAlg"){
                    V8CHECKBOOL(!val->IsInt32(), "ordAlg: should be int32");
                    iptcp->ord_alg = val->Int32Value();
                } else {
                    std::string error("Unknow field: ");
                    error += keystr;
                    V8CHECKBOOL(true, error.c_str());
                }
            }
            return true;
        }
        
        static NAN_METHOD(InteriorSync) {
            NanScope();
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_iptcp iptcp;
                      glp_init_iptcp(&iptcp);
                      if (args.Length() == 1)
                         if (!IptcpInit(&iptcp, args[0])) return;
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      V8CHECK(lp->thread, "an async operation is inprogress");
                          
                      glp_interior(lp->handle, &iptcp);
            )
        }
        
        class InteriorWorker : public NanAsyncWorker {
        public:
            InteriorWorker(NanCallback *callback, Problem *lp)
            : NanAsyncWorker(callback), lp(lp){
                glp_init_iptcp(&iptcp);
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    glp_interior(lp->handle, &iptcp);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
        public:
            Problem *lp;
            glp_iptcp iptcp;
            
        };
        
        static NAN_METHOD(Interior) {
            NanScope();
            
            V8CHECK(args.Length() > 2, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsObject() || args[0]->IsNull()) || !args[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            InteriorWorker *worker = new InteriorWorker(callback, lp);
            if (!IptcpInit(&worker->iptcp, args[0])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static bool MpscpInit(glp_mpscp *mpscp, Local<Value> value){
            if (value->IsObject()){
                Local<Object> obj = value->ToObject();
                Local<Array> props = obj->GetPropertyNames();
                for(uint32_t i = 0; i < props->Length(); i++){
                    Local<Value> key = props->Get(i);
                    Local<Value> val = obj->Get(key);
                    std::string keystr = std::string(V8TOCSTRING(key));
                    if (keystr == "blank"){
                        V8CHECKBOOL(!val->IsInt32(), "blank: should be int32");
                        mpscp->blank = val->Int32Value();
                    } else if (keystr == "tolMps"){
                        V8CHECKBOOL(!val->IsNumber(), "tolMps: should be number");
                        mpscp->tol_mps = val->NumberValue();
                    } else if (keystr == "objName"){
                        V8CHECKBOOL(!val->IsString(), "objName: should be a string");
                        std::string objname = std::string(V8TOCSTRING(val));
                        mpscp->obj_name = new char[objname.length()];
                        memcpy((void *)mpscp->obj_name, objname.c_str(), objname.length());
                    } else {
                        std::string error("Unknow field: ");
                        error += keystr;
                        V8CHECKBOOL(true, error.c_str());
                    }
                }
            }
            return true;
        }
        
        static NAN_METHOD(ReadMpsSync) {
            NanScope();
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !(args[1]->IsObject() || args[1]->IsNull() || args[1]->IsUndefined()) || !args[2]->IsString(), "Wrong arguments");
            
            GLP_CATCH_RET(
                glp_mpscp mpscp;
                glp_init_mpscp(&mpscp);
                if (args.Length() == 1)
                    if (!MpscpInit(&mpscp, args[0])) return;
                      
                Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                V8CHECK(!lp->handle, "object deleted");
                V8CHECK(lp->thread, "an async operation is inprogress");
                          
                int ret = glp_read_mps(lp->handle, args[0]->Int32Value(), &mpscp, V8TOCSTRING(args[2]));
                if (mpscp.obj_name) delete[] mpscp.obj_name;
                NanReturnValue(ret);
            )
        }
        
        class ReadMpsWorker : public NanAsyncWorker {
        public:
            ReadMpsWorker(NanCallback *callback, Problem *lp, int fmt, std::string file)
            : NanAsyncWorker(callback), fmt(fmt), lp(lp), file(file){
                glp_init_mpscp(&mpscp);
            }
            
            ~ReadMpsWorker(){
                if (mpscp.obj_name) delete[] mpscp.obj_name;
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_read_mps(lp->handle, fmt, &mpscp, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
            
        public:
            int ret, fmt;
            Problem *lp;
            glp_mpscp mpscp;
            std::string file;
        };
        
        static NAN_METHOD(ReadMps) {
            NanScope();
            
            V8CHECK(args.Length() != 4, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !(args[1]->IsObject() || args[1]->IsNull())
                    || !args[2]->IsString() || !args[3]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[3].As<Function>());
            ReadMpsWorker *worker = new ReadMpsWorker(callback, lp, args[0]->Int32Value(), V8TOCSTRING(args[2]));
            if (!MpscpInit(&worker->mpscp, args[1])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static NAN_METHOD(WriteMpsSync) {
            NanScope();
            std::string objname;
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !(args[1]->IsObject() || args[1]->IsNull() || args[1]->IsUndefined()) || !args[2]->IsString(), "Wrong arguments");
            
            GLP_CATCH_RET(
              glp_mpscp mpscp;
              glp_init_mpscp(&mpscp);
              if (args.Length() == 1)
                if (!MpscpInit(&mpscp, args[0])) return;
              
              Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
              V8CHECK(!lp->handle, "object deleted");
              V8CHECK(lp->thread, "an async operation is inprogress");
                          
              NanReturnValue(glp_write_mps(lp->handle, args[0]->Int32Value(), &mpscp, V8TOCSTRING(args[2])));
            )
        }
        
        class WriteMpsWorker : public NanAsyncWorker {
        public:
            WriteMpsWorker(NanCallback *callback, Problem *lp, int fmt, std::string file)
            : NanAsyncWorker(callback), fmt(fmt), lp(lp), file(file){
                glp_init_mpscp(&mpscp);
            }
            
            ~WriteMpsWorker(){
                if (mpscp.obj_name) delete[] mpscp.obj_name;
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_write_mps(lp->handle, fmt, &mpscp, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
            
        public:
            int ret, fmt;
            Problem *lp;
            glp_mpscp mpscp;
            std::string file;
        };
        
        static NAN_METHOD(WriteMps) {
            NanScope();
            
            V8CHECK(args.Length() != 4, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !(args[1]->IsObject() || args[1]->IsNull())
                    || !args[2]->IsString() || !args[3]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[3].As<Function>());
            WriteMpsWorker *worker = new WriteMpsWorker(callback, lp, args[0]->Int32Value(), V8TOCSTRING(args[2]));
            if (!MpscpInit(&worker->mpscp, args[1])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        
        
        static void IocpCallback(glp_tree *T, void *info){
            NanCallback* cb = (NanCallback*)info;
            const unsigned argc = 1;
            Local<Value> t = Tree::Instantiate(T);
            Local<Value> argv[argc] = {NanNew<Value>(t)};
            cb->Call(argc, argv);
            Tree* host = ObjectWrap::Unwrap<Tree>(t->ToObject());
            host->thread = true;
            host->handle = NULL;
        };
        
        
        static bool IocpInit(glp_iocp *iocp, Local<Value> value){
            if (value->IsObject()){
                Local<Object> obj = value->ToObject();
                Local<Array> props = obj->GetPropertyNames();
                for(uint32_t i = 0; i < props->Length(); i++){
                    Local<Value> key = props->Get(i);
                    Local<Value> val = obj->Get(key);
                    std::string keystr = std::string(V8TOCSTRING(key));
                    if (keystr == "msgLev"){
                        V8CHECKBOOL(!val->IsInt32(), "msgLev: should be int32");
                        iocp->msg_lev = val->Int32Value();
                    } else if (keystr == "brTech"){
                        V8CHECKBOOL(!val->IsInt32(), "brTech: should be int32");
                        iocp->br_tech = val->Int32Value();
                    } else if (keystr == "btTech"){
                        V8CHECKBOOL(!val->IsInt32(), "btTech: should be int32");
                        iocp->bt_tech = val->Int32Value();
                    } else if (keystr == "tolInt"){
                        V8CHECKBOOL(!val->IsNumber(), "tolInt: should be number");
                        iocp->tol_int = val->NumberValue();
                    } else if (keystr == "tolObj"){
                        V8CHECKBOOL(!val->IsNumber(), "tolObj: should be number");
                        iocp->tol_obj = val->NumberValue();
                    } else if (keystr == "tmLim"){
                        V8CHECKBOOL(!val->IsInt32(), "tmLim: should be int32");
                        iocp->tm_lim = val->Int32Value();
                    } else if (keystr == "outFrq"){
                        V8CHECKBOOL(!val->IsInt32(), "outFrq: should be int32");
                        iocp->out_frq = val->Int32Value();
                    } else if (keystr == "outDly"){
                        V8CHECKBOOL(!val->IsInt32(), "outDly: should be int32");
                        iocp->out_dly = val->Int32Value();
                    } else if (keystr == "ppTech"){
                        V8CHECKBOOL(!val->IsInt32(), "ppTech: should be int32");
                        iocp->pp_tech = val->Int32Value();
                    } else if (keystr == "mipGap"){
                        V8CHECKBOOL(!val->IsNumber(), "mipGap: should be number");
                        iocp->mip_gap = val->NumberValue();
                    } else if (keystr == "mirCuts"){
                        V8CHECKBOOL(!val->IsInt32(), "mirCuts: should be int32");
                        iocp->mir_cuts = val->Int32Value();
                    } else if (keystr == "gmiCuts"){
                        V8CHECKBOOL(!val->IsInt32(), "gmiCuts: should be int32");
                        iocp->gmi_cuts = val->Int32Value();
                    } else if (keystr == "covCuts"){
                        V8CHECKBOOL(!val->IsInt32(), "covCuts: should be int32");
                        iocp->cov_cuts = val->Int32Value();
                    } else if (keystr == "clqCuts"){
                        V8CHECKBOOL(!val->IsInt32(), "clqCuts: should be int32");
                        iocp->clq_cuts = val->Int32Value();
                    } else if (keystr == "presolve"){
                        V8CHECKBOOL(!val->IsInt32(), "presolve: should be int32");
                        iocp->presolve = val->Int32Value();
                    } else if (keystr == "binarize"){
                        V8CHECKBOOL(!val->IsInt32(), "binarize: should be int32");
                        iocp->binarize = val->Int32Value();
                    } else if (keystr == "fpHeur"){
                        V8CHECKBOOL(!val->IsInt32(), "fpHeur: should be int32");
                        iocp->fp_heur = val->Int32Value();
                    } else if (keystr == "psHeur"){
                        V8CHECKBOOL(!val->IsInt32(), "psHeur: should be int32");
                        iocp->ps_heur = val->Int32Value();
                    } else if (keystr == "psTmLim"){
                        V8CHECKBOOL(!val->IsInt32(), "psTmLim: should be int32");
                        iocp->ps_tm_lim = val->Int32Value();
                    } else if (keystr == "useSol"){
                        V8CHECKBOOL(!val->IsInt32(), "useSol: should be int32");
                        iocp->use_sol = val->Int32Value();
                    } else if (keystr == "alien"){
                        V8CHECKBOOL(!val->IsInt32(), "alien: should be int32");
                        iocp->alien = val->Int32Value();
                    } else if (keystr == "saveSol"){
                        V8CHECKBOOL(!val->IsString(), "saveSol: should be a string");
                        std::string solfile = std::string(V8TOCSTRING(val));
                        iocp->save_sol = new char[solfile.length()];
                        memcpy((void *)iocp->save_sol, solfile.c_str(), solfile.length());
                    } else if (keystr == "cbFunc"){
                        V8CHECKBOOL(!val->IsFunction(), "cbFunc: should be a function");
                        iocp->cb_func = IocpCallback;
                        iocp->cb_info = new NanCallback(Local<Function>::Cast(val));
                    } else if (keystr == "cbReasons"){
                        V8CHECKBOOL(!val->IsInt32(), "cbReason: should be int32");
                        iocp->cb_reasons = val->Int32Value();
                    } else {
                        std::string error("Unknow field: ");
                        error += keystr;
                        V8CHECKBOOL(true, error.c_str());
                    }
                }
            }
            return true;
        }
        
        static NAN_METHOD(IntoptSync) {
            NanScope();
            V8CHECK(args.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_iocp iocp;
                      glp_init_iocp(&iocp);
                      if (args.Length() == 1)
                          if (!IocpInit(&iocp, args[0])) return;
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      V8CHECK(lp->thread, "an async operation is inprogress");
                          
                      glp_intopt(lp->handle, &iocp);
                      if (iocp.cb_info) delete (NanCallback*)iocp.cb_info;
                      if (iocp.save_sol) delete[] iocp.save_sol;
            )
        }
        
        class IntoptWorker : public NanAsyncWorker {
        public:
            IntoptWorker(NanCallback *callback, Problem *lp)
            : NanAsyncWorker(callback), lp(lp){
                glp_init_iocp(&parm);
                glp_init_mip_ctx(&ctx);
                ctx.parm = &parm;
                state = 0;
            }
            
            ~IntoptWorker(){
                if (parm.cb_info) delete (NanCallback*)parm.cb_info;
                if (parm.save_sol) delete[] parm.save_sol;
            }
            
            void Execute () {
                try {
                    if (state) {
                        glp_intopt_run(&ctx);
                    } else {
                        state = 1;
                        glp_intopt_start(lp->handle, &ctx);
                    }
                } catch (std::string s){
                    ctx.done = 1;
                    SetErrorMessage(s.c_str());
                }
            }
            void WorkComplete() {
                lp->thread = false;
                NanScope();
                if (ctx.done) {
                    state = 2;
                    glp_intopt_stop(lp->handle, &ctx);
                    if (ErrorMessage() == NULL)
                        HandleOKCallback();
                    else
                        HandleErrorCallback();
                    
                    delete callback;
                    callback = NULL;
                } else {
                    parm.cb_func(ctx.tree, parm.cb_info);
                    lp->thread = true;
                    NanAsyncQueueWorker(this);
                }
            }
            
            void HandleOKCallback(){
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ctx.ret)};
                callback->Call(2, args);
            }
            
            void Destroy() {
                if (state == 2) delete this;
            }
        public:
            int state;
            Problem *lp;
            glp_iocp parm;
            glp_mip_ctx ctx;
        };
        
        static NAN_METHOD(Intopt) {
            NanScope();
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsObject() || args[0]->IsNull()) || !args[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            IntoptWorker *worker = new IntoptWorker(callback, lp);
            if (!IocpInit(&worker->parm, args[0])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static NAN_METHOD(ReadLpSync) {
            NanScope();
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsString(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanReturnValue(glp_read_lp(lp->handle, NULL, V8TOCSTRING(args[0])));
        }
        
        class ReadLpWorker : public NanAsyncWorker {
        public:
            ReadLpWorker(NanCallback *callback, Problem *lp, std::string file)
            : NanAsyncWorker(callback), lp(lp), file(file){
                
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_read_lp(lp->handle, NULL, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
            
        public:
            int ret;
            Problem *lp;
            std::string file;
        };
        
        static NAN_METHOD(ReadLp) {
            NanScope();
            
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!args[0]->IsString() || !args[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            ReadLpWorker *worker = new ReadLpWorker(callback, lp, V8TOCSTRING(args[0]));
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static NAN_METHOD(WriteLpSync) {
            NanScope();
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsString(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            GLP_CATCH_RET(NanReturnValue(glp_write_lp(lp->handle, NULL, V8TOCSTRING(args[0])));)
        }
        
        class WriteLpWorker : public NanAsyncWorker {
        public:
            WriteLpWorker(NanCallback *callback, Problem *lp, std::string file)
            : NanAsyncWorker(callback), lp(lp), file(file){
                
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_write_lp(lp->handle, NULL, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
        public:
            int ret;
            Problem *lp;
            std::string file;
        };
        
        static NAN_METHOD(WriteLp) {
            NanScope();
            
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!args[0]->IsString() || !args[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            WriteLpWorker *worker = new WriteLpWorker(callback, lp, V8TOCSTRING(args[0]));
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static NAN_METHOD(CheckKkt) {
            NanScope();
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            double ae_max, re_max;
            int ae_ind, re_ind;
            GLP_CATCH_RET(glp_check_kkt(lp->handle, args[0]->Int32Value(), args[1]->Int32Value(), &ae_max, &ae_ind, &re_max, &re_ind);)
            
            NanCallback* cb = new NanCallback(Local<Function>::Cast(args[2]));
            const unsigned argc = 4;
            Local<Value> argv[argc] = {
                NanNew<Number>(ae_max),
                NanNew<Int32>(ae_ind),
                NanNew<Number>(re_max),
                NanNew<Int32>(re_ind)
            };
            GLP_CATCH(cb->Call(argc, argv);)
            delete cb;
        }
        
        static NAN_METHOD(PrintRangesSync) {
            NanScope();
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsInt32Array() || args[0]->IsNull() || args[0]->IsUndefined()) ||
                !args[1]->IsInt32() || !args[2]->IsString(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            uint32_t count = 0;
            int* plist = NULL;
            int ret = 0;
            GLP_CATCH(
                if (args[0]->IsInt32Array()) {
                    Local<Int32Array> list = Local<Int32Array>::Cast(args[0]);
                    count = list->Length();
                    if (count > 1) {
                        plist = new int[count];
                        for (size_t i = 0; i < count; i++) plist[i] = list->Get(i)->Int32Value();
                        count--;
                    }
                }
                      
                ret = glp_print_ranges(lp->handle, count, plist, args[1]->Int32Value(), V8TOCSTRING(args[2]));
                      
            )
            if (plist) delete[] plist;
            NanReturnValue(ret);
        }
        
        
        class PrintRangesWorker : public NanAsyncWorker {
        public:
            PrintRangesWorker(NanCallback *callback, Problem *lp, int len, int flags, char *file)
            : NanAsyncWorker(callback), lp(lp), len(len), flags(flags), file(file){
                if (len > 0)
                    list = new int[len];
                else
                    list = NULL;
            }
            ~PrintRangesWorker(){
                if (list) delete[] list;
            }
            
            void HandleOKCallback() {
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_print_ranges(lp->handle, len, list, flags, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
        public:
            Problem *lp;
            int len, flags;
            std::string file;
            int *list;
            int ret;
        };
        
        static NAN_METHOD(PrintRanges) {
            NanScope();
            
            V8CHECK(args.Length() != 4, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsInt32Array() || args[0]->IsNull() || args[0]->IsUndefined()) ||
                !args[1]->IsInt32() || !args[2]->IsString() || !args[3]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            size_t len = 0;
            Local<Int32Array> list;
            if (args[0]->IsInt32Array()) {
                list = Local<Int32Array>::Cast(args[0]);
                len = list->Length();
            }
            
            NanCallback *callback = new NanCallback(args[3].As<Function>());
            PrintRangesWorker *worker = new PrintRangesWorker(callback, lp, len, args[1]->Int32Value(), V8TOCSTRING(args[2]));
            if (len > 0) {
                for (size_t i = 0; i < len; i++) worker->list[i] = list->Get(i)->Int32Value();
                worker->len--;
            }
            
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        static NAN_METHOD(GetBfcp) {
            NanScope();
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            GLP_CATCH_RET(
                glp_bfcp bfcp;
                glp_get_bfcp(lp->handle, &bfcp);
                Local<Object> ret = NanNew<Object>();
                GLP_SET_FIELD_INT32(ret, "type", bfcp.type);
                GLP_SET_FIELD_DOUBLE(ret, "pivTol", bfcp.piv_tol);
                GLP_SET_FIELD_INT32(ret, "pivLim", bfcp.piv_lim);
                GLP_SET_FIELD_INT32(ret, "suhl", bfcp.suhl);
                GLP_SET_FIELD_DOUBLE(ret, "epsTol", bfcp.eps_tol);
                GLP_SET_FIELD_INT32(ret, "nfsMax", bfcp.nfs_max);
                GLP_SET_FIELD_INT32(ret, "nrsMax", bfcp.nrs_max);
                      
                NanReturnValue(ret);
            )
        }
        
        static NAN_METHOD(SetBfcp) {
            NanScope();
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsObject() || args[0]->IsNull() || args[0]->IsUndefined()), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            GLP_CATCH_RET(
                      glp_bfcp bfcp;
                      glp_get_bfcp(lp->handle, &bfcp);
                      
                      if (args[0]->IsObject()){
                          Local<Object> obj = args[0]->ToObject();
                          Local<Array> props = obj->GetPropertyNames();
                          for(uint32_t i = 0; i < props->Length(); i++){
                              Local<Value> key = props->Get(i);
                              Local<Value> val = obj->Get(key);
                              std::string keystr = std::string(V8TOCSTRING(key));
                              if (keystr == "type"){
                                  V8CHECK(!val->IsInt32(), "type: should be int32");
                                  bfcp.type = val->Int32Value();
                              } else if (keystr == "pivTol"){
                                  V8CHECK(!val->IsNumber(), "pivTol: should be number");
                                  bfcp.piv_tol = val->NumberValue();
                              } else if (keystr == "pivLim"){
                                  V8CHECK(!val->IsInt32(), "pivLim: should be int32");
                                  bfcp.piv_lim = val->Int32Value();
                              } else if (keystr == "suhl"){
                                  V8CHECK(!val->IsInt32(), "suhl: should be int32");
                                  bfcp.suhl = val->Int32Value();
                              } else if (keystr == "epsTol"){
                                  V8CHECK(!val->IsNumber(), "epsTol: should be number");
                                  bfcp.eps_tol = val->NumberValue();
                              } else if (keystr == "nfsMax"){
                                  V8CHECK(!val->IsInt32(), "nfsMax: should be int32");
                                  bfcp.nfs_max = val->Int32Value();
                              } else if (keystr == "nrsMax"){
                                  V8CHECK(!val->IsInt32(), "nrsMax: should be int32");
                                  bfcp.nrs_max = val->Int32Value();
                              } else {
                                  std::string error("Unknow field: ");
                                  error += keystr;
                                  V8CHECK(true, error.c_str());
                              }
                          }
                      }
                      
                      glp_set_bfcp(lp->handle, &bfcp);
            )
        }
        
        class ScaleWorker : public NanAsyncWorker {
        public:
            ScaleWorker(NanCallback *callback, Problem *lp, int param)
            : NanAsyncWorker(callback), lp(lp), param(param){
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    glp_scale_prob(lp->handle, param);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
        public:
            Problem *lp;
            int param;
        };
        
        static NAN_METHOD(Scale) {
            NanScope();
            
            V8CHECK(args.Length() != 2, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !args[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            ScaleWorker *worker = new ScaleWorker(callback, lp, args[0]->Int32Value());
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        

        class FactorizeWorker : public NanAsyncWorker {
        public:
            FactorizeWorker(NanCallback *callback, Problem *lp)
            : NanAsyncWorker(callback), lp(lp) {
            }
            void WorkComplete() {
                lp->thread = false;
                NanAsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_factorize(lp->handle);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> args[] = {NanNull(), NanNew<Int32>(ret)};
                callback->Call(2, args);
            }
        public:
            Problem *lp;
            int ret;
        };
        
        static NAN_METHOD(Factorize) {
            NanScope();
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            NanCallback *callback = new NanCallback(args[0].As<Function>());
            FactorizeWorker *worker = new FactorizeWorker(callback, lp);
            lp->thread = true;
            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        
        
        
        GLP_BIND_VOID_STR(Problem, SetProbName, glp_set_prob_name);
        
        GLP_BIND_STR(Problem, GetProbName, glp_get_prob_name);
        
        GLP_BIND_VOID_INT32(Problem, SetObjDir, glp_set_obj_dir);
        
        GLP_BIND_VALUE(Problem, GetObjDir, glp_get_obj_dir);
        
        GLP_BIND_VOID_INT32(Problem, AddRows, glp_add_rows);
        
        GLP_BIND_VOID_INT32_STR(Problem, SetRowName, glp_set_row_name);
        
        GLP_BIND_STR_INT32(Problem, GetRowName, glp_get_row_name);
        
        GLP_BIND_VOID_INT32_INT32_DOUBLE_DOUBLE(Problem, SetRowBnds, glp_set_row_bnds);
        
        GLP_BIND_VOID_INT32(Problem, AddCols, glp_add_cols);
        
        GLP_BIND_VOID_INT32_STR(Problem, SetColName, glp_set_col_name);
        
        GLP_BIND_STR_INT32(Problem, GetColName, glp_get_col_name);
        
        GLP_BIND_VOID_INT32_INT32_DOUBLE_DOUBLE(Problem, SetColBnds, glp_set_col_bnds);
        
        GLP_BIND_VOID_INT32_DOUBLE(Problem, SetObjCoef, glp_set_obj_coef);
        
        GLP_BIND_VALUE_INT32(Problem, GetObjCoef, glp_get_obj_coef);
        
        GLP_BIND_VALUE(Problem, GetObjVal, glp_get_obj_val);
        
        GLP_BIND_VOID_STR(Problem, SetObjName, glp_set_obj_name);
        
        GLP_BIND_STR(Problem, GetObjName, glp_get_obj_name);
        
        GLP_BIND_VOID_INT32_INT32ARRAY_FLOAT64ARRAY(Problem, SetMatRow, glp_set_mat_row);
        
        GLP_BIND_VALUE_INT32_CALLBACK(Problem, GetMatRow, glp_get_mat_row);
        
        GLP_BIND_VOID_INT32_INT32ARRAY_FLOAT64ARRAY(Problem, SetMatCol, glp_set_mat_col);
        
        GLP_BIND_VALUE_INT32_CALLBACK(Problem, GetMatCol, glp_get_mat_col);
        
        GLP_BIND_VOID(Problem, SortMatrix, glp_sort_matrix);
        
        GLP_BIND_VOID_INT32ARRAY(Problem, DelRows, glp_del_rows);
        
        GLP_BIND_VOID_INT32ARRAY(Problem, DelCols, glp_del_cols);
        
        GLP_BIND_VOID(Problem, Erase, glp_erase_prob);
        
        GLP_BIND_VALUE(Problem, GetNumRows, glp_get_num_rows);
        
        GLP_BIND_VALUE(Problem, GetNumCols, glp_get_num_cols);
        
        GLP_BIND_VALUE_INT32(Problem, GetRowType, glp_get_row_type);
        
        GLP_BIND_VALUE_INT32(Problem, GetRowLb, glp_get_row_lb);
        
        GLP_BIND_VALUE_INT32(Problem, GetRowUb, glp_get_row_ub);
        
        GLP_BIND_VALUE_INT32(Problem, GetColType, glp_get_col_type);
        
        GLP_BIND_VALUE_INT32(Problem, GetColLb, glp_get_col_lb);
        
        GLP_BIND_VALUE_INT32(Problem, GetColUb, glp_get_col_ub);
        
        GLP_BIND_VALUE(Problem, GetNumNz, glp_get_num_nz);
        
        GLP_BIND_VOID(Problem, CreateIndex, glp_create_index);
        
        GLP_BIND_VALUE_STR(Problem, FindRow, glp_find_row);
        
        GLP_BIND_VALUE_STR(Problem, FindCol, glp_find_col);
        
        GLP_BIND_VOID(Problem, DeleteIndex, glp_delete_index);
        
        GLP_BIND_VOID_INT32_DOUBLE(Problem, SetRii, glp_set_rii);
        
        GLP_BIND_VOID_INT32_DOUBLE(Problem, SetSjj, glp_set_sjj);
        
        GLP_BIND_VALUE_INT32(Problem, GetRii, glp_get_rii);
        
        GLP_BIND_VALUE_INT32(Problem, GetSjj, glp_get_sjj);
        
        GLP_BIND_VOID_INT32(Problem, ScaleSync, glp_scale_prob);
        
        GLP_BIND_VOID(Problem, UnscaleSync, glp_unscale_prob);
        GLP_ASYNC_VOID(Problem, Unscale, glp_unscale_prob);
        
        GLP_BIND_VOID_INT32_INT32(Problem, SetRowStat, glp_set_row_stat);
        
        GLP_BIND_VOID_INT32_INT32(Problem, SetColStat, glp_set_col_stat);
        
        GLP_BIND_VALUE_INT32(Problem, GetRowStat, glp_get_row_stat);
        
        GLP_BIND_VALUE_INT32(Problem, GetColStat, glp_get_col_stat);
        
        GLP_BIND_VOID(Problem, StdBasis, glp_std_basis);
        
        GLP_BIND_VOID_INT32(Problem, AdvBasis, glp_adv_basis);
        
        GLP_BIND_VOID(Problem, CpxBasis, glp_cpx_basis);
        
        GLP_BIND_VALUE(Problem, GetStatus, glp_get_status);
        
        GLP_BIND_VALUE(Problem, GetPrimStat, glp_get_prim_stat);
        
        GLP_BIND_VALUE(Problem, GetDualStat, glp_get_dual_stat);
        
        GLP_BIND_VALUE_INT32(Problem, GetRowPrim, glp_get_row_prim);
        
        GLP_BIND_VALUE_INT32(Problem, GetRowDual, glp_get_row_dual);
        
        GLP_BIND_VALUE_INT32(Problem, GetColPrim, glp_get_col_prim);
        
        GLP_BIND_VALUE_INT32(Problem, GetColDual, glp_get_col_dual);
        
        GLP_BIND_VALUE(Problem, GetUnbndRay, glp_get_unbnd_ray);
        
        GLP_BIND_VALUE(Problem, GetItCnt, glp_get_it_cnt);
        
        GLP_BIND_VOID_INT32(Problem, SetItCnt, glp_set_it_cnt);
        
        GLP_BIND_VALUE(Problem, IptStatus, glp_ipt_status);
        
        GLP_BIND_VALUE(Problem, IptObjVal, glp_ipt_obj_val);
        
        GLP_BIND_VALUE_INT32(Problem, IptRowPrim, glp_ipt_row_prim);
        
        GLP_BIND_VALUE_INT32(Problem, IptRowDual, glp_ipt_row_dual);
        
        GLP_BIND_VALUE_INT32(Problem, IptColPrim, glp_ipt_col_prim);
        
        GLP_BIND_VALUE_INT32(Problem, IptColDual, glp_ipt_col_dual);
        
        GLP_BIND_VOID_INT32_INT32(Problem, SetColKind, glp_set_col_kind);
        
        GLP_BIND_VALUE_INT32(Problem, GetColKind, glp_get_col_kind);
        
        GLP_BIND_VALUE(Problem, GetNumInt, glp_get_num_int);
        
        GLP_BIND_VALUE(Problem, GetNumBin, glp_get_num_bin);
        
        GLP_BIND_VALUE_INT32_STR(Problem, ReadProbSync, glp_read_prob);
        GLP_ASYNC_INT32_INT32_STR(Problem, ReadProb, glp_read_prob);
        
        GLP_BIND_VALUE_INT32_STR(Problem, WriteProbSync, glp_write_prob);
        GLP_ASYNC_INT32_INT32_STR(Problem, WriteProb, glp_write_prob);
        
        GLP_BIND_VALUE(Problem, MipStatus, glp_mip_status);
        
        GLP_BIND_VALUE(Problem, MipObjVal, glp_mip_obj_val);
        
        GLP_BIND_VALUE_INT32(Problem, MipRowVal, glp_mip_row_val);
        
        GLP_BIND_VALUE_INT32(Problem, MipColVal, glp_mip_col_val);
        
        GLP_BIND_VALUE_STR(Problem, PrintSolSync, glp_print_sol);
        GLP_ASYNC_INT32_STR(Problem, PrintSol, glp_print_sol);
        
        GLP_BIND_VALUE_STR(Problem, ReadSolSync, glp_read_sol);
        GLP_ASYNC_INT32_STR(Problem, ReadSol, glp_read_sol);
        
        GLP_BIND_VALUE_STR(Problem, WriteSolSync, glp_write_sol);
        GLP_ASYNC_INT32_STR(Problem, WriteSol, glp_write_sol);
        
        GLP_BIND_VALUE_STR(Problem, PrintIptSync, glp_print_ipt);
        GLP_ASYNC_INT32_STR(Problem, PrintIpt, glp_print_ipt);
        
        GLP_BIND_VALUE_STR(Problem, ReadIptSync, glp_read_ipt);
        GLP_ASYNC_INT32_STR(Problem, ReadIpt, glp_read_ipt);
        
        GLP_BIND_VALUE_STR(Problem, WriteIptSync, glp_write_ipt);
        GLP_ASYNC_INT32_STR(Problem, WriteIpt, glp_write_ipt);
        
        GLP_BIND_VALUE_STR(Problem, PrintMipSync, glp_print_mip);
        GLP_ASYNC_INT32_STR(Problem, PrintMip, glp_print_mip);
        
        GLP_BIND_VALUE_STR(Problem, ReadMipSync, glp_read_mip);
        GLP_ASYNC_INT32_STR(Problem, ReadMip, glp_read_mip);
        
        GLP_BIND_VALUE_STR(Problem, WriteMipSync, glp_write_mip);
        GLP_ASYNC_INT32_STR(Problem, WriteMip, glp_write_mip);
        
        GLP_BIND_VALUE(Problem, BfExists, glp_bf_exists);
        
        GLP_BIND_VALUE(Problem, FactorizeSync, glp_factorize);
        
        GLP_BIND_VALUE(Problem, BfUpdated, glp_bf_updated);
        
        GLP_BIND_VALUE_INT32(Problem, GetBhead, glp_get_bhead);
        
        GLP_BIND_VALUE_INT32(Problem, GetRowBind, glp_get_row_bind);
        
        GLP_BIND_VALUE_INT32(Problem, GetColBind, glp_get_col_bind);
        
        GLP_BIND_DELETE(Problem, Delete, glp_delete_prob);
        
        //void glp_ftran(glp_prob *P, double x[]);
        
        //void glp_btran(glp_prob *P, double x[]);
        
        GLP_BIND_VALUE(Problem, WarmUp, glp_warm_up);
        
        //int glp_eval_tab_row(glp_prob *P, int k, int ind[], double val[]);
        
        //int glp_eval_tab_col(glp_prob *P, int k, int ind[], double val[]);
        
        //int glp_transform_row(glp_prob *P, int len, int ind[], double val[]);
        
        //int glp_transform_col(glp_prob *P, int len, int ind[], double val[]);
        
        //int glp_prim_rtest(glp_prob *P, int len, const int ind[],
        //                   const double val[], int dir, double eps);
        
        //int glp_dual_rtest(glp_prob *P, int len, const int ind[],
        //                   const double val[], int dir, double eps);
        
        //void glp_analyze_bound(glp_prob *P, int k, double *value1, int *var1,
        //                       double *value2, int *var2);
        
        //void glp_analyze_coef(glp_prob *P, int k, double *coef1, int *var1,
        //                      double *value1, double *coef2, int *var2, double *value2);
        
        
        static Persistent<FunctionTemplate> constructor;
    public:
        glp_prob *handle;
        bool thread;
    };
    
    Persistent<FunctionTemplate> Problem::constructor;
}
    
    
    
    







