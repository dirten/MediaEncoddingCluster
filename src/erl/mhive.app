{application, mhive,
    [{description,"Media Encoding Cluster"},
     {vsn,"0.0.1"},
     {modules,[file_scanner, client]},
     {registered,[file_scanner,encodeclient]},
     {mod, {mhive,[]}},
     {start_phases,[]}
]}.
