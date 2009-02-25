-module(webadmin).

-export([config_data/0]).
-export([helloworld/2, files/2]).

config_data()->
  {testtool,
   [{web_data,{"TestTool","/testtool/webadmin/helloworld"}},
    {alias,{erl_alias,"/testtool",[webadmin]}}]}.

files(_Env,_Input)->
  [header()].

helloworld(Env,Input)->
  [header(),html_header(),helloworld_body(Env,Input),ets:tab2list(file),html_end()].

header() ->
  header("text/html").

header(MimeType) ->
  "Content-type: " ++ MimeType ++ "\r\n\r\n".

html_header() ->
  "<HTML>
               <HEAD>
                  <TITLE>Hello world Example </TITLE>
               </HEAD>\n".

helloworld_body(_Env,Input)->
  ["<BODY>Hello World</BODY><br>",Input,"<br>",
  filelib:wildcard("/tmp/*")].

html_end()->
  "</HTML>".