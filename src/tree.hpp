
#include <node.h>
#include <node_object_wrap.h>
#include "glpk/glpk.h"
#include "common.h"
#include "stdlib.h"

namespace NodeGLPK {

    using namespace v8;
    
    class Tree : public node::ObjectWrap {
    public:
        static void Init(Handle<Object> exports){
            // Prepare constructor template
            Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
            tpl->SetClassName(NanNew<String>("Tree"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            
            // prototypes
            NODE_SET_PROTOTYPE_METHOD(tpl, "reason", Reason);
            NODE_SET_PROTOTYPE_METHOD(tpl, "terminate", Terminate);
            NODE_SET_PROTOTYPE_METHOD(tpl, "treeSize", TreeSize);
            NODE_SET_PROTOTYPE_METHOD(tpl, "currNode", CurrNode);
            NODE_SET_PROTOTYPE_METHOD(tpl, "nextNode", NextNode);
            NODE_SET_PROTOTYPE_METHOD(tpl, "prevNode", PrevNode);
            NODE_SET_PROTOTYPE_METHOD(tpl, "upNode", UpNode);
            NODE_SET_PROTOTYPE_METHOD(tpl, "nodeLevel", NodeLevel);
            NODE_SET_PROTOTYPE_METHOD(tpl, "nodeBound", NodeBound);
            NODE_SET_PROTOTYPE_METHOD(tpl, "bestNode", BestNode);
            NODE_SET_PROTOTYPE_METHOD(tpl, "mipGap", MipGap);
            NODE_SET_PROTOTYPE_METHOD(tpl, "rowAttrib", RowAttrib);
            NODE_SET_PROTOTYPE_METHOD(tpl, "poolSize", PoolSize);
            NODE_SET_PROTOTYPE_METHOD(tpl, "delRow", DelRow);
            NODE_SET_PROTOTYPE_METHOD(tpl, "clearPool", ClearPool);
            NODE_SET_PROTOTYPE_METHOD(tpl, "canBranch", CanBranch);
            NODE_SET_PROTOTYPE_METHOD(tpl, "branchUpon", BranchUpon);
            NODE_SET_PROTOTYPE_METHOD(tpl, "selectNode", SelectNode);
            NODE_SET_PROTOTYPE_METHOD(tpl, "addRow", AddRow);
            NODE_SET_PROTOTYPE_METHOD(tpl, "heurSol", HeurSol);
            
            NanAssignPersistent(constructor, tpl);
        }

        static Local<Value> Instantiate(glp_tree* tree){
            Local<Function> cons = NanNew<FunctionTemplate>(constructor)->GetFunction();
            Local<Value> ret = cons->NewInstance();
            Tree* host = ObjectWrap::Unwrap<Tree>(ret->ToObject());
            host->handle = tree;
            host->thread = false;
            return ret;
        }
    private:
        explicit Tree(): node::ObjectWrap(){};
        ~Tree(){};
        
        static NAN_METHOD(New) {
            NanScope();
            
            V8CHECK(!args.IsConstructCall(), "Constructor Tree requires 'new'");
            
            GLP_CATCH_RET(
                      Tree* obj = new Tree();
                      obj->Wrap(args.This());
                      NanReturnValue(args.This());
            )
        }
        
        GLP_BIND_VALUE(Tree, Reason, glp_ios_reason);
        
        GLP_BIND_VOID(Tree, Terminate, glp_ios_terminate);
        
        static NAN_METHOD(TreeSize) {
            NanScope();
            
            Tree* host = ObjectWrap::Unwrap<Tree>(args.Holder());
            V8CHECK(!host->handle, "object deleted");
            V8CHECK(host->thread, "an async operation is inprogress");
            
            int a_cnt, n_cnt, t_cnt;
            GLP_CATCH_RET(glp_ios_tree_size(host->handle, &a_cnt, &n_cnt, &t_cnt);)
            Local<Object> ret = NanNew<Object>();
            GLP_SET_FIELD_INT32(ret, "a", a_cnt);
            GLP_SET_FIELD_INT32(ret, "n", n_cnt);
            GLP_SET_FIELD_INT32(ret, "t", t_cnt);
            
            NanReturnValue(ret);
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
            NanScope();
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsInt32(), "Wrong arguments");
            
            Tree* host = ObjectWrap::Unwrap<Tree>(args.Holder());
            V8CHECK(!host->handle, "object deleted");
            V8CHECK(host->thread, "an async operation is inprogress");
            
            glp_attr attr;
            GLP_CATCH_RET(glp_ios_row_attr(host->handle, args[0]->Int32Value(), &attr);)
            
            Local<Object> ret = NanNew<Object>();
            GLP_SET_FIELD_INT32(ret, "level", attr.level);
            GLP_SET_FIELD_INT32(ret, "origin", attr.origin);
            GLP_SET_FIELD_INT32(ret, "klass", attr.klass);
            
            NanReturnValue(ret);
        }
        
        GLP_BIND_VALUE(Tree, PoolSize, glp_ios_pool_size);
        
        static NAN_METHOD(AddRow) {
            NanScope();
            
            V8CHECK(args.Length() != 7, "Wrong number of arguments");
            V8CHECK(!args[0]->IsString() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32Array()
                    || !args[4]->IsFloat64Array() || !args[5]->IsInt32() || !args[6]->IsNumber(), "Wrong arguments");
            
            Tree* tree = ObjectWrap::Unwrap<Tree>(args.Holder());
            V8CHECK(!tree->handle, "object deleted");
            V8CHECK(tree->thread, "an async operation is inprogress");
            
            Local<Int32Array> ind = Local<Int32Array>::Cast(args[3]);
            Local<Float64Array> val = Local<Float64Array>::Cast(args[4]);
            
            unsigned int count = ind->Length();
            V8CHECK((count < 1) || (count != val->Length()), "Invalid arrays length");
            
            int* pind = (int*)malloc(count * sizeof(int));
            double* pval = (double*)malloc(count * sizeof(double));
            
            for (unsigned int i = 0; i < count; i++){
                pind[i] = ind->Int32Value();
                pval[i] = val->NumberValue();
            }
            
            count--;
            GLP_CATCH(NanReturnValue(glp_ios_add_row(tree->handle, V8TOCSTRING(args[0]), args[1]->Int32Value(),
                args[2]->Int32Value(), count, pind, pval, args[5]->Int32Value(), args[6]->NumberValue()));)
            
            free(pind);
            free(pval);
        }
        
        GLP_BIND_VOID_INT32(Tree, DelRow, glp_ios_del_row);
        
        GLP_BIND_VOID(Tree, ClearPool, glp_ios_clear_pool);
        
        GLP_BIND_VALUE_INT32(Tree, CanBranch, glp_ios_can_branch);
        
        GLP_BIND_VOID_INT32_INT32(Tree, BranchUpon, glp_ios_branch_upon);
        
        GLP_BIND_VOID_INT32(Tree, SelectNode, glp_ios_select_node);
        
        static NAN_METHOD(HeurSol) {
            NanScope();
            
            V8CHECK(args.Length() != 1, "Wrong number of arguments");
            V8CHECK(!args[0]->IsFloat64Array(), "Wrong arguments");
        
            Tree* tree = ObjectWrap::Unwrap<Tree>(args.Holder());
            V8CHECK(!tree->handle, "object deleted");
            V8CHECK(tree->thread, "an async operation is inprogress");
            
            Local<Float64Array> x = Local<Float64Array>::Cast(args[0]);
            
            int count = (int)x->Length();
            GLP_CATCH_RET(V8CHECK(count != (glp_get_num_cols(glp_ios_get_prob(tree->handle)) + 1), "Invalid arrays length");)
            
            double* px = (double*)malloc(count * sizeof(double));
            for (int i = 0; i < count; i++) px[i] = x->NumberValue();
            GLP_CATCH(NanReturnValue(glp_ios_heur_sol(tree->handle, px));)
            free(px);
        }
    public:
        static Persistent<FunctionTemplate> constructor;
        glp_tree *handle;
        bool thread;
    };
    
    Persistent<FunctionTemplate> Tree::constructor;
}
 
