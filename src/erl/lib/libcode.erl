-module(libcode).

-compile(export_all).


get_mhivesys_exe()->
     BinName=case os:type() of
      {win32,nt} ->
        "mhivesys.exe";
      {unix, linux}->
        "mhivesys"
    end,
  case code:priv_dir(mhive) of
    Path when is_list(Path) ->filename:join([Path,"priv",BinName]);
    {error, bad_name}->filename:join(["priv",BinName])
    end.
