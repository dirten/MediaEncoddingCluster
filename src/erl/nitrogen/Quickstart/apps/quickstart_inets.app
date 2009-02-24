{application, quickstart_inets, [
	{description,  "Nitrogen Quickstart Examples"},
	{mod, {quickstart, []}},
	{env, [
		{platform, inets},
		{port, 9000},
		{session_timeout, 20},
		{sign_key, "b37ca07"},
		{wwwroot, "nitrogen/Quickstart/wwwroot"}
	]}
]}.