-module(file_scanner).
-behaviour(gen_server).

-export([start/0, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2,handle_info/2, code_change/3, terminate/2,loop/0, process_file_list/1, process_file/1, filter/2, diff/2]).

start()->
  spawn(?MODULE,init,[[]]).

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

stop()->
  gen_server:call(?MODULE, stop).

init([])->
  io:format("~w start up~n", [?MODULE]),
  process_flag(trap_exit, true),
  Pid=spawn(?MODULE,loop,[]),
  register(loop, Pid),
  {ok, state}.

loop()->
  receive
    stop ->
      io:format("~w stop loop~n", [?MODULE]),
      ok
    after 5000->
      FileList=filelib:wildcard("/tmp/*"),
      process_file_list(FileList),
      io:format("~w Loop entry1 ~n", [?MODULE]),
      loop()
    end.

process_file_list([T|H])->
  Files =diff([H],[T|H]),
  io:format("File ~w~n",[Files]);

%  process_file(T),
%  process_file_list(H),
%  ok;
process_file_list([])->
  ok.

process_file(File)->
  io:format("File ~s~n",[File]),
  ok.

filter(F, [H|T]) ->
    case F(H) of
        true  -> [H|filter(F, T)];
        false -> filter(F, T)
    end;
filter(_F, []) -> [].

diff(L1, L2) ->
    filter(fun(X) -> not lists:member(X, L2) end, L1).

handle_call({import_file,Thing},_From,_N)->
  io:format("~w handle_call import~w~n", [?MODULE,Thing]),
  {reply, Thing, state};

handle_call({scan,Directory},_From,_N)->
  io:format("~w handle_call scan~w~n", [?MODULE,Directory]),
  {reply, Directory, state}.


handle_cast(_Msg,N)->
  io:format("~w handle_cast~w~n", [?MODULE,N]),
  {noreply, N}.

handle_info(Info,N)->
  io:format("~w handle_info~w~n", [?MODULE,{Info,N}]),
  {noreply, N}.

terminate(Reason,_N)->
  loop ! stop,
  io:format("~w shutdown ~w~n", [?MODULE, Reason]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.

