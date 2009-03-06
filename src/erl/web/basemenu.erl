-module (basemenu).
-include ("nitrogen/include/wf.inc").
-compile(export_all).

render(_,Item) ->
	Items = [
		{media, "Media", "/web/media"},
		{encoding, "Encodings", "/web/encoding"},
		{profile, "Profiles", "/web/profile"},
		{folder, "Folders", "/web/folder"},
		{setting, "Settings", "/web/setting"},
 		{system, "System", "/web/system"}
%		{account, "Account", "/web/account"}
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