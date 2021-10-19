[![Build Status](https://travis-ci.org/dmitrymyadzelets/node-glpk.svg?branch=node-v12)](https://travis-ci.org/dmitrymyadzelets/node-glpk)
# node-glpk
Node.js native module for GLPK.

## Install
```sh
$ npm install glpk
```
## Example
```js
var glp = require("glpk");
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
```
