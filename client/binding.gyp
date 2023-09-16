{
  "targets": [
    {
      "target_name": "client",
      "sources": [
        "cppAddon/src/client.cc",
        "cppAddon/src/ConnectionHandler.cpp",
        "cppAddon/src/InputProtocol.cpp",
        "cppAddon/src/event.cpp",
        "cppAddon/src/StompFrame.cpp",
        "cppAddon/src/StompProtocol.cpp"
      ],
      "libraries": [
        "-lboost_system" 
      ],
      "include_dirs": [
        "/usr/include/boost",
        "cppAddon/include"
      ],
      "cflags_cc": [
        "-std=c++17",     
        "-fexceptions"  
      ]

    }
  ]
}
