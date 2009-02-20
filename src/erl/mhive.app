{application, mhive,
    [{description,"Media Encoding Cluster"},
     {vsn,"0.0.1"},
     {modules,[
      mhive,
      packet_server, 
      packet_stack,
      file_port,
      file_scanner, 
      file_scanner_sup,
      file_export,
      libdb, 
      libfile, 
      client]},
     {registered,[file_scanner_loop,packet_server]},
     {applications, [kernel, stdlib]},
     {mod, {mhive,[]}},
     {env, [
        {platform, inets},
        {port, 9000},
        {session_timeout, 20},
        {sign_key, "b37ca07"},
        {wwwroot, "wwwroot"}
      ]}
]}.
