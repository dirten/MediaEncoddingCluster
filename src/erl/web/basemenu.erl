-module (basemenu).
-include ("nitrogen/include/wf.inc").
-compile(export_all).

render(_,Item) ->
	Items = [
		{media, "Media", "media"},
		{profile, "Profiles", "profile"},
		{encoding, "Encodings", "encoding"},
		{folder, "Folders", "folder"},
		{setting, "Settings", "setting"},
		{account, "Account", "account"}
	],

	Transform = fun(DataRow, Acc) ->
%    io:format("Item:~w:~w",[Item,DataRow]),
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