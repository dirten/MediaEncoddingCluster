@echo off

echo Copy Nitrogen WWW files into .\content\wwwroot\nitrogen
rmdir /s /q .\wwwroot\nitrogen
mkdir .\wwwroot\nitrogen
copy ..\www\* .\wwwroot\nitrogen

echo Starting Nitrogen on Inets...
C:\Programme\erl5.6.5\bin\erl -make
C:\Programme\erl5.6.5\bin\erl -name nitrogen@127.0.0.1 -pa apps ebin include -pa ..\ebin ..\include -eval "application:start(quickstart_inets)"
