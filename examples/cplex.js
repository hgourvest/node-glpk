var glp = require('..');
var lp = new glp.Problem();

glp.termOutput(true);

lp.readLp("todd.lpt", function(err, ret){
    if (err) return;
    lp.scale(glp.SF_AUTO, function(err){
        lp.simplex({presolve: glp.ON}, function(err){
            if (err) console.log(err);
            if (lp.getNumInt() > 0){
                function callback(tree){
                   if (tree.reason() == glp.IBINGO){
                        console.log("BINGO: " + lp.mipObjVal());
                    }
                }
                lp.intopt({cbFunc: callback, cbReasons: glp.FBINGO}, function(err, ret){
                    if (err)
                        console.log(err);
                    else
                       console.log("objective: " + lp.mipObjVal())
                    lp.delete();
                });
            }
        });
    });
});
