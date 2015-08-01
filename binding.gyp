{
  "targets": [
    {
      "target_name": "notify",
      "sources": [ "notify.cpp" ],
      "include_dirs": [ "<!(node -e \"require('nan')\")",
        "<!@(pkg-config libnotify --cflags-only-I | sed s/-I//g)"
      ],
      "libraries": [
        "<!@(pkg-config libnotify --libs)"
      ]
    }
  ]
}
