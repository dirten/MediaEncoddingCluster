{application,mhive,
             [{description,"Media Encoding Cluster"},
              {vsn,"0.0.4.2"},
              {modules,[auto_update,config,
                        client_supervisor,
                        encoding_client,file_export,file_port,file_scanner,
                        libav,libdb,libfile,libweb,libcode,libnet,libutil,mhive,mhive_supervisor,
                        node_watcher, node_finder,packet_server,packet_stack,
                        sys_port,setup,scheduler,user_default,
                        mochijson, mochijson2,mochihex,mochinum
                        ]},
              {registered,[file_scanner_loop,packet_server]},
              {applications,[kernel,stdlib]},
              {mod,{mhive, [ ]}},
              {env,[{platform,inets},
                    {port,9000},
                    {session_timeout,20},
                    {sign_key,"b37ca07"}
              ]}]}.