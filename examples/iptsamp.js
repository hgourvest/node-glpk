
var glp = require('..');

var lp = new glp.Problem();
lp.readMps(glp.MPS_DECK, null, "25fv47.mps",
    function(err, ret){
        lp.interior(null, function(err){
            lp.printIpt("25fv47.txt", function(err){
                lp.delete();
            });
        });
});

