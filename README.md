[![Build Status](https://travis-ci.org/hgourvest/node-glpk.svg?branch=master)](https://travis-ci.org/hgourvest/node-glpk)
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
prob.readLp("todd.lpt");
prob.scale(glp.SF_AUTO);
prob.simplex({presolve: glp.ON});
if (prob.getNumInt() > 0){
  function callback(tree){
    if (tree.reason() == glp.IBINGO){
      // ...
    }
  }
  prob.intopt({cbFunc: callback});
}
prob.delete();
```
