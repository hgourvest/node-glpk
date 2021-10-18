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
        static void Init(Local<Object> exports){
            // Prepare constructor template
            Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
            tpl->SetClassName(Nan::New<String>("Problem").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            
            // Prototype
            Nan::SetPrototypeMethod(tpl, "setProbName", SetProbName);
            Nan::SetPrototypeMethod(tpl, "getProbName", GetProbName);
            Nan::SetPrototypeMethod(tpl, "setObjDir", SetObjDir);
            Nan::SetPrototypeMethod(tpl, "getObjDir", GetObjDir);
            Nan::SetPrototypeMethod(tpl, "addRows", AddRows);
            Nan::SetPrototypeMethod(tpl, "setRowName", SetRowName);
            Nan::SetPrototypeMethod(tpl, "getRowName", GetRowName);
            Nan::SetPrototypeMethod(tpl, "setRowBnds", SetRowBnds);
            Nan::SetPrototypeMethod(tpl, "addCols", AddCols);
            Nan::SetPrototypeMethod(tpl, "setColName", SetColName);
            Nan::SetPrototypeMethod(tpl, "getColName", GetColName);
            Nan::SetPrototypeMethod(tpl, "setColBnds", SetColBnds);
            Nan::SetPrototypeMethod(tpl, "setObjCoef", SetObjCoef);
            Nan::SetPrototypeMethod(tpl, "getObjCoef", GetObjCoef);
            Nan::SetPrototypeMethod(tpl, "loadMatrix", LoadMatrix);
            Nan::SetPrototypeMethod(tpl, "simplexSync", SimplexSync);
            Nan::SetPrototypeMethod(tpl, "simplex", Simplex);
            Nan::SetPrototypeMethod(tpl, "getObjVal", GetObjVal);
            Nan::SetPrototypeMethod(tpl, "getColPrim", GetColPrim);
            Nan::SetPrototypeMethod(tpl, "setObjName", SetObjName);
            Nan::SetPrototypeMethod(tpl, "getObjName", GetObjName);
            Nan::SetPrototypeMethod(tpl, "setMatRow", SetMatRow);
            Nan::SetPrototypeMethod(tpl, "getMatRow", GetMatRow);
            Nan::SetPrototypeMethod(tpl, "setMatCol", SetMatCol);
            Nan::SetPrototypeMethod(tpl, "getMatCol", GetMatCol);
            Nan::SetPrototypeMethod(tpl, "sortMatrix", SortMatrix);
            Nan::SetPrototypeMethod(tpl, "delRows", DelRows);
            Nan::SetPrototypeMethod(tpl, "delCols", DelCols);
            Nan::SetPrototypeMethod(tpl, "erase", Erase);
            Nan::SetPrototypeMethod(tpl, "delete", Delete);
            Nan::SetPrototypeMethod(tpl, "getNumRows", GetNumRows);
            Nan::SetPrototypeMethod(tpl, "getNumCols", GetNumCols);
            Nan::SetPrototypeMethod(tpl, "getRowType", GetRowType);
            Nan::SetPrototypeMethod(tpl, "getRowLb", GetRowLb);
            Nan::SetPrototypeMethod(tpl, "getRowUb", GetRowUb);
            Nan::SetPrototypeMethod(tpl, "getColType", GetColType);
            Nan::SetPrototypeMethod(tpl, "getColLb", GetColLb);
            Nan::SetPrototypeMethod(tpl, "getColUb", GetColUb);
            Nan::SetPrototypeMethod(tpl, "getNumNz", GetNumNz);
            Nan::SetPrototypeMethod(tpl, "createIndex", CreateIndex);
            Nan::SetPrototypeMethod(tpl, "findRow", FindRow);
            Nan::SetPrototypeMethod(tpl, "findCol", FindCol);
            Nan::SetPrototypeMethod(tpl, "deleteIndex", DeleteIndex);
            Nan::SetPrototypeMethod(tpl, "setRii", SetRii);
            Nan::SetPrototypeMethod(tpl, "setSjj", SetSjj);
            Nan::SetPrototypeMethod(tpl, "getRii", GetRii);
            Nan::SetPrototypeMethod(tpl, "getSjj", GetSjj);
            Nan::SetPrototypeMethod(tpl, "scaleSync", ScaleSync);
            Nan::SetPrototypeMethod(tpl, "scale", Scale);
            Nan::SetPrototypeMethod(tpl, "unscaleSync", UnscaleSync);
            Nan::SetPrototypeMethod(tpl, "unscale", Unscale);
            Nan::SetPrototypeMethod(tpl, "setRowStat", SetRowStat);
            Nan::SetPrototypeMethod(tpl, "setColStat", SetColStat);
            Nan::SetPrototypeMethod(tpl, "getRowStat", GetRowStat);
            Nan::SetPrototypeMethod(tpl, "getColStat", GetColStat);
            Nan::SetPrototypeMethod(tpl, "stdBasis", StdBasis);
            Nan::SetPrototypeMethod(tpl, "advBasis", AdvBasis);
            Nan::SetPrototypeMethod(tpl, "cpxBasis", CpxBasis);
            Nan::SetPrototypeMethod(tpl, "exactSync", ExactSync);
            Nan::SetPrototypeMethod(tpl, "exact", Exact);
            Nan::SetPrototypeMethod(tpl, "getStatus", GetStatus);
            Nan::SetPrototypeMethod(tpl, "getPrimStat", GetPrimStat);
            Nan::SetPrototypeMethod(tpl, "getDualStat", GetDualStat);
            Nan::SetPrototypeMethod(tpl, "getRowPrim", GetRowPrim);
            Nan::SetPrototypeMethod(tpl, "getRowDual", GetRowDual);
            Nan::SetPrototypeMethod(tpl, "getColDual", GetColDual);
            Nan::SetPrototypeMethod(tpl, "getUnbndRay", GetUnbndRay);
            Nan::SetPrototypeMethod(tpl, "getItCnt", GetItCnt);
            Nan::SetPrototypeMethod(tpl, "setItCnt", SetItCnt);
            Nan::SetPrototypeMethod(tpl, "interiorSync", InteriorSync);
            Nan::SetPrototypeMethod(tpl, "interior", Interior);
            Nan::SetPrototypeMethod(tpl, "iptStatus", IptStatus);
            Nan::SetPrototypeMethod(tpl, "readMpsSync", ReadMpsSync);
            Nan::SetPrototypeMethod(tpl, "readMps", ReadMps);
            Nan::SetPrototypeMethod(tpl, "writeMpsSync", WriteMpsSync);
            Nan::SetPrototypeMethod(tpl, "writeMps", WriteMps);
            Nan::SetPrototypeMethod(tpl, "iptObjVal", IptObjVal);
            Nan::SetPrototypeMethod(tpl, "iptRowPrim", IptRowPrim);
            Nan::SetPrototypeMethod(tpl, "iptRowDual", IptRowDual);
            Nan::SetPrototypeMethod(tpl, "iptColPrim", IptColPrim);
            Nan::SetPrototypeMethod(tpl, "iptColDual", IptColDual);
            Nan::SetPrototypeMethod(tpl, "setColKind", SetColKind);
            Nan::SetPrototypeMethod(tpl, "getColKind", GetColKind);
            Nan::SetPrototypeMethod(tpl, "getNumInt", GetNumInt);
            Nan::SetPrototypeMethod(tpl, "getNumBin", GetNumBin);
            Nan::SetPrototypeMethod(tpl, "intoptSync", IntoptSync);
            Nan::SetPrototypeMethod(tpl, "intopt", Intopt);
            Nan::SetPrototypeMethod(tpl, "readProbSync", ReadProbSync);
            Nan::SetPrototypeMethod(tpl, "readProb", ReadProb);
            Nan::SetPrototypeMethod(tpl, "writeProbSync", WriteProbSync);
            Nan::SetPrototypeMethod(tpl, "writeProb", WriteProb);
            Nan::SetPrototypeMethod(tpl, "readLp", ReadLp);
            Nan::SetPrototypeMethod(tpl, "readLpSync", ReadLpSync);
            Nan::SetPrototypeMethod(tpl, "writeLpSync", WriteLpSync);
            Nan::SetPrototypeMethod(tpl, "writeLp", WriteLp);
            Nan::SetPrototypeMethod(tpl, "mipStatus", MipStatus);
            Nan::SetPrototypeMethod(tpl, "mipObjVal", MipObjVal);
            Nan::SetPrototypeMethod(tpl, "mipRowVal", MipRowVal);
            Nan::SetPrototypeMethod(tpl, "mipColVal", MipColVal);
            Nan::SetPrototypeMethod(tpl, "checkKkt", CheckKkt);
            Nan::SetPrototypeMethod(tpl, "printSolSync", PrintSolSync);
            Nan::SetPrototypeMethod(tpl, "printSol", PrintSol);
            Nan::SetPrototypeMethod(tpl, "readSolSync", ReadSolSync);
            Nan::SetPrototypeMethod(tpl, "readSol", ReadSol);
            Nan::SetPrototypeMethod(tpl, "writeSolSync", WriteSolSync);
            Nan::SetPrototypeMethod(tpl, "writeSol", WriteSol);
            Nan::SetPrototypeMethod(tpl, "printRangesSync", PrintRangesSync);
            Nan::SetPrototypeMethod(tpl, "printRanges", PrintRanges);
            Nan::SetPrototypeMethod(tpl, "printIptSync", PrintIptSync);
            Nan::SetPrototypeMethod(tpl, "printIpt", PrintIpt);
            Nan::SetPrototypeMethod(tpl, "readIptSync", ReadIptSync);
            Nan::SetPrototypeMethod(tpl, "readIpt", ReadIpt);
            Nan::SetPrototypeMethod(tpl, "writeIptSync", WriteIptSync);
            Nan::SetPrototypeMethod(tpl, "writeIpt", WriteIpt);
            Nan::SetPrototypeMethod(tpl, "printMipSync", PrintMipSync);
            Nan::SetPrototypeMethod(tpl, "printMip", PrintMip);
            Nan::SetPrototypeMethod(tpl, "readMipSync", ReadMipSync);
            Nan::SetPrototypeMethod(tpl, "readMip", ReadMip);
            Nan::SetPrototypeMethod(tpl, "writeMipSync", WriteMipSync);
            Nan::SetPrototypeMethod(tpl, "writeMip", WriteMip);
            Nan::SetPrototypeMethod(tpl, "bfExists", BfExists);
            Nan::SetPrototypeMethod(tpl, "factorize", Factorize);
            Nan::SetPrototypeMethod(tpl, "bfUpdated", BfUpdated);
            Nan::SetPrototypeMethod(tpl, "getBfcp", GetBfcp);
            Nan::SetPrototypeMethod(tpl, "setBfcp", SetBfcp);
            Nan::SetPrototypeMethod(tpl, "getBhead", GetBhead);
            Nan::SetPrototypeMethod(tpl, "getRowBind", GetRowBind);
            Nan::SetPrototypeMethod(tpl, "getColBind", GetColBind);
            Nan::SetPrototypeMethod(tpl, "warmUp", WarmUp);
            
            constructor.Reset(tpl);

            exports->Set(Nan::New<String>("Problem").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
        }
        
        static bool SmcpInit(glp_smcp* scmp, Local<Value> value){
            if (!value->IsObject()) return false;
            Local<Object> obj = Nan::To<Object>(value).ToLocalChecked();
            Local<Array> props = Nan::GetPropertyNames(obj).ToLocalChecked();
            for(uint32_t i = 0; i < props->Length(); i++){
                Local<Value> key = props->Get(i);
                Local<Value> val = obj->Get(key);
                std::string keystr = std::string(V8TOCSTRING(key));
                if (keystr == "msgLev"){
                    V8CHECKBOOL(!val->IsInt32(), "msgLev: should be int32");
                    scmp->msg_lev = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "meth"){
                    V8CHECKBOOL(!val->IsInt32(), "meth: should be int32");
                    scmp->meth = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "pricing"){
                    V8CHECKBOOL(!val->IsInt32(), "pricing: should be int32");
                    scmp->pricing = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "rTest"){
                    V8CHECKBOOL(!val->IsInt32(), "rTest: should be int32");
                    scmp->r_test = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "tolBnd"){
                    V8CHECKBOOL(!val->IsNumber(), "tolBnd: should be a Number");
                    scmp->tol_bnd = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "tolDj"){
                    V8CHECKBOOL(!val->IsNumber(), "tolDj: should be a Number");
                    scmp->tol_dj = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "tolPiv"){
                    V8CHECKBOOL(!val->IsNumber(), "tolPiv: should be a Number");
                    scmp->tol_piv = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "objLl"){
                    V8CHECKBOOL(!val->IsNumber(), "objLl: should be a Number");
                    scmp->obj_ll = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "objUl"){
                    V8CHECKBOOL(!val->IsNumber(), "objUl: should be a Number");
                    scmp->obj_ul = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "itLim"){
                    V8CHECKBOOL(!val->IsInt32(), "itLim: should be int32");
                    scmp->it_lim = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "tmLim"){
                    V8CHECKBOOL(!val->IsInt32(), "tmLim: should be int32");
                    scmp->tm_lim = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "outFrq"){
                    V8CHECKBOOL(!val->IsInt32(), "outFrq: should be int32");
                    scmp->out_frq = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "outDly"){
                    V8CHECKBOOL(!val->IsInt32(), "outDly: should be int32");
                    scmp->out_dly = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "presolve"){
                    V8CHECKBOOL(!val->IsInt32(), "presolve: should be int32");
                    scmp->presolve = val->Int32Value(Nan::GetCurrentContext()).FromJust();
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
            V8CHECK(!info.IsConstructCall(), "Constructor Problem requires 'new'");
            
            GLP_CATCH_RET(Problem* obj = new Problem();
                      obj->Wrap(info.This());
                      info.GetReturnValue().Set(info.This());
            )
        }

        static NAN_METHOD(LoadMatrix) {
            V8CHECK(info.Length() != 4, "Wrong number of arguments");
            V8CHECK(!info[0]->IsInt32() || !info[1]->IsInt32Array()
                    || !info[2]->IsInt32Array() || !info[3]->IsFloat64Array(), "Wrong arguments");
            
            Local<Int32Array> ia = Local<Int32Array>::Cast(info[1]);
            Local<Int32Array> ja = Local<Int32Array>::Cast(info[2]);
            Local<Float64Array> ar = Local<Float64Array>::Cast(info[3]);
            
            int* pia = new int[ia->Length()];
            int* pja = new int[ja->Length()];
            double* par = new double[ar->Length()];
            
            for (size_t i = 0; i < ia->Length(); i++) pia[i] = ia->Get(i)->Int32Value(Nan::GetCurrentContext()).FromJust();
            for (size_t i = 0; i < ja->Length(); i++) pja[i] = ja->Get(i)->Int32Value(Nan::GetCurrentContext()).FromJust();
            for (size_t i = 0; i < ar->Length(); i++) par[i] = ar->Get(i)->NumberValue(Nan::GetCurrentContext()).FromJust();
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            GLP_CATCH(glp_load_matrix(lp->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), pia, pja, par);)
            
            delete[] pia;
            delete[] pja;
            delete[] par;
        }

        static NAN_METHOD(SimplexSync) {
            V8CHECK(info.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_smcp scmp;
                      glp_init_smcp(&scmp);
                      if (info.Length() == 1)
                          if (!SmcpInit(&scmp, info[0])) return;
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      V8CHECK(lp->thread, "an async operation is inprogress");
                      
                      glp_simplex(lp->handle, &scmp);
            )
        }
        
        class SimplexWorker : public Nan::AsyncWorker {
        public:
            SimplexWorker(Nan::Callback *callback, Problem *lp)
            : Nan::AsyncWorker(callback), lp(lp){
                glp_init_smcp(&smcp);
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
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
            V8CHECK(info.Length() > 2, "Wrong number of arguments");
            V8CHECK(!(info[0]->IsObject() || info[0]->IsNull()) || !info[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            SimplexWorker *worker = new SimplexWorker(callback, lp);
            if (!SmcpInit(&worker->smcp, info[0])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static NAN_METHOD(ExactSync) {
            V8CHECK(info.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_smcp scmp;
                      glp_init_smcp(&scmp);
                      if (info.Length() == 1) {
                          if (info[0]->IsObject())
                              if(!SmcpInit(&scmp, info[0])) return;
                      }
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      V8CHECK(lp->thread, "an async operation is inprogress");
                          
                      glp_exact(lp->handle, &scmp);
            )
        }
        
        class ExactWorker : public Nan::AsyncWorker {
        public:
            ExactWorker(Nan::Callback *callback, Problem *lp)
            : Nan::AsyncWorker(callback), lp(lp){
                glp_init_smcp(&smcp);
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
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
            V8CHECK(info.Length() > 2, "Wrong number of arguments");
            V8CHECK(!(info[0]->IsObject() || info[0]->IsNull()) || !info[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            ExactWorker *worker = new ExactWorker(callback, lp);
            if (!SmcpInit(&worker->smcp, info[0])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static bool IptcpInit(glp_iptcp* iptcp, Local<Value> value){
            if (!value->IsObject()) return true;
            Local<Object> obj = Nan::To<Object>(value).ToLocalChecked();
            Local<Array> props = Nan::GetPropertyNames(obj).ToLocalChecked();
            for(uint32_t i = 0; i < props->Length(); i++){
                Local<Value> key = props->Get(i);
                Local<Value> val = obj->Get(key);
                std::string keystr = std::string(V8TOCSTRING(key));
                if (keystr == "msgLev"){
                    V8CHECKBOOL(!val->IsInt32(), "msgLev: should be int32");
                    iptcp->msg_lev = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else if (keystr == "ordAlg"){
                    V8CHECKBOOL(!val->IsInt32(), "ordAlg: should be int32");
                    iptcp->ord_alg = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                } else {
                    std::string error("Unknow field: ");
                    error += keystr;
                    V8CHECKBOOL(true, error.c_str());
                }
            }
            return true;
        }
        
        static NAN_METHOD(InteriorSync) {
            
            V8CHECK(info.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_iptcp iptcp;
                      glp_init_iptcp(&iptcp);
                      if (info.Length() == 1)
                         if (!IptcpInit(&iptcp, info[0])) return;
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      V8CHECK(lp->thread, "an async operation is inprogress");
                          
                      glp_interior(lp->handle, &iptcp);
            )
        }
        
        class InteriorWorker : public Nan::AsyncWorker {
        public:
            InteriorWorker(Nan::Callback *callback, Problem *lp)
            : Nan::AsyncWorker(callback), lp(lp){
                glp_init_iptcp(&iptcp);
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
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
            V8CHECK(info.Length() > 2, "Wrong number of arguments");
            V8CHECK(!(info[0]->IsObject() || info[0]->IsNull()) || !info[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            InteriorWorker *worker = new InteriorWorker(callback, lp);
            if (!IptcpInit(&worker->iptcp, info[0])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static bool MpscpInit(glp_mpscp *mpscp, Local<Value> value){
            if (value->IsObject()){
                Local<Object> obj = Nan::To<Object>(value).ToLocalChecked();
                Local<Array> props = Nan::GetPropertyNames(obj).ToLocalChecked();
                for(uint32_t i = 0; i < props->Length(); i++){
                    Local<Value> key = props->Get(i);
                    Local<Value> val = obj->Get(key);
                    std::string keystr = std::string(V8TOCSTRING(key));
                    if (keystr == "blank"){
                        V8CHECKBOOL(!val->IsInt32(), "blank: should be int32");
                        mpscp->blank = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "tolMps"){
                        V8CHECKBOOL(!val->IsNumber(), "tolMps: should be number");
                        mpscp->tol_mps = val->NumberValue(Nan::GetCurrentContext()).FromJust();
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
            V8CHECK(info.Length() != 3, "Wrong number of arguments");
            V8CHECK(!info[0]->IsInt32() || !(info[1]->IsObject() || info[1]->IsNull() || info[1]->IsUndefined()) || !info[2]->IsString(), "Wrong arguments");
            
            GLP_CATCH_RET(
                glp_mpscp mpscp;
                glp_init_mpscp(&mpscp);
                if (info.Length() == 1)
                    if (!MpscpInit(&mpscp, info[0])) return;
                      
                Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
                V8CHECK(!lp->handle, "object deleted");
                V8CHECK(lp->thread, "an async operation is inprogress");
                          
                int ret = glp_read_mps(lp->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), &mpscp, V8TOCSTRING(info[2]));
                if (mpscp.obj_name) delete[] mpscp.obj_name;
                info.GetReturnValue().Set(ret);
            )
        }
        
        class ReadMpsWorker : public Nan::AsyncWorker {
        public:
            ReadMpsWorker(Nan::Callback *callback, Problem *lp, int fmt, std::string file)
            : Nan::AsyncWorker(callback), fmt(fmt), lp(lp), file(file){
                glp_init_mpscp(&mpscp);
            }
            
            ~ReadMpsWorker(){
                if (mpscp.obj_name) delete[] mpscp.obj_name;
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_read_mps(lp->handle, fmt, &mpscp, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
            
        public:
            int ret, fmt;
            Problem *lp;
            glp_mpscp mpscp;
            std::string file;
        };
        
        static NAN_METHOD(ReadMps) {
            V8CHECK(info.Length() != 4, "Wrong number of arguments");
            V8CHECK(!info[0]->IsInt32() || !(info[1]->IsObject() || info[1]->IsNull())
                    || !info[2]->IsString() || !info[3]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[3].As<Function>());
            ReadMpsWorker *worker = new ReadMpsWorker(callback, lp, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), V8TOCSTRING(info[2]));
            if (!MpscpInit(&worker->mpscp, info[1])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static NAN_METHOD(WriteMpsSync) {
            std::string objname;
            
            V8CHECK(info.Length() != 3, "Wrong number of arguments");
            V8CHECK(!info[0]->IsInt32() || !(info[1]->IsObject() || info[1]->IsNull() || info[1]->IsUndefined()) || !info[2]->IsString(), "Wrong arguments");
            
            GLP_CATCH_RET(
              glp_mpscp mpscp;
              glp_init_mpscp(&mpscp);
              if (info.Length() == 1)
                if (!MpscpInit(&mpscp, info[0])) return;
              
              Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
              V8CHECK(!lp->handle, "object deleted");
              V8CHECK(lp->thread, "an async operation is inprogress");
                          
              info.GetReturnValue().Set(glp_write_mps(lp->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), &mpscp, V8TOCSTRING(info[2])));
            )
        }
        
        class WriteMpsWorker : public Nan::AsyncWorker {
        public:
            WriteMpsWorker(Nan::Callback *callback, Problem *lp, int fmt, std::string file)
            : Nan::AsyncWorker(callback), fmt(fmt), lp(lp), file(file){
                glp_init_mpscp(&mpscp);
            }
            
            ~WriteMpsWorker(){
                if (mpscp.obj_name) delete[] mpscp.obj_name;
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_write_mps(lp->handle, fmt, &mpscp, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
            
        public:
            int ret, fmt;
            Problem *lp;
            glp_mpscp mpscp;
            std::string file;
        };
        
        static NAN_METHOD(WriteMps) {
            V8CHECK(info.Length() != 4, "Wrong number of arguments");
            V8CHECK(!info[0]->IsInt32() || !(info[1]->IsObject() || info[1]->IsNull())
                    || !info[2]->IsString() || !info[3]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[3].As<Function>());
            WriteMpsWorker *worker = new WriteMpsWorker(callback, lp, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), V8TOCSTRING(info[2]));
            if (!MpscpInit(&worker->mpscp, info[1])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        
        
        static void IocpCallback(glp_tree *T, void *info){
            Nan::Callback* cb = (Nan::Callback*)info;
            const unsigned argc = 1;
            Local<Value> t = Tree::Instantiate(T);
            Local<Value> argv[argc] = {Nan::New<Value>(t)};
            cb->Call(argc, argv);
            Tree* host = ObjectWrap::Unwrap<Tree>(Nan::To<Object>(t).ToLocalChecked());
            host->thread = true;
            host->handle = NULL;
        };
        
        
        static bool IocpInit(glp_iocp *iocp, Local<Value> value){
            if (value->IsObject()){
                Local<Object> obj = Nan::To<Object>(value).ToLocalChecked();
                Local<Array> props = Nan::GetPropertyNames(obj).ToLocalChecked();
                for(uint32_t i = 0; i < props->Length(); i++){
                    Local<Value> key = props->Get(i);
                    Local<Value> val = obj->Get(key);
                    std::string keystr = std::string(V8TOCSTRING(key));
                    if (keystr == "msgLev"){
                        V8CHECKBOOL(!val->IsInt32(), "msgLev: should be int32");
                        iocp->msg_lev = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "brTech"){
                        V8CHECKBOOL(!val->IsInt32(), "brTech: should be int32");
                        iocp->br_tech = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "btTech"){
                        V8CHECKBOOL(!val->IsInt32(), "btTech: should be int32");
                        iocp->bt_tech = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "tolInt"){
                        V8CHECKBOOL(!val->IsNumber(), "tolInt: should be number");
                        iocp->tol_int = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "tolObj"){
                        V8CHECKBOOL(!val->IsNumber(), "tolObj: should be number");
                        iocp->tol_obj = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "tmLim"){
                        V8CHECKBOOL(!val->IsInt32(), "tmLim: should be int32");
                        iocp->tm_lim = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "outFrq"){
                        V8CHECKBOOL(!val->IsInt32(), "outFrq: should be int32");
                        iocp->out_frq = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "outDly"){
                        V8CHECKBOOL(!val->IsInt32(), "outDly: should be int32");
                        iocp->out_dly = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "ppTech"){
                        V8CHECKBOOL(!val->IsInt32(), "ppTech: should be int32");
                        iocp->pp_tech = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "mipGap"){
                        V8CHECKBOOL(!val->IsNumber(), "mipGap: should be number");
                        iocp->mip_gap = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "mirCuts"){
                        V8CHECKBOOL(!val->IsInt32(), "mirCuts: should be int32");
                        iocp->mir_cuts = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "gmiCuts"){
                        V8CHECKBOOL(!val->IsInt32(), "gmiCuts: should be int32");
                        iocp->gmi_cuts = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "covCuts"){
                        V8CHECKBOOL(!val->IsInt32(), "covCuts: should be int32");
                        iocp->cov_cuts = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "clqCuts"){
                        V8CHECKBOOL(!val->IsInt32(), "clqCuts: should be int32");
                        iocp->clq_cuts = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "presolve"){
                        V8CHECKBOOL(!val->IsInt32(), "presolve: should be int32");
                        iocp->presolve = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "binarize"){
                        V8CHECKBOOL(!val->IsInt32(), "binarize: should be int32");
                        iocp->binarize = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "fpHeur"){
                        V8CHECKBOOL(!val->IsInt32(), "fpHeur: should be int32");
                        iocp->fp_heur = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "psHeur"){
                        V8CHECKBOOL(!val->IsInt32(), "psHeur: should be int32");
                        iocp->ps_heur = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "psTmLim"){
                        V8CHECKBOOL(!val->IsInt32(), "psTmLim: should be int32");
                        iocp->ps_tm_lim = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "useSol"){
                        V8CHECKBOOL(!val->IsInt32(), "useSol: should be int32");
                        iocp->use_sol = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "alien"){
                        V8CHECKBOOL(!val->IsInt32(), "alien: should be int32");
                        iocp->alien = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                    } else if (keystr == "saveSol"){
                        V8CHECKBOOL(!val->IsString(), "saveSol: should be a string");
                        std::string solfile = std::string(V8TOCSTRING(val));
                        iocp->save_sol = new char[solfile.length()];
                        memcpy((void *)iocp->save_sol, solfile.c_str(), solfile.length());
                    } else if (keystr == "cbFunc"){
                        V8CHECKBOOL(!val->IsFunction(), "cbFunc: should be a function");
                        iocp->cb_func = IocpCallback;
                        iocp->cb_info = new Nan::Callback(Local<Function>::Cast(val));
                    } else if (keystr == "cbReasons"){
                        V8CHECKBOOL(!val->IsInt32(), "cbReason: should be int32");
                        iocp->cb_reasons = val->Int32Value(Nan::GetCurrentContext()).FromJust();
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
            V8CHECK(info.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_iocp iocp;
                      glp_init_iocp(&iocp);
                      if (info.Length() == 1)
                          if (!IocpInit(&iocp, info[0])) return;
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      V8CHECK(lp->thread, "an async operation is inprogress");
                          
                      glp_intopt(lp->handle, &iocp);
                      if (iocp.cb_info) delete (Nan::Callback*)iocp.cb_info;
                      if (iocp.save_sol) delete[] iocp.save_sol;
            )
        }
        
        class IntoptWorker : public Nan::AsyncWorker {
        public:
            IntoptWorker(Nan::Callback *callback, Problem *lp)
            : Nan::AsyncWorker(callback), lp(lp){
                glp_init_iocp(&parm);
                glp_init_mip_ctx(&ctx);
                ctx.parm = &parm;
                state = 0;
            }
            
            ~IntoptWorker(){
                if (parm.cb_info) delete (Nan::Callback*)parm.cb_info;
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
                Nan::HandleScope scope;
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
                    Nan::AsyncQueueWorker(this);
                }
            }
            
            void HandleOKCallback(){
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ctx.ret)};
                callback->Call(2, info);
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
            V8CHECK(info.Length() != 2, "Wrong number of arguments");
            V8CHECK(!(info[0]->IsObject() || info[0]->IsNull()) || !info[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            IntoptWorker *worker = new IntoptWorker(callback, lp);
            if (!IocpInit(&worker->parm, info[0])){
                worker->Destroy();
                return;
            }
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static NAN_METHOD(ReadLpSync) {
            V8CHECK(info.Length() != 1, "Wrong number of arguments");
            V8CHECK(!info[0]->IsString(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            info.GetReturnValue().Set(glp_read_lp(lp->handle, NULL, V8TOCSTRING(info[0])));
        }
        
        class ReadLpWorker : public Nan::AsyncWorker {
        public:
            ReadLpWorker(Nan::Callback *callback, Problem *lp, std::string file)
            : Nan::AsyncWorker(callback), lp(lp), file(file){
                
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_read_lp(lp->handle, NULL, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
            
        public:
            int ret;
            Problem *lp;
            std::string file;
        };
        
        static NAN_METHOD(ReadLp) {
            V8CHECK(info.Length() != 2, "Wrong number of arguments");
            V8CHECK(!info[0]->IsString() || !info[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            ReadLpWorker *worker = new ReadLpWorker(callback, lp, V8TOCSTRING(info[0]));
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static NAN_METHOD(WriteLpSync) {
            V8CHECK(info.Length() != 1, "Wrong number of arguments");
            V8CHECK(!info[0]->IsString(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            GLP_CATCH_RET(info.GetReturnValue().Set(glp_write_lp(lp->handle, NULL, V8TOCSTRING(info[0])));)
        }
        
        class WriteLpWorker : public Nan::AsyncWorker {
        public:
            WriteLpWorker(Nan::Callback *callback, Problem *lp, std::string file)
            : Nan::AsyncWorker(callback), lp(lp), file(file){
                
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_write_lp(lp->handle, NULL, file.c_str());
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
        public:
            int ret;
            Problem *lp;
            std::string file;
        };
        
        static NAN_METHOD(WriteLp) {
            V8CHECK(info.Length() != 2, "Wrong number of arguments");
            V8CHECK(!info[0]->IsString() || !info[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            WriteLpWorker *worker = new WriteLpWorker(callback, lp, V8TOCSTRING(info[0]));
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static NAN_METHOD(CheckKkt) {
            V8CHECK(info.Length() != 3, "Wrong number of arguments");
            V8CHECK(!info[0]->IsInt32() || !info[1]->IsInt32() || !info[2]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            double ae_max, re_max;
            int ae_ind, re_ind;
            GLP_CATCH_RET(glp_check_kkt(lp->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), info[1]->Int32Value(Nan::GetCurrentContext()).FromJust(), &ae_max, &ae_ind, &re_max, &re_ind);)
            
            Nan::Callback* cb = new Nan::Callback(Local<Function>::Cast(info[2]));
            const unsigned argc = 4;
            Local<Value> argv[argc] = {
                Nan::New<Number>(ae_max),
                Nan::New<Int32>(ae_ind),
                Nan::New<Number>(re_max),
                Nan::New<Int32>(re_ind)
            };
            GLP_CATCH(cb->Call(argc, argv);)
            delete cb;
        }
        
        static NAN_METHOD(PrintRangesSync) {
            V8CHECK(info.Length() != 3, "Wrong number of arguments");
            V8CHECK(!(info[0]->IsInt32Array() || info[0]->IsNull() || info[0]->IsUndefined()) ||
                !info[1]->IsInt32() || !info[2]->IsString(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            uint32_t count = 0;
            int* plist = NULL;
            int ret = 0;
            GLP_CATCH(
                if (info[0]->IsInt32Array()) {
                    Local<Int32Array> list = Local<Int32Array>::Cast(info[0]);
                    count = list->Length();
                    if (count > 1) {
                        plist = new int[count];
                        for (size_t i = 0; i < count; i++) plist[i] = list->Get(i)->Int32Value(Nan::GetCurrentContext()).FromJust();
                        count--;
                    }
                }
                      
                ret = glp_print_ranges(lp->handle, count, plist, info[1]->Int32Value(Nan::GetCurrentContext()).FromJust(), V8TOCSTRING(info[2]));
                      
            )
            if (plist) delete[] plist;
            info.GetReturnValue().Set(ret);
        }
        
        
        class PrintRangesWorker : public Nan::AsyncWorker {
        public:
            PrintRangesWorker(Nan::Callback *callback, Problem *lp, int len, int flags, char *file)
            : Nan::AsyncWorker(callback), lp(lp), len(len), flags(flags), file(file){
                if (len > 0)
                    list = new int[len];
                else
                    list = NULL;
            }
            ~PrintRangesWorker(){
                if (list) delete[] list;
            }
            
            void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
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
            V8CHECK(info.Length() != 4, "Wrong number of arguments");
            V8CHECK(!(info[0]->IsInt32Array() || info[0]->IsNull() || info[0]->IsUndefined()) ||
                !info[1]->IsInt32() || !info[2]->IsString() || !info[3]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            size_t len = 0;
            Local<Int32Array> list;
            if (info[0]->IsInt32Array()) {
                list = Local<Int32Array>::Cast(info[0]);
                len = list->Length();
            }
            
            Nan::Callback *callback = new Nan::Callback(info[3].As<Function>());
            PrintRangesWorker *worker = new PrintRangesWorker(callback, lp, len, info[1]->Int32Value(Nan::GetCurrentContext()).FromJust(), V8TOCSTRING(info[2]));
            if (len > 0) {
                for (size_t i = 0; i < len; i++) worker->list[i] = list->Get(i)->Int32Value(Nan::GetCurrentContext()).FromJust();
                worker->len--;
            }
            
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        
        static NAN_METHOD(GetBfcp) {
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            GLP_CATCH_RET(
                glp_bfcp bfcp;
                glp_get_bfcp(lp->handle, &bfcp);
                Local<Object> ret = Nan::New<Object>();
                GLP_SET_FIELD_INT32(ret, "type", bfcp.type);
                GLP_SET_FIELD_DOUBLE(ret, "pivTol", bfcp.piv_tol);
                GLP_SET_FIELD_INT32(ret, "pivLim", bfcp.piv_lim);
                GLP_SET_FIELD_INT32(ret, "suhl", bfcp.suhl);
                GLP_SET_FIELD_DOUBLE(ret, "epsTol", bfcp.eps_tol);
                GLP_SET_FIELD_INT32(ret, "nfsMax", bfcp.nfs_max);
                GLP_SET_FIELD_INT32(ret, "nrsMax", bfcp.nrs_max);
                      
                info.GetReturnValue().Set(ret);
            )
        }
        
        static NAN_METHOD(SetBfcp) {
            V8CHECK(info.Length() != 1, "Wrong number of arguments");
            V8CHECK(!(info[0]->IsObject() || info[0]->IsNull() || info[0]->IsUndefined()), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            GLP_CATCH_RET(
                      glp_bfcp bfcp;
                      glp_get_bfcp(lp->handle, &bfcp);
                      
                      if (info[0]->IsObject()){
                          Local<Object> obj = Nan::To<Object>(info[0]).ToLocalChecked();
                          Local<Array> props = Nan::GetPropertyNames(obj).ToLocalChecked();
                          for(uint32_t i = 0; i < props->Length(); i++){
                              Local<Value> key = props->Get(i);
                              Local<Value> val = obj->Get(key);
                              std::string keystr = std::string(V8TOCSTRING(key));
                              if (keystr == "type"){
                                  V8CHECK(!val->IsInt32(), "type: should be int32");
                                  bfcp.type = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                              } else if (keystr == "pivTol"){
                                  V8CHECK(!val->IsNumber(), "pivTol: should be number");
                                  bfcp.piv_tol = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                              } else if (keystr == "pivLim"){
                                  V8CHECK(!val->IsInt32(), "pivLim: should be int32");
                                  bfcp.piv_lim = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                              } else if (keystr == "suhl"){
                                  V8CHECK(!val->IsInt32(), "suhl: should be int32");
                                  bfcp.suhl = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                              } else if (keystr == "epsTol"){
                                  V8CHECK(!val->IsNumber(), "epsTol: should be number");
                                  bfcp.eps_tol = val->NumberValue(Nan::GetCurrentContext()).FromJust();
                              } else if (keystr == "nfsMax"){
                                  V8CHECK(!val->IsInt32(), "nfsMax: should be int32");
                                  bfcp.nfs_max = val->Int32Value(Nan::GetCurrentContext()).FromJust();
                              } else if (keystr == "nrsMax"){
                                  V8CHECK(!val->IsInt32(), "nrsMax: should be int32");
                                  bfcp.nrs_max = val->Int32Value(Nan::GetCurrentContext()).FromJust();
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
        
        class ScaleWorker : public Nan::AsyncWorker {
        public:
            ScaleWorker(Nan::Callback *callback, Problem *lp, int param)
            : Nan::AsyncWorker(callback), lp(lp), param(param){
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
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
            V8CHECK(info.Length() != 2, "Wrong number of arguments");
            V8CHECK(!info[0]->IsInt32() || !info[1]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
            ScaleWorker *worker = new ScaleWorker(callback, lp, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust());
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
        }
        

        class FactorizeWorker : public Nan::AsyncWorker {
        public:
            FactorizeWorker(Nan::Callback *callback, Problem *lp)
            : Nan::AsyncWorker(callback), lp(lp) {
            }
            void WorkComplete() {
                lp->thread = false;
                Nan::AsyncWorker::WorkComplete();
            }
            void Execute () {
                try {
                    ret = glp_factorize(lp->handle);
                } catch (std::string s){
                    SetErrorMessage(s.c_str());
                }
            }
            void HandleOKCallback() {
                Local<Value> info[] = {Nan::Null(), Nan::New<Int32>(ret)};
                callback->Call(2, info);
            }
        public:
            Problem *lp;
            int ret;
        };
        
        static NAN_METHOD(Factorize) {
            V8CHECK(info.Length() != 1, "Wrong number of arguments");
            V8CHECK(!info[0]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(info.Holder());
            V8CHECK(!lp->handle, "object deleted");
            V8CHECK(lp->thread, "an async operation is inprogress");
            
            Nan::Callback *callback = new Nan::Callback(info[0].As<Function>());
            FactorizeWorker *worker = new FactorizeWorker(callback, lp);
            lp->thread = true;
            Nan::AsyncQueueWorker(worker);
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
        
        
        static Nan::Persistent<FunctionTemplate> constructor;
    public:
        glp_prob *handle;
        bool thread;
    };
    
    Nan::Persistent<FunctionTemplate> Problem::constructor;
}
    
    
    
    







