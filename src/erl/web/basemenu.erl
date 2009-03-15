-module (basemenu).
-include ("nitrogen/include/wf.inc").
-compile(export_all).

render(_,Item) ->

	case application:get_env(mode) of
    {ok,System}->ok;
    _->System=server
      end,
%  io:format("SystemEnv:~p",[System]),
  I = [
		{media, "Media", "/web/media"},
		{encoding, "Encodings", "/web/encoding"},
		{profile, "Profiles", "/web/profile"},
		{folder, "Folders", "/web/folder"},
		{setting, "Settings", "/web/setting"},
 		{system, "System", "/web/system"}
%		{account, "Account", "/web/account"}
	],
  if System==both ->
      Items=lists:append(I,[{client, "Client Configuration", "/web/client"}]);
    System==client ->
      Items=[{client, "Client Configuration", "/web/client"}];
    true->
      Items=I
  end,
	Transform = fun(DataRow, Acc) ->
		case element(1, DataRow) == Item of
			true -> {DataRow, Acc, {link@class, selected}};
			false -> {DataRow, Acc, []}
		end
	end,

	% Response.
	#list { body=#bind {
		data=Items,
		map={ ignore, link@text, link@url },
		transform=Transform,
		body=#listitem { body=#link { id=link }}
	}}.