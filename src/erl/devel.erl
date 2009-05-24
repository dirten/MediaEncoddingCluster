-module(devel).
-compile(export_all).
-include_lib("kernel/include/file.hrl").

rebuild(Version)->
  clean(Version),
  build(Version).

update()->
  {ok, [Release]} = file:consult("VERSION"),
  FromVersion=element(1,Release),
  ToVersion=element(2,Release),
  io:format("Building Update from Version:~p to Version ~p~n",[FromVersion, ToVersion]),
  update(FromVersion, ToVersion).

build()->
  {ok, [Release]} = file:consult("VERSION"),
  Version=element(2,Release),
  io:format("Building Version:~p~n",[Version]),
  build(Version).
  
build(Version)->
  code:add_patha("releases/"++Version++"/ebin"),
  code:add_patha("releases/"++Version),
  file:make_dir("releases/"++Version++"/ebin"),
  make:all([load,{outdir,"releases/"++Version++"/ebin"}]).

clean()->
  {ok, [Release]} = file:consult("VERSION"),
  Version=element(2,Release),
  io:format("Cleaning Version:~p~n",[Version]),
  clean(Version).
clean(Version)->
  remove_dir_tree("releases/"++Version++"/ebin"),
  file:make_dir("releases/"++Version++"/ebin").

release()->
  {ok, [Release]} = file:consult("VERSION"),
  Version=element(2,Release),
  io:format("Building Version:~p~n",[Version]),
  release(Version).

release(Version)->

  RelDir="releases/"++Version,

  code:add_patha(RelDir++"/ebin"),
  code:add_patha(RelDir),
  io:format("~p~n",[code:get_path()]),
  {ok,SrcDir}=file:get_cwd(),
  file:set_cwd(RelDir),
  RelFileName="mhive-"++Version,
  RelFile = RelFileName++ ".rel",

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  %% Reading Release File
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  io:fwrite("Reading file: \"~s\" ...~n", [RelFile]),
  {ok, [RelSpec]} = file:consult(RelFile),
  io:fwrite("Creating file: \"~s\" from \"~s\" ...~n",["plain.rel", RelFile]),
  {release,
   {RelName, RelVsn},
   {erts, ErtsVsn},
   AppVsns} = RelSpec,

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  %% Create Plain Release File
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  PlainRelSpec = {release,
                  {RelName, RelVsn},
                  {erts, ErtsVsn},
                  lists:filter(fun({kernel, _}) ->
                                   true;
                                 ({stdlib, _}) ->
                                   true;
                                 (_) ->
                                   false
                               end, AppVsns)
                 },
  {ok, Fd} = file:open("plain.rel", [write]),
  io:fwrite(Fd, "~p.~n", [PlainRelSpec]),
  file:close(Fd),
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  %% Build Tar Release
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  file:delete(filename:join(["ebin", "mhive.app"])),
  file:delete(filename:join(["ebin", "mhive_client.app"])),

  libfile:copy("mhive.app", filename:join(["ebin", "mhive.app"])),
  %  libfile:copy("mhive_client.app", filename:join(["ebin", "mhive_client.app"])),

  io:fwrite("Creating directory \"tmp\" ...~n"),
  file:make_dir("tmp"),
  file:make_dir("priv"),
  RootDir = code:root_dir(),
  TmpBinDir = filename:join(["tmp", "bin"]),
  ErtsBinDir = filename:join([RootDir, "erts-" ++ ErtsVsn, "bin"]),
  file:make_dir(TmpBinDir),
  file:make_dir(ErtsBinDir),

  case os:type() of
    {win32,nt} ->
      libfile:copy(filename:join([ErtsBinDir, "epmd.exe"]),filename:join([TmpBinDir, "epmd.exe"])),
      libfile:copy(SrcDir++"/bin/Debug/mhivesys.exe", filename:join(["priv", "mhivesys.exe"]));
    {unix, _}->
      libfile:copy(filename:join([ErtsBinDir, "epmd"]),filename:join([TmpBinDir, "epmd"])),
      libfile:copy(filename:join([ErtsBinDir, "run_erl"]),filename:join([TmpBinDir, "run_erl"])),
      libfile:copy(filename:join([ErtsBinDir, "to_erl"]),filename:join([TmpBinDir, "to_erl"])),
      libfile:copy(SrcDir++"/bin/mhivesys", filename:join(["priv", "mhivesys"])),
      libfile:copy(filename:join([SrcDir,"..", "mectl"]),filename:join([TmpBinDir, "mectl"]))
  end,
  libfile:copy(SrcDir++"/default.data", filename:join(["priv", "default.data"])),

  io:fwrite("Making \"plain.script\" and \"plain.boot\" files ...~n"),
  make_script("plain"),
  io:fwrite("Making \"~s.script\" and \"~s.boot\" files ...~n", [RelFileName, RelFileName]),
  make_script(RelFileName),

  TarFileName = io_lib:fwrite("~s.tar.gz", [RelFileName]),
  io:fwrite("Creating tar file \"~s\" ...~n", [TarFileName]),

  file:make_dir("priv/wwwroot"),
  file:make_dir("priv/wwwroot/tmp"),
  libfile:copy_dir("../../wwwroot","priv/wwwroot/tmp",[".svn","logs"]),
  remove_dir_tree("priv/wwwroot/tmp"),

  systools:make_tar(RelFileName, [{erts, RootDir},{path,["./ebin"]},{dirs,[priv]}]),
%  make_tar(RelFileName),


  io:fwrite("Extracting \"~s\" into directory \"tmp\" ...~n", [TarFileName]),
  extract_tar(TarFileName, "tmp"),

  io:fwrite("Deleting \"erl\" and \"start\" in directory \"~s\" ...~n",
            [ErtsBinDir]),
%  file:delete(filename:join([ErtsBinDir, "erl"])),
%  file:delete(filename:join([ErtsBinDir, "start"])),


  %    io:fwrite("Copying file \"plain.boot\" to \"~s\" ...~n",
  %              [filename:join([TmpBinDir, "start.boot"])]),
  libfile:copy("plain.boot", filename:join([TmpBinDir, "start.boot"])),

  %    io:fwrite("Copying files \"epmd\", \"run_erl\" and \"to_erl\" from \n"
  %              "\"~s\" to \"~s\" ...~n",
  %              [ErtsBinDir, TmpBinDir]),
  
  %    file:make_dir("tmp/config"),
  file:make_dir("tmp/logs"),
  libfile:touch("tmp/logs/empty_file"),
  file:make_dir("tmp/data"),
  libfile:touch("tmp/data/empty_file"),
  libfile:copy(filename:join([SrcDir, "sys.config"]),filename:join(["tmp","releases",Version, "sys.config"])),
  %    copy_file("logger.config", filename:join(["tmp/config", "logger.config"]),[preserve]),
  %    copy_file("mhive_client.app", filename:join(["tmp/lib",string:to_lower(RelName)++"-"++RelVsn,"ebin", "mhive_client.app"])),

  StartErlDataFile = filename:join(["tmp", "releases", "start_erl.data"]),
  io:fwrite("Creating \"~s\" ...~n", [StartErlDataFile]),
  StartErlData = io_lib:fwrite("~s ~s~n", [ErtsVsn, RelVsn]),
  libfile:write(StartErlDataFile, StartErlData),

  io:fwrite("Recreating tar file \"~s\" from contents in directory "
    "\"tmp\" ...~n", [TarFileName]),


  libfile:touch("tmp/.hosts.erlang"),
  {ok, Tar} = erl_tar:open(TarFileName, [write, compressed]),
  {ok, Cwd} = file:get_cwd(),
  file:set_cwd("tmp"),
  file:delete("bin/erl"),
  erl_tar:add(Tar, ".hosts.erlang", []),
  erl_tar:add(Tar, "bin", []),
  erl_tar:add(Tar, "erts-" ++ ErtsVsn, []),
  erl_tar:add(Tar, "releases", []),
  erl_tar:add(Tar, "lib", []),
  %    erl_tar:add(Tar, "config", []),
  erl_tar:add(Tar, "data", []),
  erl_tar:add(Tar, "logs", [verbose]),
  %    erl_tar:add(Tar, "wwwroot", []),
  erl_tar:close(Tar),
  file:set_cwd(Cwd),
  io:fwrite("Removing directory \"tmp\" ...~n"),
  %  remove_dir_tree("tmp"),
  file:set_cwd(SrcDir),
  ok.

%% LOCALS
update(FromVersion, ToVersion)->
  systools:make_relup("mhive-"++ToVersion, ["mhive-"++FromVersion], ["mhive-"++FromVersion],[{path,["releases/"++FromVersion,"releases/"++ToVersion,"releases/"++ToVersion++"/ebin"]},{outdir,"releases/"++ToVersion}]),
  systools:make_script("mhive-"++ToVersion,[{path,["releases/"++ToVersion++"/ebin"]}]),

  RelDir="releases/"++ToVersion,
  {ok,SrcDir}=file:get_cwd(),
  file:set_cwd(RelDir),
  systools:make_tar("mhive-"++ToVersion,[{path,["./ebin"]}]),
  file:set_cwd(SrcDir).


%% make_script(RelFileName)
%%
make_script(RelFileName) ->
  Opts = [no_module_tests],
  systools:make_script(RelFileName, Opts).

%% make_tar(RelFileName)
%%
make_tar(RelFileName) ->
  RootDir = code:root_dir(),
  systools:make_tar(RelFileName, [{erts, RootDir},{path,["./ebin"]},{dirs,["priv"]}]).
remove_dir_tree(Dir) ->
  remove_all_files(".", [Dir]).

remove_all_files(Dir, Files) ->
  lists:foreach(fun(File) ->
                    FilePath = filename:join([Dir, File]),
                    {ok, FileInfo} = file:read_file_info(FilePath),
                    case FileInfo#file_info.type of
                      directory ->
                        {ok, DirFiles} = file:list_dir(FilePath),
                        remove_all_files(FilePath, DirFiles),
                        file:del_dir(FilePath);
                      _ ->
                        file:delete(FilePath)
                    end
                end, Files).
%% extract_tar(TarFile, DestDir)
%%
extract_tar(TarFile, DestDir) ->
  erl_tar:extract(TarFile, [{cwd, DestDir}, compressed]).