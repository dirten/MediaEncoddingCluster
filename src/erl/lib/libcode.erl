-module(libcode).

-compile(export_all).


get_mhivesys_exe()->
     BinName=case os:type() of
      {win32,nt} ->
        "mhivesys.exe";
      {unix, _}->
        "mhivesys"
    end,
  case code:priv_dir(mhive) of
    Path when is_list(Path) ->filename:join([Path,BinName]);
    {error, bad_name}->filename:join(["priv",BinName])
    end.
