{
  "targets": [
    {
      "target_name": "glpk",
      "sources": [ "src/nodeglpk.cc", "src/problem.hpp", "src/tree.hpp"],
      "cflags": [ "-fexceptions" ],
      "cflags_cc": [ "-fexceptions" ],
      "conditions": [
            ['OS=="mac"', {
                "xcode_settings": {
                    "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
                }
            }]
        ],
      "dependencies": [ "src/glpk/glpk.gyp:libglpk" ],
      "include_dirs": [ "<!(node -e \"require('nan')\")" ],
    }
  ]
}
