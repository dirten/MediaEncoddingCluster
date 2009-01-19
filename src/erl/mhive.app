{application, mhive,
    [{description,"Media Encoding Cluster"},
     {vsn,"0.0.1"},
     {modules,[file_port, client]},
     {registered,[packet_sender,encodeclient]},
     {mod, {mhive,[]}},
     {start_phases,[]}
]}.
