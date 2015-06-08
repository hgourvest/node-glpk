var glp = require("..");
var prob = new glp.Problem();

prob.readLpSync("todd.lpt");
prob.scaleSync(glp.SF_AUTO);
prob.simplexSync({presolve: glp.ON});
if (prob.getNumInt() > 0){
    function callback(tree){
        if (tree.reason() == glp.IBINGO){
            // ...
        }
    }
    prob.intoptSync({cbFunc: callback});
}
console.log("objective: " + prob.mipObjVal());
prob.delete();