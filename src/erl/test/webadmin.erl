-module(webadmin).
%-compile(export_all).
-export([start/0, start_httpd/0, stop/0, test/3, test/2, body/3, current_encodings/3]).
start()->
  inets:start(),
{ok, Pid} = inets:start(
    httpd, [
    {port, 9090},
    {server_name,"httpd_test"},
    {server_root,"/tmp"},
    {document_root,"/tmp/htdocs"},
    {bind_address, "localhost"},
    {modules,[mod_esi]},
    {error_log_format, pretty}
           ]),
           httpd:info(Pid).
start_httpd()->
  inets:start(),
%  inets:start(httpd,[{file,"wwwroot/conf/8080.conf"}]).
  httpd:start("/home/jhoelscher/otp-13A/lib/erlang/lib/inets-5.0.13/examples/server_root/conf/8080.conf").

stop()->
  inets:stop().

record_to_json(_R,[],_C,Acc)->
  Acc;

record_to_json(Record,[H|T],C,Acc)->
  NewAcc=[{H,element(C, Record)}]++Acc,
  record_to_json(Record,T,(C+1),NewAcc).

record_to_json(Record)->
  Fields=libdb:columns(element(1, Record)),
  lists:reverse(record_to_json(Record, Fields,2,[])).



reformat([], Acc)->Acc;
reformat([H|T], Acc) when is_tuple(H)->
  NewAcc=[{struct,record_to_json(H)}]++Acc,
  reformat(T, NewAcc).
%{"page":"1","total":"101","data":[{object},{object},{object},{object},...]}

body(SessionID,Data2,Data3)->
  Data=libdb:read(file),
  F=reformat(Data,[]),
  io:format("Data:~p~n",[Data3]),
  J= mochijson:encode({struct,[{page,1},{total,length(Data)},{data,{array,F}}]}),
    mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
    mod_esi:deliver(SessionID, J).

current_encodings(SessionID,Data2,Data3)->
  Data=libdb:read(file),
  F=reformat(Data,[]),
  io:format("Data:~p~n",[Data3]),
  J= mochijson:encode({struct,[{page,1},{total,length(Data)},{data,{array,F}}]}),
    mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
    mod_esi:deliver(SessionID, J).

test(SessionID,_Data2,_Data3)->
%  io:format("Bla Function:~p",[Data2]),
  io:format("Bla Function:arity/3",[]),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
%    mod_esi:deliver(SessionID, top("new esi format test")),
    Data=libdb:read(file),
    io:format("Data:~p",[Data]),
    mod_esi:deliver(SessionID, "This new format is nice12<BR>"),
    J= mochijson:encode(Data),
    io:format("JSON:~p",[J]),
    mod_esi:deliver(SessionID,J),
%    mod_esi:deliver(SessionID, "This new format is nice123<BR>"),
%    mod_esi:deliver(SessionID, footer()).
"ende".


test(_Data1,_Data2)->
  io:format("Bla Function:arity/2",[]),

[ "Content-Type: text/plain\r\n\r\nAccept-Ranges:none\n\nsome very plain text"  ].
