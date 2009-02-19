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
     {registered,[file_scanner,encodeclient]},
     {applications, [kernel, stdlib]},
     {mod, {mhive,[]}},
     {start_phases,[]}
]}.
