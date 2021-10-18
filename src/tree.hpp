
#include <node.h>
#include <node_object_wrap.h>
#include "glpk/glpk.h"
#include "common.h"
#include "stdlib.h"

namespace NodeGLPK {

    using namespace v8;
    
    class Tree : public node::ObjectWrap {
    public:
        static void Init(Local<Object> exports){
            // Prepare constructor template
            Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
            tpl->SetClassName(Nan::New("Tree").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            
            // prototypes
            Nan::SetPrototypeMethod(tpl, "reason", Reason);
            Nan::SetPrototypeMethod(tpl, "terminate", Terminate);
            Nan::SetPrototypeMethod(tpl, "treeSize", TreeSize);
            Nan::SetPrototypeMethod(tpl, "currNode", CurrNode);
            Nan::SetPrototypeMethod(tpl, "nextNode", NextNode);
            Nan::SetPrototypeMethod(tpl, "prevNode", PrevNode);
            Nan::SetPrototypeMethod(tpl, "upNode", UpNode);
            Nan::SetPrototypeMethod(tpl, "nodeLevel", NodeLevel);
            Nan::SetPrototypeMethod(tpl, "nodeBound", NodeBound);
            Nan::SetPrototypeMethod(tpl, "bestNode", BestNode);
            Nan::SetPrototypeMethod(tpl, "mipGap", MipGap);
            Nan::SetPrototypeMethod(tpl, "rowAttrib", RowAttrib);
            Nan::SetPrototypeMethod(tpl, "poolSize", PoolSize);
            Nan::SetPrototypeMethod(tpl, "delRow", DelRow);
            Nan::SetPrototypeMethod(tpl, "clearPool", ClearPool);
            Nan::SetPrototypeMethod(tpl, "canBranch", CanBranch);
            Nan::SetPrototypeMethod(tpl, "branchUpon", BranchUpon);
            Nan::SetPrototypeMethod(tpl, "selectNode", SelectNode);
            Nan::SetPrototypeMethod(tpl, "addRow", AddRow);
            Nan::SetPrototypeMethod(tpl, "heurSol", HeurSol);
            
            //NanAssignPersistent(constructor, tpl);
            constructor.Reset(tpl);
        }

        static Local<Value> Instantiate(glp_tree* tree){
            Local<Function> cons = Nan::New<FunctionTemplate>(constructor)->GetFunction();
            Local<Value> ret = Nan::NewInstance(cons).ToLocalChecked();
            Tree* host = ObjectWrap::Unwrap<Tree>(ret->ToObject());
            host->handle = tree;
            host->thread = false;
            return ret;
        }
    private:
        explicit Tree(): node::ObjectWrap(){};
        ~Tree(){};
        
        static NAN_METHOD(New) {
            V8CHECK(!info.IsConstructCall(), "Constructor Tree requires 'new'");
            
            GLP_CATCH_RET(
                      Tree* obj = new Tree();
                      obj->Wrap(info.This());
                      info.GetReturnValue().Set(info.This());
            )
        }
        
        GLP_BIND_VALUE(Tree, Reason, glp_ios_reason);
        
        GLP_BIND_VOID(Tree, Terminate, glp_ios_terminate);
        
        static NAN_METHOD(TreeSize) {
            Tree* host = ObjectWrap::Unwrap<Tree>(info.Holder());
            V8CHECK(!host->handle, "object deleted");
            V8CHECK(host->thread, "an async operation is inprogress");
            
            int a_cnt, n_cnt, t_cnt;
            GLP_CATCH_RET(glp_ios_tree_size(host->handle, &a_cnt, &n_cnt, &t_cnt);)
            Local<Object> ret = Nan::New<Object>();
            GLP_SET_FIELD_INT32(ret, "a", a_cnt);
            GLP_SET_FIELD_INT32(ret, "n", n_cnt);
            GLP_SET_FIELD_INT32(ret, "t", t_cnt);
            
            info.GetReturnValue().Set(ret);
        }
        
        GLP_BIND_VALUE(Tree, CurrNode, glp_ios_curr_node);
        
        GLP_BIND_VALUE_INT32(Tree, NextNode, glp_ios_next_node);
        
        GLP_BIND_VALUE_INT32(Tree, PrevNode, glp_ios_prev_node);
        
        GLP_BIND_VALUE_INT32(Tree, UpNode, glp_ios_up_node);
        
        GLP_BIND_VALUE_INT32(Tree, NodeLevel, glp_ios_node_level);
        
        GLP_BIND_VALUE_INT32(Tree, NodeBound, glp_ios_node_bound);
        
        GLP_BIND_VALUE(Tree, BestNode, glp_ios_best_node);
        
        GLP_BIND_VALUE(Tree, MipGap, glp_ios_mip_gap);
        
        static NAN_METHOD(RowAttrib) {
            V8CHECK(info.Length() != 1, "Wrong number of arguments");
            V8CHECK(!info[0]->IsInt32(), "Wrong arguments");
            
            Tree* host = ObjectWrap::Unwrap<Tree>(info.Holder());
            V8CHECK(!host->handle, "object deleted");
            V8CHECK(host->thread, "an async operation is inprogress");
            
            glp_attr attr;
            GLP_CATCH_RET(glp_ios_row_attr(host->handle, info[0]->Int32Value(Nan::GetCurrentContext()).FromJust(), &attr);)
            
            Local<Object> ret = Nan::New<Object>();
            GLP_SET_FIELD_INT32(ret, "level", attr.level);
            GLP_SET_FIELD_INT32(ret, "origin", attr.origin);
            GLP_SET_FIELD_INT32(ret, "klass", attr.klass);
            
            info.GetReturnValue().Set(ret);
        }
        
        GLP_BIND_VALUE(Tree, PoolSize, glp_ios_pool_size);
        
        static NAN_METHOD(AddRow) {
            V8CHECK(info.Length() != 7, "Wrong number of arguments");
            V8CHECK(!info[0]->IsString() || !info[1]->IsInt32() || !info[2]->IsInt32() || !info[3]->IsInt32Array()
                    || !info[4]->IsFloat64Array() || !info[5]->IsInt32() || !info[6]->IsNumber(), "Wrong arguments");
            
            Tree* tree = ObjectWrap::Unwrap<Tree>(info.Holder());
            V8CHECK(!tree->handle, "object deleted");
            V8CHECK(tree->thread, "an async operation is inprogress");
            
            Local<Int32Array> ind = Local<Int32Array>::Cast(info[3]);
            Local<Float64Array> val = Local<Float64Array>::Cast(info[4]);
            
            unsigned int count = ind->Length();
            V8CHECK((count < 1) || (count != val->Length()), "Invalid arrays length");
            
            int* pind = (int*)malloc(count * sizeof(int));
            double* pval = (double*)malloc(count * sizeof(double));
            
            for (unsigned int i = 0; i < count; i++){
                pind[i] = ind->Int32Value(Nan::GetCurrentContext()).FromJust();
                pval[i] = val->NumberValue(Nan::GetCurrentContext()).FromJust();
            }
            
            count--;
            GLP_CATCH(info.GetReturnValue().Set(glp_ios_add_row(tree->handle, V8TOCSTRING(info[0]), info[1]->Int32Value(Nan::GetCurrentContext()).FromJust(),
                info[2]->Int32Value(Nan::GetCurrentContext()).FromJust(), count, pind, pval, info[5]->Int32Value(Nan::GetCurrentContext()).FromJust(), info[6]->NumberValue(Nan::GetCurrentContext()).FromJust()));)
            
            free(pind);
            free(pval);
        }
        
        GLP_BIND_VOID_INT32(Tree, DelRow, glp_ios_del_row);
        
        GLP_BIND_VOID(Tree, ClearPool, glp_ios_clear_pool);
        
        GLP_BIND_VALUE_INT32(Tree, CanBranch, glp_ios_can_branch);
        
        GLP_BIND_VOID_INT32_INT32(Tree, BranchUpon, glp_ios_branch_upon);
        
        GLP_BIND_VOID_INT32(Tree, SelectNode, glp_ios_select_node);
        
        static NAN_METHOD(HeurSol) {
            V8CHECK(info.Length() != 1, "Wrong number of arguments");
            V8CHECK(!info[0]->IsFloat64Array(), "Wrong arguments");
        
            Tree* tree = ObjectWrap::Unwrap<Tree>(info.Holder());
            V8CHECK(!tree->handle, "object deleted");
            V8CHECK(tree->thread, "an async operation is inprogress");
            
            Local<Float64Array> x = Local<Float64Array>::Cast(info[0]);
            
            int count = (int)x->Length();
            GLP_CATCH_RET(V8CHECK(count != (glp_get_num_cols(glp_ios_get_prob(tree->handle)) + 1), "Invalid arrays length");)
            
            double* px = (double*)malloc(count * sizeof(double));
            for (int i = 0; i < count; i++) px[i] = x->NumberValue(Nan::GetCurrentContext()).FromJust();
            GLP_CATCH(info.GetReturnValue().Set(glp_ios_heur_sol(tree->handle, px));)
            free(px);
        }
    public:
        static Nan::Persistent<FunctionTemplate> constructor;
        glp_tree *handle;
        bool thread;
    };
    
    Nan::Persistent<FunctionTemplate> Tree::constructor;
}
 
