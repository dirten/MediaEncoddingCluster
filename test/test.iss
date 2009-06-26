; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{95DACE35-5D89-49BD-BF67-A3C17C9B767E}
AppName=My Program
AppVerName=My Program 1.5
AppPublisher=My Company, Inc.
AppPublisherURL=http://www.example.com/
AppSupportURL=http://www.example.com/
AppUpdatesURL=http://www.example.com/
DefaultDirName={pf}\My Program
DefaultGroupName=My Program
;LicenseFile=F:\MediaEncodingCluster-svn\COPYING
;InfoAfterFile=E:\Programme\Inno Setup 5\Examples\Readme-German.txt
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "c:\Programme\Inno Setup 5\Examples\MyProg.exe"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\My Program"; Filename: "{app}\MyProg.exe"
Name: "{commondesktop}\My Program"; Filename: "{app}\MyProg.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\MyProg.exe"; Description: "{cm:LaunchProgram,My Program}";Check: checkFileVersion('c:\windows\winsxs','msvcr80.dll')

[Code]

var
  SearchPath: String;
  SearchFile: String;
  FindRec: TFindRec;
  FileFound: Boolean;
  DirDepth: Integer;

procedure SearchForFile();
var
  SearchHandle: THandle;
  SearchPathLength: Integer;
begin
  if FindFirst(SearchPath + '\*', FindRec) then begin
    try
      repeat
        // Don't count directories
        if FindRec.Attributes and FILE_ATTRIBUTE_DIRECTORY <>0 then begin
          MsgBox(FindRec.Name + ' files found in the directory.'+MyPath,mbInformation, MB_OK);
          if (FindRec.Name <> '.') and (FindRec.Name <> '..') then
            MyFindFile(MyPath+'\'+FindRec.Name)
        end;
      until (FindNext(FindRec) = False) or FileFound;
    finally
      FindClose(FindRec);
    end;
  end;
end;

function checkFileVersion(Path:String;Filename:String):Boolean;
var
  FindRec: TFindRec;
  MS, LS: Cardinal;

begin
  if FindFirst(Path + '\*', FindRec) then begin
    try
      repeat
        if (FindRec.Attributes and FILE_ATTRIBUTE_DIRECTORY) <> 0 then begin
          if (CompareStr(FindRec.Name, '.') <> 0) and (CompareStr(FindRec.Name, '..') <> 0) then begin
            checkFileVersion(Path+'\'+FindRec.Name,Filename);
          end;
        end else begin
          if (CompareStr(AnsiLowercase(FindRec.Name), AnsiLowercase(FileName)) = 0) then begin
          GetVersionNumbers(ExpandConstant('{sys}\comctl32.dll'), MS, LS);
            msgbox('Datei gefunden in:' + #13 + Path+'\'+FindRec.Name+' mit MS:'+IntToStr(MS)+' und LS:'+IntToStr(LS), mbinformation, mb_ok);
          end;
        end;
      until (FindNext(FindRec) = False);
    finally
      FindClose(FindRec);
    end;
  end;
  Result:=False;
end;
function HaveMSVCR80():Boolean;
var
  SearchPath: String;
  SearchFile: String;
begin
  SearchPath:='c:\Windows\WinSxs';
  SearchFile:='msvcr80.dll';
  
end;

function InitializeSetup: Boolean;
begin
    Result := True;
end;

