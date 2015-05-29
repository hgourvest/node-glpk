{
  "targets": [
    {
      "target_name": "glpk",
      "sources": [ "src/nodeglpk.cc", "src/problem.hpp", "src/tree.hpp"],

        "conditions": [
            ['OS=="mac"', {
                "xcode_settings": {
                    "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
                }
            }]
        ],
        "dependencies": [
            "src/glpk/glpk.gyp:libglpk"
      ]
    }
  ]
}
