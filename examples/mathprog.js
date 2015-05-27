var glp = require('..');
var lp = new glp.Problem();
var mpl = new glp.Mathprog();

mpl.readModel("gap.mod", glp.OFF);
mpl.generate();
mpl.buildProb(lp);

lp.simplex({presolve: glp.ON});
if (lp.getNumInt() > 0){
    function callback(tree){
        if (tree.reason() == glp.IBINGO){
                //...
        }
    }
    lp.intopt({cbFunc: callback});
    mpl.postsolve(lp, glp.MIP);
} else {
    mpl.postsolve(lp, glp.SOL);
}

mpl.delete();
lp.delete();