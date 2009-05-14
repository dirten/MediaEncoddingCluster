-module(libcode).

-compile(export_all).

get_mhive_version()->
  AppList=application:loaded_applications(),
  [App|_]=[App||App<-AppList,element(1, App)=:=mhive],
  element(3,App).

get_mhivesys_exe()->
  BinName=case os:type() of
            {win32,nt} ->
              "mhivesys.exe";
            {unix, _}->
              "mhivesys"
          end,
  case code:priv_dir(mhive) of
    Path when is_list(Path) ->filename:join([Path,BinName]);
    %    {error, bad_name}->"/usr/bin/valgrind --log-file=/tmp/mhivesys  --tool=memcheck --leak-check=full --show-reachable=yes "++filename:join(["priv",BinName])
    {error, bad_name}->filename:join(["bin",BinName])
  end.
    
get_privdir()->
  case code:priv_dir(mhive) of
    Path when is_list(Path) ->Path;
    {error, bad_name}->"."
  end.
