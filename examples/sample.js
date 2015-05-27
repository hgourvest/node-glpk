
var glp = require('..');

var lp = new glp.Problem();
lp.setProbName("sample");
lp.setObjDir(glp.MAX);

lp.addRows(3);
lp.setRowName(1, "p");
lp.setRowBnds(1, glp.UP, 0.0, 100.0);
lp.setRowName(2, "q");
lp.setRowBnds(2, glp.UP, 0.0, 600.0);
lp.setRowName(3, "r");
lp.setRowBnds(3, glp.UP, 0.0, 300.0);

lp.addCols(3);
lp.setColName(1, "x1");
lp.setColBnds(1, glp.LO, 0.0, 0.0);
lp.setObjCoef(1, 10.0);
lp.setColName(2, "x2");
lp.setColBnds(2, glp.LO, 0.0, 0.0);
lp.setObjCoef(2, 6.0);
lp.setColName(3, "x3");
lp.setColBnds(3, glp.LO, 0.0, 0.0);
lp.setObjCoef(3, 4.0);

var ia = new Int32Array(10);
var ja = new Int32Array(10);
var ar = new Float64Array(10);
ia[1] = 1, ja[1] = 1, ar[1] =  1.0; /* a[1,1] =  1 */
ia[2] = 1, ja[2] = 2, ar[2] =  1.0; /* a[1,2] =  1 */
ia[3] = 1, ja[3] = 3, ar[3] =  1.0; /* a[1,3] =  1 */
ia[4] = 2, ja[4] = 1, ar[4] = 10.0; /* a[2,1] = 10 */
ia[5] = 3, ja[5] = 1, ar[5] =  2.0; /* a[3,1] =  2 */
ia[6] = 2, ja[6] = 2, ar[6] =  4.0; /* a[2,2] =  4 */
ia[7] = 3, ja[7] = 2, ar[7] =  2.0; /* a[3,2] =  2 */
ia[8] = 2, ja[8] = 3, ar[8] =  5.0; /* a[2,3] =  5 */
ia[9] = 3, ja[9] = 3, ar[9] =  6.0; /* a[3,3] =  6 */
lp.loadMatrix(9, ia, ja, ar);

lp.simplex({
    msgLev: glp.MSG_ALL,
    meth: glp.DUAL,
    pricing: glp.PT_PSE,
    rTest: glp.RT_HAR,
    presolve: glp.OFF
});

var z = lp.getObjVal(),
   x1 = lp.getColPrim(1),
   x2 = lp.getColPrim(2),
   x3 = lp.getColPrim(3);

console.log("\nz = " + z + "; x1 = " + x1 + "; x2 = " + x2 + "; x3 = " + x3 + "\n");

//console.log(lp.getItCnt());
lp.delete();
