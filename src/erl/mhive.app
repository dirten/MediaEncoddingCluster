{application, mhive,
    [{description,"Media Encoding Cluster"},
     {vsn,"0.0.1"},
     {modules,[my_gen_server, client]},
     {registered,[packet_sender,encodeclient]},
     {mod, {mhive,[]}},
     {start_phases,[]}
]}.
