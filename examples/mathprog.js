var glp = require('..');

var lp = new glp.Problem();
var mpl = new glp.Mathprog();
var kind = glp.SOL;

function cleanup(){
    mpl.delete();
    lp.delete();
};

function postsolve(err){
    if (err){
        console.log(err);
        cleanup();
        return;
    }
    
    mpl.postsolve(lp, kind, function(err){
        if (kind == glp.SOL)
            console.log(lp.getObjVal());
        else
            console.log(lp.mipObjVal());
        cleanup();
    });
}

mpl.readModel("gap.mod", glp.OFF, function(err, ret){
    mpl.generate(null, function(err, ret){
        mpl.buildProb(lp, function(err){
            lp.simplex({presolve: glp.ON}, function(err){
                if (lp.getNumInt() > 0){
                    function callback(tree){
                        if (tree.reason() == glp.IBINGO){
                            //tree.terminate();
                            
                        }
                    }
                    kind = glp.MIP;
                    lp.intopt({cbFunc: callback}, postsolve);
                } else
                    postsolve()
            });
        });
    });
});
    
