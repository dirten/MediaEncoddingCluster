-module(build_system).



-compile(export_all).



build()->
Files=filelib:wildcard("ebin/*.beam"),
Modules=[list_to_atom(filename:basename(F,".beam"))||F<-Files],
Data={application, mhive,
    [{description,"Media Encoding Cluster"},
     {vsn,"0.0.1"},
     {modules,Modules},
     {registered,[file_scanner_loop,packet_server]},
     {applications, [kernel, stdlib]},
     {mod, {mhive,[]}},
     {env, [
        {platform, inets},
        {port, 9000},
        {session_timeout, 20},
        {sign_key, string:strip("b37ca07")},
        {wwwroot, "wwwroot"}
      ]}
]},
 {ok,Pid}=file:open(filename:join(["rel", "test.app"]), write),
 io:fwrite(Pid,"~p.", [Data]),
 file:close(Pid),
 Modules.
