{
  "targets": [
    {
      "target_name": "glpk",
      "sources": [ "src/nodeglpk.cc", "src/problem.hpp", "src/tree.hpp"],
      "dependencies": [
        "src/glpk/glpk.gyp:glpk"
      ]
    }
  ]
}
