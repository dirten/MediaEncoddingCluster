-module(webadmin).
%-compile(export_all).
-export([start/0, start_httpd/0, stop/0, test/3, test/2]).
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
  inets:start(httpd,[{file,"/home/jhoelscher/otp-13A/lib/erlang/lib/inets-5.0.13/examples/server_root/conf/8080.conf"}]).
%  httpd:start("/home/jhoelscher/otp-13A/lib/erlang/lib/inets-5.0.13/examples/server_root/conf/8080.conf").

stop()->
  inets:stop().

test(SessionID,_Data2,_Data3)->
%  io:format("Bla Function:~p",[Data2]),
  io:format("Bla Function:arity/3",[]),
%    mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
%    mod_esi:deliver(SessionID, top("new esi format test")),
%    mod_esi:deliver(SessionID, "This new format is nice<BR>"),
%    mod_esi:deliver(SessionID, "This new format is nice<BR>"),
%    mod_esi:deliver(SessionID, "This new format is nice123<BR>"),
%    mod_esi:deliver(SessionID, footer()).
"Content-Type:text/plain\r\n\r\n"
.


test(_Data1,_Data2)->
  io:format("Bla Function:arity/2",[]),

[ "Content-Type: text/plain\r\n\r\nAccept-Ranges:none\n\nsome very plain text"  ].
