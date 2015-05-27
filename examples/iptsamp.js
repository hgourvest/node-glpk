
var glp = require('..');

var lp = new glp.Problem();
lp.readMps(glp.MPS_DECK, null, "25fv47.mps");
lp.interior();
lp.printIpt("25fv47.txt");
lp.delete();
