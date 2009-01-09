-module(client).
%-compiler(export_all).
-export([fileinfo/2, streaminfo/3, packet/3, packet/4, packetgroup/4, packetgroup/5]).

  

fileinfo(Server,File)->
  listener(Server,fileinfo,File,0,0,0).

streaminfo(Server,File, Stream)->
  listener(Server,streaminfo,File, Stream,0,0).

packet(Server,File, Stream, Seek)->
  listener(Server,packet,File, Stream, Seek,0).

packet(Server,File, Stream)->
  listener(Server,packet,File, Stream, -1,0).

packetgroup(Server,File, Stream, PacketCount, Seek)->
  listener(Server,packetgroup,File, Stream, Seek, PacketCount).

packetgroup(Server,File, Stream, PacketCount)->
  listener(Server,packetgroup,File, Stream, -1, PacketCount).

listener(Server, Type, File, Stream, Seek, PacketCount)->
  %  io:format("Client listenerLoop ~w~n", [Server]),
  {fileimport,Server} ! {call, self(),{Type, File,Stream, Seek, PacketCount}},
  receive    
    stopClient ->
      io:format("Client stopped~n", []);
    {fileimport, Result} ->
    Result
,      listener(Server,Type,File,Stream, Seek, PacketCount)
  after 5000 ->
      exit(port_terminated)
  end.
