
#include <node.h>

#include "tree.hpp"
#include "problem.hpp"
#include "mathprog.hpp"
#include "common.h"

#include "glpk/glpk.h"

using namespace v8;
using namespace NodeGLPK;

extern "C" {
    
    NAN_METHOD(Version) {
        NanScope();
        GLP_CATCH_RET(NanReturnValue(NanNew<String>(glp_version()));)
    }
    
    NAN_METHOD(TermOut) {
        NanScope();
        
        V8CHECK(args.Length() != 1, "Wrong number of arguments");
        V8CHECK(!args[0]->IsInt32(), "Wrong arguments");
        
        GLP_CATCH_RET(NanReturnValue(NanNew<Int32>(glp_term_out(args[0]->Int32Value())));)
    }

    int _TermHook(void *info, const char *s){
        NanCallback* cb = reinterpret_cast<NanCallback*>(info);
        Local<Value> argv[1] = {NanNew<String>(s)};
        Local<Value> ret = cb->Call(1, argv);
        return (ret->IsBoolean() && ret->BooleanValue())?1:0;
    }
    
    NAN_METHOD(TermHook) {
        NanScope();
        V8CHECK(args.Length() != 1, "Wrong number of arguments");
        V8CHECK(!(args[0]->IsFunction() || args[0]->IsNull()), "Wrong arguments");
        
        if (args[0]->IsFunction())
            GLP_CATCH_RET(glp_term_hook(_TermHook, new NanCallback(Local<Function>::Cast(args[0])));)
        else
            GLP_CATCH_RET(glp_term_hook(NULL, NULL);)
    }
                          
    NAN_METHOD(MemLimit) {
        NanScope();
        
        V8CHECK(args.Length() != 1, "Wrong number of arguments");
        V8CHECK(!args[0]->IsInt32(), "Wrong arguments");
        
        GLP_CATCH_RET(glp_mem_limit(args[0]->Int32Value());)
    }
    
    void _ErrorHook(void *info){
        throw 0;
    }
    
    void Init(Handle<Object> exports) {
        glp_init_env();
        glp_error_hook(_ErrorHook, NULL);
        
        
        exports->Set(NanNew<String>("version"), NanNew<FunctionTemplate>(Version)->GetFunction());
        exports->Set(NanNew<String>("termOut"), NanNew<FunctionTemplate>(TermOut)->GetFunction());
        exports->Set(NanNew<String>("termHook"), NanNew<FunctionTemplate>(TermHook)->GetFunction());
        exports->Set(NanNew<String>("memLimit"), NanNew<FunctionTemplate>(MemLimit)->GetFunction());
                
        GLP_DEFINE_CONSTANT(exports, GLP_MAJOR_VERSION, MAJOR_VERSION);
        GLP_DEFINE_CONSTANT(exports, GLP_MINOR_VERSION, MINOR_VERSION);
        
        GLP_DEFINE_CONSTANT(exports, GLP_MIN, MIN);
        GLP_DEFINE_CONSTANT(exports, GLP_MAX, MAX);
        
        GLP_DEFINE_CONSTANT(exports, GLP_CV, CV);
        GLP_DEFINE_CONSTANT(exports, GLP_IV, IV);
        GLP_DEFINE_CONSTANT(exports, GLP_BV, BV);
        
        GLP_DEFINE_CONSTANT(exports, GLP_FR, FR);
        GLP_DEFINE_CONSTANT(exports, GLP_LO, LO);
        GLP_DEFINE_CONSTANT(exports, GLP_UP, UP);
        GLP_DEFINE_CONSTANT(exports, GLP_DB, DB);
        GLP_DEFINE_CONSTANT(exports, GLP_FX, FX);
        
        GLP_DEFINE_CONSTANT(exports, GLP_BS, BS);
        GLP_DEFINE_CONSTANT(exports, GLP_NL, NL);
        GLP_DEFINE_CONSTANT(exports, GLP_NU, NU);
        GLP_DEFINE_CONSTANT(exports, GLP_NF, NF);
        GLP_DEFINE_CONSTANT(exports, GLP_NS, NS);
        
        GLP_DEFINE_CONSTANT(exports, GLP_SF_GM, SF_GM);
        GLP_DEFINE_CONSTANT(exports, GLP_SF_EQ, SF_EQ);
        GLP_DEFINE_CONSTANT(exports, GLP_SF_2N, SF_2N);
        GLP_DEFINE_CONSTANT(exports, GLP_SF_SKIP, SF_SKIP);
        GLP_DEFINE_CONSTANT(exports, GLP_SF_AUTO, SF_AUTO);
        
        GLP_DEFINE_CONSTANT(exports, GLP_SOL, SOL);
        GLP_DEFINE_CONSTANT(exports, GLP_IPT, IPT);
        GLP_DEFINE_CONSTANT(exports, GLP_MIP, MIP);
        
        GLP_DEFINE_CONSTANT(exports, GLP_UNDEF, UNDEF);
        GLP_DEFINE_CONSTANT(exports, GLP_FEAS, FEAS);
        GLP_DEFINE_CONSTANT(exports, GLP_INFEAS, INFEAS);
        GLP_DEFINE_CONSTANT(exports, GLP_NOFEAS, NOFEAS);
        GLP_DEFINE_CONSTANT(exports, GLP_OPT, OPT);
        GLP_DEFINE_CONSTANT(exports, GLP_UNBND, UNBND);
        
        GLP_DEFINE_CONSTANT(exports, GLP_BF_LUF, BF_LUF);
        GLP_DEFINE_CONSTANT(exports, GLP_BF_BTF, BF_BTF);
        
        GLP_DEFINE_CONSTANT(exports, GLP_MSG_OFF, MSG_OFF);
        GLP_DEFINE_CONSTANT(exports, GLP_MSG_ERR, MSG_ERR);
        GLP_DEFINE_CONSTANT(exports, GLP_MSG_ON, MSG_ON);
        GLP_DEFINE_CONSTANT(exports, GLP_MSG_ALL, MSG_ALL);
        GLP_DEFINE_CONSTANT(exports, GLP_MSG_DBG, MSG_DBG);
        
        GLP_DEFINE_CONSTANT(exports, GLP_PRIMAL, PRIMAL);
        GLP_DEFINE_CONSTANT(exports, GLP_DUALP, DUALP);
        GLP_DEFINE_CONSTANT(exports, GLP_DUAL, DUAL);
        
        GLP_DEFINE_CONSTANT(exports, GLP_PT_STD, PT_STD);
        GLP_DEFINE_CONSTANT(exports, GLP_PT_PSE, PT_PSE);
        
        GLP_DEFINE_CONSTANT(exports, GLP_RT_STD, RT_STD);
        GLP_DEFINE_CONSTANT(exports, GLP_RT_HAR, RT_HAR);
        
        GLP_DEFINE_CONSTANT(exports, GLP_ORD_NONE, ORD_NONE);
        GLP_DEFINE_CONSTANT(exports, GLP_ORD_QMD, ORD_QMD);
        GLP_DEFINE_CONSTANT(exports, GLP_ORD_AMD, ORD_AMD);
        GLP_DEFINE_CONSTANT(exports, GLP_ORD_SYMAMD, ORD_SYMAMD);
        
        GLP_DEFINE_CONSTANT(exports, GLP_BR_FFV, BR_FFV);
        GLP_DEFINE_CONSTANT(exports, GLP_BR_LFV, BR_LFV);
        GLP_DEFINE_CONSTANT(exports, GLP_BR_MFV, BR_MFV);
        GLP_DEFINE_CONSTANT(exports, GLP_BR_DTH, BR_DTH);
        GLP_DEFINE_CONSTANT(exports, GLP_BR_PCH, BR_PCH);
        
        GLP_DEFINE_CONSTANT(exports, GLP_BT_DFS, BT_DFS);
        GLP_DEFINE_CONSTANT(exports, GLP_BT_BFS, BT_BFS);
        GLP_DEFINE_CONSTANT(exports, GLP_BT_BLB, BT_BLB);
        GLP_DEFINE_CONSTANT(exports, GLP_BT_BPH, BT_BPH);
        
        GLP_DEFINE_CONSTANT(exports, GLP_PP_NONE, PP_NONE);
        GLP_DEFINE_CONSTANT(exports, GLP_PP_ROOT, PP_ROOT);
        GLP_DEFINE_CONSTANT(exports, GLP_PP_ALL, PP_ALL);
        
        GLP_DEFINE_CONSTANT(exports, GLP_RF_REG, RF_REG);
        GLP_DEFINE_CONSTANT(exports, GLP_RF_LAZY, RF_LAZY);
        GLP_DEFINE_CONSTANT(exports, GLP_RF_CUT, RF_CUT);
        
        GLP_DEFINE_CONSTANT(exports, GLP_RF_GMI, RF_GMI);
        GLP_DEFINE_CONSTANT(exports, GLP_RF_MIR, RF_MIR);
        GLP_DEFINE_CONSTANT(exports, GLP_RF_COV, RF_COV);
        GLP_DEFINE_CONSTANT(exports, GLP_RF_CLQ, RF_CLQ);
        
        GLP_DEFINE_CONSTANT(exports, GLP_ON, ON);
        GLP_DEFINE_CONSTANT(exports, GLP_OFF, OFF);
        
        GLP_DEFINE_CONSTANT(exports, GLP_IROWGEN, IROWGEN);
        GLP_DEFINE_CONSTANT(exports, GLP_IBINGO, IBINGO);
        GLP_DEFINE_CONSTANT(exports, GLP_IHEUR, IHEUR);
        GLP_DEFINE_CONSTANT(exports, GLP_ICUTGEN, ICUTGEN);
        GLP_DEFINE_CONSTANT(exports, GLP_IBRANCH, IBRANCH);
        GLP_DEFINE_CONSTANT(exports, GLP_ISELECT, ISELECT);
        GLP_DEFINE_CONSTANT(exports, GLP_IPREPRO, IPREPRO);
        
        GLP_DEFINE_CONSTANT(exports, GLP_NO_BRNCH, NO_BRNCH);
        GLP_DEFINE_CONSTANT(exports, GLP_DN_BRNCH, DN_BRNCH);
        GLP_DEFINE_CONSTANT(exports, GLP_UP_BRNCH, UP_BRNCH);
        
        GLP_DEFINE_CONSTANT(exports, GLP_EBADB, EBADB);
        GLP_DEFINE_CONSTANT(exports, GLP_ESING, ESING);
        GLP_DEFINE_CONSTANT(exports, GLP_ECOND, ECOND);
        GLP_DEFINE_CONSTANT(exports, GLP_EBOUND, EBOUND);
        GLP_DEFINE_CONSTANT(exports, GLP_EFAIL, EFAIL);
        GLP_DEFINE_CONSTANT(exports, GLP_EOBJLL, EOBJLL);
        GLP_DEFINE_CONSTANT(exports, GLP_EOBJUL, EOBJUL);
        GLP_DEFINE_CONSTANT(exports, GLP_EITLIM, EITLIM);
        GLP_DEFINE_CONSTANT(exports, GLP_ETMLIM, ETMLIM);
        GLP_DEFINE_CONSTANT(exports, GLP_ENOPFS, ENOPFS);
        GLP_DEFINE_CONSTANT(exports, GLP_ENODFS, ENODFS);
        GLP_DEFINE_CONSTANT(exports, GLP_EROOT, EROOT);
        GLP_DEFINE_CONSTANT(exports, GLP_ESTOP, ESTOP);
        GLP_DEFINE_CONSTANT(exports, GLP_EMIPGAP, EMIPGAP);
        GLP_DEFINE_CONSTANT(exports, GLP_ENOFEAS, ENOFEAS);
        GLP_DEFINE_CONSTANT(exports, GLP_ENOCVG, ENOCVG);
        GLP_DEFINE_CONSTANT(exports, GLP_EINSTAB, EINSTAB);
        GLP_DEFINE_CONSTANT(exports, GLP_EDATA, EDATA);
        GLP_DEFINE_CONSTANT(exports, GLP_ERANGE, ERANGE);
        
        GLP_DEFINE_CONSTANT(exports, GLP_KKT_PE, KKT_PE);
        GLP_DEFINE_CONSTANT(exports, GLP_KKT_PB, KKT_PB);
        GLP_DEFINE_CONSTANT(exports, GLP_KKT_DE, KKT_DE);
        GLP_DEFINE_CONSTANT(exports, GLP_KKT_DB, KKT_DB);
        GLP_DEFINE_CONSTANT(exports, GLP_KKT_CS, KKT_CS);
        
        GLP_DEFINE_CONSTANT(exports, GLP_MPS_DECK, MPS_DECK);
        GLP_DEFINE_CONSTANT(exports, GLP_MPS_FILE, MPS_FILE);
        
        Problem::Init(exports);
        Tree::Init(exports);
        Mathprog::Init(exports);
    }
    
}

NODE_MODULE(addon, Init)
