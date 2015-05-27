var glp = require('..');
var lp = new glp.Problem();

lp.readLp("examples/todd.lpt");
lp.scale(glp.SF_AUTO);
lp.simplex({presolve: glp.ON});
if (lp.getNumInt() > 0){
    function callback(tree){
        if (tree.reason() == glp.IBINGO){
            tree.terminate();
            
        }
    }
    lp.intopt({cbFunc: callback});
}

lp.delete();