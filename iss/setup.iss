[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{EDC20B92-4C2B-441C-AF0B-AB3D658825D8}
AppName=Media Encoding Cluster
AppVerName=Media Encoding Cluster 0.0.4.6
AppPublisher=CoderGrid
AppPublisherURL=http://www.codergrid.de
AppSupportURL=http://www.codergrid.de
AppUpdatesURL=http://www.codergrid.de
DefaultDirName={pf}\MediaEncodingCluster
DefaultGroupName=MediaEncodingCluster
Compression=lzma
SolidCompression=yes
OutputBaseFilename=MediaEncodingCluster-Setup-0.0.4.6
RestartIfNeededByRun=no

[Code]
function MyConst(Param: String): String;
begin
  StringChangeEx(Param,'\','/',True);
  Result:=Param;
end;
function ShouldInstallMSVC90: Boolean;
var
  MS, LS: Cardinal;
begin
  Result := False;
  if GetVersionNumbers(ExpandConstant('{sys}\comctl32.dll'), MS, LS) then
    if MS < $00050050 then
      Result := True;
end;
function ShouldInstallTest: Boolean;
var
  FilesFound: Integer;
  FindRec: TFindRec;
begin
  FilesFound := 0;
  if FindFirst(ExpandConstant('{win}\WinSxS\*.dll'), FindRec) then begin
    try
      repeat
        // Don't count directories
        if FindRec.Attributes and FILE_ATTRIBUTE_DIRECTORY = 0 then
          FilesFound := FilesFound + 1;
      until not FindNext(FindRec);
    finally
      FindClose(FindRec);
    end;
  end;
  MsgBox(IntToStr(FilesFound) + ' files found in the System directory.',
    mbInformation, MB_OK);
  Result:=False;
end;
 {
;[Components]
;Name: "runtime"; Description: "This is the Base Runtime System"; Types: full
;Name: "base/server"; Description: "Configure System as Server"; Types: full compact
;Name: "base/client"; Description: "Configure System as Client"; Types: full compact
  }

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
;Name: "german"; MessagesFile: "compiler:languages/German.isl"

[Tasks]
;Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
;Name: "config"; Description: "How to configure the System"; GroupDescription: "System Config";
;Name: "baseserver"; Description: "Configure System as Server"; GroupDescription: "System Config";
;Name: "baseclient"; Description: "Configure System as Client"; GroupDescription: "System Config";
;Name: "base\server"; Description: "Configure System as Server"; GroupDescription: "System Config"; Flags: unchecked
;Name: "base\client"; Description: "Configure System as Client"; GroupDescription: "System Config"; Flags: unchecked

[Run]
Filename: "{tmp}\vcredist_x86.exe"; Parameters: "/q"
;Filename: "{tmp}\vcredist_x86-2005-sp1.exe"; Parameters: "/q"
;Filename: "{app}/erts-/bin/epmd.exe" ; Parameters: "-daemon"
;Filename: "{app}/erts-/bin/erl.exe" ; Parameters: "-config releases/0.0.4.6/sys -eval ""setup:setup_windows_service(),init:stop()."" "; WorkingDir: "{app}"
;Filename: "{app}/erts-/bin/erl.exe" ; Parameters: "-setcookie default -config releases/0.0.4.6/sys -eval ""setup:setup_win32(client,yes),init:stop()."" "; WorkingDir: "{app}"; Tasks: baseclient and not baseserver
;Filename: "{app}/erts-/bin/erl.exe" ; Parameters: "-setcookie default -config releases/0.0.4.6/sys -eval ""setup:setup_win32(both,yes),init:stop()."" "; WorkingDir: "{app}"; Tasks: baseserver and baseclient
;Filename: "{app}/erts-/bin/erlsrv"; Parameters: "start MHiveService"; Description: "Start the MHiveService now?"; Flags:postinstall;
Filename: "sc"; Parameters: "stop MHiveService"; Description: "stop MHiveService."
Filename: "sc"; Parameters: "delete MHiveService"; Description: "delete MHive from Service DB."
Filename: "sc"; Parameters: "create MHiveService binPath= ""{app}\bin\mhive.exe -d"" "; Description: "Install MHive as a Service."
[UninstallRun]
Filename: "sc"; Parameters: "stop MHiveService"
Filename: "sc"; Parameters: "delete MHiveService"
;Filename: "{app}/erts-/bin/erlsrv"; Parameters: "remove MHiveService";

[Files]
;Source: "/releases/0.0.4.6/tmp/*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "/wwwroot/*"; DestDir: "{app}/wwwroot"; Excludes: ".svn";Flags: ignoreversion recursesubdirs createallsubdirs
Source: "c:\vcredist_x86.exe"; DestDir: "{tmp}"
;Source: "c:\vcredist_x86-2005-sp1.exe"; DestDir: "{tmp}"

;Source: ""; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: ""; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "H:/MediaEncodingCluster-tron-Debug-Vc\src\mhive.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "Y:/MediaEncodingCluster\res\*"; DestDir: "{app}\res"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Y:/MediaEncodingCluster\sql\*"; DestDir: "{app}\sql"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Y:/MediaEncodingCluster\web\*"; DestDir: "{app}\web"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "\share\english\errmsg.sys"; DestDir: "{app}\res"; Flags: ignoreversion
Source: "\Embedded\DLL\release\libmysqld.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "\bin\avcodec-52.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "\bin\avdevice-52.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "\bin\avformat-52.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "\bin\avutil-50.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "\bin\swscale-0.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
;Name: "{group}\start Runtime"; Filename: "{app}\erts-\bin\erl.exe" ; Parameters: "-setcookie default -config releases/0.0.4.6/sys -name 'console' -eval ""application:start(sasl), user_default:startserver(), init:stop()."" "; WorkingDir: "{app}"
;Name: "{group}\stop Runtime"; Filename: "{app}\erts-\bin\erl.exe" ; Parameters: "-setcookie default -config releases/0.0.4.6/sys -name 'console' -eval ""application:start(sasl), user_default:stopserver(), init:stop()."" "; WorkingDir: "{app}"
;Name: "{group}\Client\start Client"; Filename: "{app}\erts-5.6.5\bin\erl.exe" ; Parameters: "-setcookie default releases/0.0.4.1/sys -name 'console' -eval ""application:start(sasl), user_default:startclient(), init:stop()."" "; WorkingDir: "{app}"
;Name: "{group}\Client\stop Client"; Filename: "{app}\erts-5.6.5\bin\erl.exe" ; Parameters: "-setcookie default -config releases/0.0.4.1/sys -name 'console' -eval ""application:start(sasl), user_default:stopclient(), init:stop()."" "; WorkingDir: "{app}"
;Name: "{group}\start Console"; Filename: "{app}\erts-\bin\werl.exe" ; Parameters: "-sname mhive_console -setcookie default -config releases/0.0.4.6/sys"; WorkingDir: "{app}"
Name: "{group}\start MHiveService"; Filename: "sc";Parameters: "start MHiveService"
Name: "{group}\stop MHiveService"; Filename: "sc";Parameters: "stop MHiveService"
Name: "{group}\start Local MHive Client"; Filename: "{app}\bin\mhive.exe";Parameters: "-i";WorkingDir: "{app}"
Name: "{group}\Web Administration"; Filename: "http://localhost:8080/"
Name: "{group}\{cm:ProgramOnTheWeb,Media Encoding Cluster}"; Filename: "http://www.codergrid.de"
Name: "{group}\{cm:UninstallProgram,Media Encoding Cluster}"; Filename: "{uninstallexe}"
;Name: "{commondesktop}\Media Encoding Cluster"; Filename: "{app}\bin\mhive.exe"; Tasks: desktopicon

