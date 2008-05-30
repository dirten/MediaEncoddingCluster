-module(packet_distributor).
-export([start/0, init/0]).



start()->
  spawn(?MODULE, init, []).

init()->
  register(pds, self()),
  loop().


loop()->
  receive
    {client, {getdata}} ->
      client ! data,
      loop()
  end.
  
  