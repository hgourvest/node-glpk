var glp = require('..');

var lp = new glp.Problem();
var mpl = new glp.Mathprog();
var kind = glp.SOL;

function cleanup(){
    mpl.delete();
    lp.delete();
};


function CB(func){
    return function(err, code){
        if (err) {
            console.log(err);
            cleanup();
        } else
            func(code);
    }
}

function postsolve() {
    mpl.postsolve(lp, kind, CB(function(){
        if (kind == glp.SOL)
            console.log(lp.getObjVal());
        else
            console.log(lp.mipObjVal());
        cleanup();
    }));
}

mpl.readModel("examples/gap.mod", glp.OFF, CB(function(ret){
    mpl.generate(null, function(err, ret){
        if (err) {
            console.log(err);
            console.log("line: " + mpl.getLine());
            cleanup();
            return;
        }
        mpl.buildProb(lp, CB(function(err){
            lp.simplex({presolve: glp.ON}, CB(function(err){
                if (lp.getNumInt() > 0){
                    function callback(tree){
                        if (tree.reason() == glp.IBINGO){
                            //tree.terminate();
                            
                        }
                    }
                    kind = glp.MIP;
                    lp.intopt({cbFunc: callback}, CB(postsolve));
                } else
                    postsolve()
            }));
        }));
    });
}));
    
