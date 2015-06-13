var glp = require('..');

glp.termOutput(true);

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
            var msg = mpl.getLastError();
            if (msg) console.log("error at line: " + mpl.getLine() + ", " + msg);
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

mpl.readModel("gap.mod", glp.OFF, CB(function(ret){
    mpl.generate(null, CB(function(err, ret){
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
    }));
}));

