[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{EDC20B92-4C2B-441C-AF0B-AB3D658825D8}
AppName=Media Encoding Cluster
AppVerName=Media Encoding Cluster 0.0.4.1
AppPublisher=CoderGrid
AppPublisherURL=http://www.codergrid.de
AppSupportURL=http://www.codergrid.de
AppUpdatesURL=http://www.codergrid.de
DefaultDirName={pf}\MediaEncodingCluster
DefaultGroupName=MediaEncodingCluster
Compression=lzma
SolidCompression=yes
OutputBaseFilename=MediaEncodingCluster-Setup-0.0.4.1

[Code]
function MyConst(Param: String): String;
begin
  StringChangeEx(Param,'\','/',True);
  Result:=Param;
end;

[INI]
Filename: "{app}/erts-5.6.5/bin/erl.ini"; Section: "erlang"; Flags: uninsdeletesection
Filename: "{app}/erts-5.6.5/bin/erl.ini"; Section: "erlang"; Key: "Bindir"; String: "{code:MyConst|{app}}/erts-5.6.5/bin"
Filename: "{app}/erts-5.6.5/bin/erl.ini"; Section: "erlang"; Key: "Progname"; String: "erl"
Filename: "{app}/erts-5.6.5/bin/erl.ini"; Section: "erlang"; Key: "Rootdir"; String: "{code:MyConst|{app}}"

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Run]
Filename: "{tmp}\vcredist_x86.exe"

[Files]
Source: "c:\erltest\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "e:\vcredist_x86.exe"; DestDir: "{tmp}"

;Source: "F:/MediaEncodingCluster-build/src/erl/Release/mhivesys"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: ""; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: ""; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "H:/MEC\res\*"; DestDir: "{app}\res"; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "H:/MEC\sql\*"; DestDir: "{app}\sql"; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "H:/MEC\web\*"; DestDir: "{app}\web"; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "F:/ffmpeg-r13242-gpl-lstatic-win32/lib\..\bin\avcodec-52.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "F:/ffmpeg-r13242-gpl-lstatic-win32/lib\..\bin\avdevice-52.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "F:/ffmpeg-r13242-gpl-lstatic-win32/lib\..\bin\avformat-52.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "F:/ffmpeg-r13242-gpl-lstatic-win32/lib\..\bin\avutil-49.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "F:/ffmpeg-r13242-gpl-lstatic-win32/lib\..\bin\SDL.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "F:/ffmpeg-r13242-gpl-lstatic-win32/lib\..\bin\swscale-0.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\start Server"; Filename: "{app}\bin\mhive.exe" ; Parameters: "-s -r ..\web"; WorkingDir: "{app}\bin"
Name: "{group}\start Client"; Filename: "{app}\bin\mhive.exe" ; Parameters: "-i"; WorkingDir: "{app}\bin"
Name: "{group}\{cm:ProgramOnTheWeb,Administration}"; Filename: "http://localhost:8080"
Name: "{group}\{cm:ProgramOnTheWeb,Media Encoding Cluster}"; Filename: "http://www.codergrid.de"
Name: "{group}\{cm:UninstallProgram,Media Encoding Cluster}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Media Encoding Cluster"; Filename: "{app}\bin\mhive.exe"; Tasks: desktopicon

