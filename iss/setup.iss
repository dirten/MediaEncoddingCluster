; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{EDC20B92-4C2B-441C-AF0B-AB3D658825D8}
AppName=Media Encoding Cluster
AppVerName=Media Encoding Cluster 0.0.3
AppPublisher=CoderGrid
AppPublisherURL=http://www.codergrid.de
AppSupportURL=http://www.codergrid.de
AppUpdatesURL=http://www.codergrid.de
DefaultDirName={pf}\MediaEncodingCluster--0.0.3
DefaultGroupName=MediaEncodingCluster
Compression=lzma
SolidCompression=yes
OutputBaseFilename=MediaEncodingCluster-Setup-0.0.3


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:/Programme/MEC\bin\mhive.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:/Programme/MEC\bin\.hive.cfg"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:/Programme/MEC\bin\hive\*"; DestDir: "{app}\bin\hive"; Flags: ignoreversion
Source: "C:/Programme/MEC\res\*"; DestDir: "{app}\res"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/Programme/MEC\sql\*"; DestDir: "{app}\sql"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/Programme/MEC\web\*"; DestDir: "{app}\web"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/devel/mysql-noinstall-5.1.30-win32/mysql-5.1.30-win32\share\english\errmsg.sys"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:/devel/mysql-noinstall-5.1.30-win32/mysql-5.1.30-win32/lib/debug\libmysqld.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "C:/devel/ffmpeg-13712/lib\..\bin\avcodec-52.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "C:/devel/ffmpeg-13712/lib\..\bin\avdevice-52.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "C:/devel/ffmpeg-13712/lib\..\bin\avformat-52.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "C:/devel/ffmpeg-13712/lib\..\bin\avutil-49.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "C:/devel/ffmpeg-13712/lib\..\bin\SDL.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "C:/devel/ffmpeg-13712/lib\..\bin\swscale-0.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\start Server"; Filename: "{app}\bin\mhive.exe" ; Parameters: "-s -r ..\web"; WorkingDir: "{app}\bin"
Name: "{group}\start Client"; Filename: "{app}\bin\mhive.exe" ; Parameters: "-s -r ..\web"; WorkingDir: "{app}\bin"
Name: "{group}\{cm:ProgramOnTheWeb,Administration}"; Filename: "http://localhost:8080"
Name: "{group}\{cm:ProgramOnTheWeb,Media Encoding Cluster}"; Filename: "http://www.codergrid.de"
Name: "{group}\{cm:UninstallProgram,Media Encoding Cluster}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Media Encoding Cluster"; Filename: "{app}\bin\mhive.exe"; Tasks: desktopicon

