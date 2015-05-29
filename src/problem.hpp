#include <node.h>
#include <node_object_wrap.h>
#include <stdlib.h>
#include <string>

#include "glpk/glpk.h"
#include "common.h"
//#include "tree.hpp"

namespace NodeGLPK {
    
    using namespace v8;
    
    class Problem : public node::ObjectWrap {
    public:
        static void Init(Handle<Object> exports){
            Isolate* isolate = Isolate::GetCurrent();
            
            // Prepare constructor template
            Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
            tpl->SetClassName(String::NewFromUtf8(isolate, "Problem"));
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
            NODE_SET_PROTOTYPE_METHOD(tpl, "scale", Scale);
            NODE_SET_PROTOTYPE_METHOD(tpl, "unscale", Unscale);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setRowStat", SetRowStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "setColStat", SetColStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getRowStat", GetRowStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "getColStat", GetColStat);
            NODE_SET_PROTOTYPE_METHOD(tpl, "stdBasis", StdBasis);
            NODE_SET_PROTOTYPE_METHOD(tpl, "advBasis", AdvBasis);
            NODE_SET_PROTOTYPE_METHOD(tpl, "cpxBasis", CpxBasis);
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
            NODE_SET_PROTOTYPE_METHOD(tpl, "interior", Interior);
            NODE_SET_PROTOTYPE_METHOD(tpl, "iptStatus", IptStatus);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readMps", ReadMps);
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
            NODE_SET_PROTOTYPE_METHOD(tpl, "intopt", Intopt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readProb", ReadProb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeProb", WriteProb);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readLp", ReadLp);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeLp", WriteLp);
            NODE_SET_PROTOTYPE_METHOD(tpl, "mipStatus", MipStatus);
            NODE_SET_PROTOTYPE_METHOD(tpl, "mipObjVal", MipObjVal);
            NODE_SET_PROTOTYPE_METHOD(tpl, "mipRowVal", MipRowVal);
            NODE_SET_PROTOTYPE_METHOD(tpl, "mipColVal", MipColVal);
            NODE_SET_PROTOTYPE_METHOD(tpl, "checkKkt", CheckKkt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printSol", PrintSol);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readSol", ReadSol);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeSol", WriteSol);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printRanges", PrintRanges);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printIpt", PrintIpt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readIpt", ReadIpt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "writeIpt", WriteIpt);
            NODE_SET_PROTOTYPE_METHOD(tpl, "printMip", PrintMip);
            NODE_SET_PROTOTYPE_METHOD(tpl, "readMip", ReadMip);
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
            
            constructor.Reset(isolate, tpl->GetFunction());
            exports->Set(String::NewFromUtf8(isolate, "Problem"),
                         tpl->GetFunction());
        }
        
    private:
        explicit Problem(){
            handle = glp_create_prob();
        }

        ~Problem(){
            if (handle) glp_delete_prob(handle);
        }
        
        static void New(const FunctionCallbackInfo<Value>& args){
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(!args.IsConstructCall(), "Constructor Problem requires 'new'");
            
            GLP_CATCH_RET(Problem* obj = new Problem();
                      obj->Wrap(args.This());
                      args.GetReturnValue().Set(args.This());
            )
        }

        static void LoadMatrix(const FunctionCallbackInfo<Value>& args){
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() != 4, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !args[1]->IsInt32Array()
                    || !args[2]->IsInt32Array() || !args[3]->IsFloat64Array(), "Wrong arguments");
            
            Local<Int32Array> ia = Local<Int32Array>::Cast(args[1]);
            Local<Int32Array> ja = Local<Int32Array>::Cast(args[2]);
            Local<Float64Array> ar = Local<Float64Array>::Cast(args[3]);
            
            int* pia = (int*)malloc(ia->Length() * sizeof(int));
            int* pja = (int*)malloc(ja->Length() * sizeof(int));
            double* par = (double*)malloc(ar->Length() * sizeof(double));
            
            for (size_t i = 0; i < ia->Length(); i++) pia[i] = ia->Get(i)->Int32Value();
            for (size_t i = 0; i < ja->Length(); i++) pja[i] = ja->Get(i)->Int32Value();
            for (size_t i = 0; i < ar->Length(); i++) par[i] = ar->Get(i)->NumberValue();
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            
            GLP_CATCH(glp_load_matrix(lp->handle, args[0]->Int32Value(), pia, pja, par);)
            
            free(pia);
            free(pja);
            free(par);
        }

        static void ScmpInit(Isolate* isolate, glp_smcp* scmp, Local<Value> value){
            Local<Object> obj = value->ToObject();
            Local<Array> props = obj->GetPropertyNames();
            for(uint32_t i = 0; i < props->Length(); i++){
                Local<Value> key = props->Get(i);
                Local<Value> val = obj->Get(key);
                std::string keystr = std::string(V8TOCSTRING(key));
                if (keystr == "msgLev"){
                    V8CHECK(!val->IsInt32(), "msgLev: should be int32");
                    scmp->msg_lev = val->Int32Value();
                } else if (keystr == "meth"){
                    V8CHECK(!val->IsInt32(), "meth: should be int32");
                    scmp->meth = val->Int32Value();
                } else if (keystr == "pricing"){
                    V8CHECK(!val->IsInt32(), "pricing: should be int32");
                    scmp->pricing = val->Int32Value();
                } else if (keystr == "rTest"){
                    V8CHECK(!val->IsInt32(), "rTest: should be int32");
                    scmp->r_test = val->Int32Value();
                } else if (keystr == "tolBnd"){
                    V8CHECK(!val->IsNumber(), "tolBnd: should be a Number");
                    scmp->tol_bnd = val->NumberValue();
                } else if (keystr == "tolDj"){
                    V8CHECK(!val->IsNumber(), "tolDj: should be a Number");
                    scmp->tol_dj = val->NumberValue();
                } else if (keystr == "tolPiv"){
                    V8CHECK(!val->IsNumber(), "tolPiv: should be a Number");
                    scmp->tol_piv = val->NumberValue();
                } else if (keystr == "objLl"){
                    V8CHECK(!val->IsNumber(), "objLl: should be a Number");
                    scmp->obj_ll = val->NumberValue();
                } else if (keystr == "objUl"){
                    V8CHECK(!val->IsNumber(), "objUl: should be a Number");
                    scmp->obj_ul = val->NumberValue();
                } else if (keystr == "itLim"){
                    V8CHECK(!val->IsInt32(), "itLim: should be int32");
                    scmp->it_lim = val->Int32Value();
                } else if (keystr == "tmLim"){
                    V8CHECK(!val->IsInt32(), "tmLim: should be int32");
                    scmp->tm_lim = val->Int32Value();
                } else if (keystr == "outFrq"){
                    V8CHECK(!val->IsInt32(), "outFrq: should be int32");
                    scmp->out_frq = val->Int32Value();
                } else if (keystr == "outDly"){
                    V8CHECK(!val->IsInt32(), "outDly: should be int32");
                    scmp->out_dly = val->Int32Value();
                } else if (keystr == "presolve"){
                    V8CHECK(!val->IsInt32(), "presolve: should be int32");
                    scmp->presolve = val->Int32Value();
                } else {
                    std::string error("Unknow field: ");
                    error += keystr;
                    V8CHECK(true, error.c_str());
                }
            }
        }

        static void Simplex(const FunctionCallbackInfo<Value>& args){
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_smcp scmp;
                      glp_init_smcp(&scmp);
                      if (args.Length() == 1) {
                          if (args[0]->IsObject()){
                              ScmpInit(isolate, &scmp, args[0]);
                          }
                      }
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      
                      glp_simplex(lp->handle, &scmp);
            )
        }
        
        static void Exact(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_smcp scmp;
                      glp_init_smcp(&scmp);
                      if (args.Length() == 1) {
                          if (args[0]->IsObject()){
                              ScmpInit(isolate, &scmp, args[0]);
                          }
                      }
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      
                      glp_exact(lp->handle, &scmp);
            )
        }
        
        static void IptcpInit(Isolate* isolate, glp_iptcp* iptcp, Local<Value> value){
            Local<Object> obj = value->ToObject();
            Local<Array> props = obj->GetPropertyNames();
            for(uint32_t i = 0; i < props->Length(); i++){
                Local<Value> key = props->Get(i);
                Local<Value> val = obj->Get(key);
                std::string keystr = std::string(V8TOCSTRING(key));
                if (keystr == "msgLev"){
                    V8CHECK(!val->IsInt32(), "msgLev: should be int32");
                    iptcp->msg_lev = val->Int32Value();
                } else if (keystr == "ordAlg"){
                    V8CHECK(!val->IsInt32(), "ordAlg: should be int32");
                    iptcp->ord_alg = val->Int32Value();
                } else {
                    std::string error("Unknow field: ");
                    error += keystr;
                    V8CHECK(true, error.c_str());
                }
            }
        }
        
        static void Interior(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_iptcp iptcp;
                      glp_init_iptcp(&iptcp);
                      if (args.Length() == 1) {
                          if (args[0]->IsObject()){
                              IptcpInit(isolate, &iptcp, args[0]);
                          }
                      }
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      
                      glp_interior(lp->handle, &iptcp);
            )
        }
        
        static void ReadMps(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            std::string objname;
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !(args[1]->IsObject() || args[1]->IsNull() || args[1]->IsUndefined()) || !args[2]->IsString(), "Wrong arguments");
            
            GLP_CATCH_RET(
                      glp_mpscp mpscp;
                      glp_init_mpscp(&mpscp);
                      if (args.Length() == 1) {
                          if (args[1]->IsObject()){
                              Local<Object> obj = args[1]->ToObject();
                              Local<Array> props = obj->GetPropertyNames();
                              for(uint32_t i = 0; i < props->Length(); i++){
                                  Local<Value> key = props->Get(i);
                                  Local<Value> val = obj->Get(key);
                                  std::string keystr = std::string(V8TOCSTRING(key));
                                  if (keystr == "blank"){
                                      V8CHECK(!val->IsInt32(), "blank: should be int32");
                                      mpscp.blank = val->Int32Value();
                                  } else if (keystr == "tolMps"){
                                      V8CHECK(!val->IsNumber(), "tolMps: should be number");
                                      mpscp.tol_mps = val->NumberValue();
                                  } else if (keystr == "objName"){
                                      V8CHECK(!val->IsString(), "objName: should be a string");
                                      objname = std::string(V8TOCSTRING(val));
                                      mpscp.obj_name = (char*) objname.c_str();
                                  } else {
                                      std::string error("Unknow field: ");
                                      error += keystr;
                                      V8CHECK(true, error.c_str());
                                  }
                              }
                          }
                      }
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      
                      args.GetReturnValue().Set(glp_read_mps(lp->handle, args[0]->Int32Value(), &mpscp, V8TOCSTRING(args[2])));
            )
        }
        
        static void WriteMps(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            std::string objname;
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !(args[1]->IsObject() || args[1]->IsNull() || args[1]->IsUndefined()) || !args[2]->IsString(), "Wrong arguments");
            
            GLP_CATCH_RET(
                      glp_mpscp mpscp;
                      glp_init_mpscp(&mpscp);
                      if (args.Length() == 1) {
                          if (args[1]->IsObject()){
                              Local<Object> obj = args[1]->ToObject();
                              Local<Array> props = obj->GetPropertyNames();
                              for(uint32_t i = 0; i < props->Length(); i++){
                                  Local<Value> key = props->Get(i);
                                  Local<Value> val = obj->Get(key);
                                  std::string keystr = std::string(V8TOCSTRING(key));
                                  if (keystr == "blank"){
                                      V8CHECK(!val->IsInt32(), "blank: should be int32");
                                      mpscp.blank = val->Int32Value();
                                  } else if (keystr == "tolMps"){
                                      V8CHECK(!val->IsNumber(), "tolMps: should be number");
                                      mpscp.tol_mps = val->NumberValue();
                                  } else if (keystr == "objName"){
                                      V8CHECK(!val->IsString(), "objName: should be a string");
                                      objname = std::string(V8TOCSTRING(val));
                                      mpscp.obj_name = (char*) objname.c_str();
                                  } else {
                                      std::string error("Unknow field: ");
                                      error += keystr;
                                      V8CHECK(true, error.c_str());
                                  }
                              }
                          }
                      }
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      
                      args.GetReturnValue().Set(glp_write_mps(lp->handle, args[0]->Int32Value(), &mpscp, V8TOCSTRING(args[2])));
            )
        }
        
        typedef struct{
            Local<Function> cb;
        } CBInfo;
        
        static void IocpCallback(glp_tree *T, void *info){
            Isolate* isolate = Isolate::GetCurrent();
            Local<Function> cb = ((CBInfo*)info)->cb;
            
            const unsigned argc = 1;
            Local<Value> t = Tree::Instantiate(T);
            Local<Value> argv[argc] = {Local<Value>::New(isolate, t)};
            cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
        };
        
        static void Intopt(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            std::string solfile;
            CBInfo info;
            
            V8CHECK(args.Length() > 1, "Wrong number of arguments");
            
            GLP_CATCH_RET(
                      glp_iocp iocp;
                      glp_init_iocp(&iocp);
                      if (args.Length() == 1) {
                          if (args[0]->IsObject()){
                              Local<Object> obj = args[0]->ToObject();
                              Local<Array> props = obj->GetPropertyNames();
                              for(uint32_t i = 0; i < props->Length(); i++){
                                  Local<Value> key = props->Get(i);
                                  Local<Value> val = obj->Get(key);
                                  std::string keystr = std::string(V8TOCSTRING(key));
                                  if (keystr == "msgLev"){
                                      V8CHECK(!val->IsInt32(), "msgLev: should be int32");
                                      iocp.msg_lev = val->Int32Value();
                                  } else if (keystr == "brTech"){
                                      V8CHECK(!val->IsInt32(), "brTech: should be int32");
                                      iocp.br_tech = val->Int32Value();
                                  } else if (keystr == "btTech"){
                                      V8CHECK(!val->IsInt32(), "btTech: should be int32");
                                      iocp.bt_tech = val->Int32Value();
                                  } else if (keystr == "tolInt"){
                                      V8CHECK(!val->IsNumber(), "tolInt: should be number");
                                      iocp.tol_int = val->NumberValue();
                                  } else if (keystr == "tolObj"){
                                      V8CHECK(!val->IsNumber(), "tolObj: should be number");
                                      iocp.tol_obj = val->NumberValue();
                                  } else if (keystr == "tmLim"){
                                      V8CHECK(!val->IsInt32(), "tmLim: should be int32");
                                      iocp.tm_lim = val->Int32Value();
                                  } else if (keystr == "outFrq"){
                                      V8CHECK(!val->IsInt32(), "outFrq: should be int32");
                                      iocp.out_frq = val->Int32Value();
                                  } else if (keystr == "outDly"){
                                      V8CHECK(!val->IsInt32(), "outDly: should be int32");
                                      iocp.out_dly = val->Int32Value();
                                  } else if (keystr == "ppTech"){
                                      V8CHECK(!val->IsInt32(), "ppTech: should be int32");
                                      iocp.pp_tech = val->Int32Value();
                                  } else if (keystr == "mipGap"){
                                      V8CHECK(!val->IsNumber(), "mipGap: should be number");
                                      iocp.mip_gap = val->NumberValue();
                                  } else if (keystr == "mirCuts"){
                                      V8CHECK(!val->IsInt32(), "mirCuts: should be int32");
                                      iocp.mir_cuts = val->Int32Value();
                                  } else if (keystr == "gmiCuts"){
                                      V8CHECK(!val->IsInt32(), "gmiCuts: should be int32");
                                      iocp.gmi_cuts = val->Int32Value();
                                  } else if (keystr == "covCuts"){
                                      V8CHECK(!val->IsInt32(), "covCuts: should be int32");
                                      iocp.cov_cuts = val->Int32Value();
                                  } else if (keystr == "clqCuts"){
                                      V8CHECK(!val->IsInt32(), "clqCuts: should be int32");
                                      iocp.clq_cuts = val->Int32Value();
                                  } else if (keystr == "presolve"){
                                      V8CHECK(!val->IsInt32(), "presolve: should be int32");
                                      iocp.presolve = val->Int32Value();
                                  } else if (keystr == "binarize"){
                                      V8CHECK(!val->IsInt32(), "binarize: should be int32");
                                      iocp.binarize = val->Int32Value();
                                  } else if (keystr == "fpHeur"){
                                      V8CHECK(!val->IsInt32(), "fpHeur: should be int32");
                                      iocp.fp_heur = val->Int32Value();
                                  } else if (keystr == "psHeur"){
                                      V8CHECK(!val->IsInt32(), "psHeur: should be int32");
                                      iocp.ps_heur = val->Int32Value();
                                  } else if (keystr == "psTmLim"){
                                      V8CHECK(!val->IsInt32(), "psTmLim: should be int32");
                                      iocp.ps_tm_lim = val->Int32Value();
                                  } else if (keystr == "useSol"){
                                      V8CHECK(!val->IsInt32(), "useSol: should be int32");
                                      iocp.use_sol = val->Int32Value();
                                  } else if (keystr == "alien"){
                                      V8CHECK(!val->IsInt32(), "alien: should be int32");
                                      iocp.alien = val->Int32Value();
                                  } else if (keystr == "saveSol"){
                                      V8CHECK(!val->IsString(), "saveSol: should be a string");
                                      solfile = std::string(V8TOCSTRING(val));
                                      iocp.save_sol = solfile.c_str();
                                  } else if (keystr == "cbFunc"){
                                      V8CHECK(!val->IsFunction(), "cbFunc: should be a function");
                                      iocp.cb_func = IocpCallback;
                                      info.cb = Local<Function>::Cast(val);
                                      iocp.cb_info = &info;
                                  } else {
                                      std::string error("Unknow field: ");
                                      error += keystr;
                                      V8CHECK(true, error.c_str());
                                  }
                              }
                          }
                      }
                      
                      Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
                      V8CHECK(!lp->handle, "object deleted");
                      
                      glp_intopt(lp->handle, &iocp);
            )
        }
        
        static void ReadLp(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsString(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            
            args.GetReturnValue().Set(glp_read_lp(lp->handle, NULL, V8TOCSTRING(args[0])));
        }
        
        static void WriteLp(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsString(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            
            GLP_CATCH_RET(args.GetReturnValue().Set(glp_write_lp(lp->handle, NULL, V8TOCSTRING(args[0])));)
        }
        
        static void CheckKkt(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsFunction(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            
            double ae_max, re_max;
            int ae_ind, re_ind;
            GLP_CATCH_RET(glp_check_kkt(lp->handle, args[0]->Int32Value(), args[1]->Int32Value(), &ae_max, &ae_ind, &re_max, &re_ind);)
            
            Local<Function> cb = Local<Function>::Cast(args[2]);
            const unsigned argc = 4;
            Local<Value> argv[argc] = {
                Int32::New(isolate, ae_max),
                Number::New(isolate, ae_ind),
                Int32::New(isolate, re_max),
                Number::New(isolate, re_ind)
            };
            GLP_CATCH_RET(cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);)
        }
        
        static void PrintRanges(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() != 3, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsInt32Array() || args[0]->IsNull() || args[0]->IsUndefined()) || !args[1]->IsInt32() || !args[2]->IsString(), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            
            uint32_t count = 0;
            int* plist = NULL;
            
            GLP_CATCH(
                      if (args[0]->IsInt32Array()) {
                          Local<Int32Array> list = Local<Int32Array>::Cast(args[0]);
                          count = list->Length();
                          if (count > 1) {
                              plist = (int*)malloc(count * sizeof(int));
                              for (size_t i = 0; i < count; i++) plist[i] = list->Get(i)->Int32Value();
                              count--;
                          }
                      }
                      
                      args.GetReturnValue().Set(glp_print_ranges(lp->handle, count, plist, args[1]->Int32Value(), V8TOCSTRING(args[2])));
                      
            )
            if (plist) free(plist);
        }
        
        static void GetBfcp(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            
            GLP_CATCH_RET(
                      glp_bfcp bfcp;
                      glp_get_bfcp(lp->handle, &bfcp);
                      Local<Object> ret = Object::New(isolate);
                      GLP_SET_FIELD_INT32(ret, "type", bfcp.type);
                      GLP_SET_FIELD_DOUBLE(ret, "pivTol", bfcp.piv_tol);
                      GLP_SET_FIELD_INT32(ret, "pivLim", bfcp.piv_lim);
                      GLP_SET_FIELD_INT32(ret, "suhl", bfcp.suhl);
                      GLP_SET_FIELD_DOUBLE(ret, "epsTol", bfcp.eps_tol);
                      GLP_SET_FIELD_INT32(ret, "nfsMax", bfcp.nfs_max);
                      GLP_SET_FIELD_INT32(ret, "nrsMax", bfcp.nrs_max);
                      
                      args.GetReturnValue().Set(ret);
            )
        }
        
        static void SetBfcp(const FunctionCallbackInfo<Value>& args) {
            Isolate* isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!(args[0]->IsObject() || args[0]->IsNull() || args[0]->IsUndefined()), "Wrong arguments");
            
            Problem* lp = ObjectWrap::Unwrap<Problem>(args.Holder());
            V8CHECK(!lp->handle, "object deleted");
            
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
        
        GLP_BIND_VOID_INT32(Problem, Scale, glp_scale_prob);
        
        GLP_BIND_VOID(Problem, Unscale, glp_unscale_prob);
        
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
        
        GLP_BIND_VALUE_INT32_STR(Problem, ReadProb, glp_read_prob);
        
        GLP_BIND_VALUE_INT32_STR(Problem, WriteProb, glp_write_prob);
        
        GLP_BIND_VALUE(Problem, MipStatus, glp_mip_status);
        
        GLP_BIND_VALUE(Problem, MipObjVal, glp_mip_obj_val);
        
        GLP_BIND_VALUE_INT32(Problem, MipRowVal, glp_mip_row_val);
        
        GLP_BIND_VALUE_INT32(Problem, MipColVal, glp_mip_col_val);
        
        GLP_BIND_VALUE_STR(Problem, PrintSol, glp_print_sol);
        
        GLP_BIND_VALUE_STR(Problem, ReadSol, glp_read_sol);
        
        GLP_BIND_VALUE_STR(Problem, WriteSol, glp_write_sol);
        
        GLP_BIND_VALUE_STR(Problem, PrintIpt, glp_print_ipt);
        
        GLP_BIND_VALUE_STR(Problem, ReadIpt, glp_read_ipt);
        
        GLP_BIND_VALUE_STR(Problem, WriteIpt, glp_write_ipt);
        
        GLP_BIND_VALUE_STR(Problem, PrintMip, glp_print_mip);
        
        GLP_BIND_VALUE_STR(Problem, ReadMip, glp_read_mip);
        
        GLP_BIND_VALUE_STR(Problem, WriteMip, glp_write_mip);
        
        GLP_BIND_VALUE(Problem, BfExists, glp_bf_exists);
        
        GLP_BIND_VALUE(Problem, Factorize, glp_factorize);
        
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
        

        
        
        
        
        
        
        
        
        
        
        
        static Persistent<Function> constructor;
    public:
        glp_prob *handle;
    };
    
    Persistent<Function> Problem::constructor;
}
    
    
    
    







