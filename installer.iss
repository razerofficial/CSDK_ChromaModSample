[Setup]
AppName=C++ Chroma Mod Sample
AppVerName=C++ Chroma Mod Sample 1.0
AppPublisher=Razer, Inc.
AppPublisherURL=https://razer.com
AppSupportURL=https://razer.com
AppUpdatesURL=https://razer.com
DefaultDirName={userpf}\Razer\CSDK_ChromaModSample
DefaultGroupName=Razer\CSDK_ChromaModSample
OutputBaseFilename=CSDK_ChromaModSampleSetup
SetupIconFile=release_icon.ico
UninstallDisplayIcon=release_icon.ico
UninstallDisplayName=Uninstall C++ Chroma Mod Sample
Compression=lzma
SolidCompression=yes
InfoBeforeFile=eula.txt
PrivilegesRequired=admin
ChangesAssociations = yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "x64\Release\CSDK_ChromaModSample.exe"; DestDir: "{userpf}\Razer\CSDK_ChromaModSample"; CopyMode: alwaysoverwrite
Source: "ChromaAppInfo.xml"; DestDir: "{userpf}\Razer\CSDK_ChromaModSample"; CopyMode: alwaysoverwrite
Source: "CChromaEditorLibrary.dll"; DestDir: "{userpf}\Razer\CSDK_ChromaModSample"; CopyMode: alwaysoverwrite
Source: "CChromaEditorLibrary64.dll"; DestDir: "{userpf}\Razer\CSDK_ChromaModSample"; CopyMode: alwaysoverwrite
Source: "Animations\Base\Fighter\*"; DestDir: "{userpf}\Razer\CSDK_ChromaModSample\Animations\Base\Fighter"; CopyMode: alwaysoverwrite
Source: "Animations\Base\Paladin\*"; DestDir: "{userpf}\Razer\CSDK_ChromaModSample\Animations\Base\Paladin"; CopyMode: alwaysoverwrite
Source: "Animations\Base\Thief\*"; DestDir: "{userpf}\Razer\CSDK_ChromaModSample\Animations\Base\Thief"; CopyMode: alwaysoverwrite

[Icons]
Name: "{group}\C++ Chroma Mod Sample"; Filename: "{userpf}\Razer\CSDK_ChromaModSample\CSDK_ChromaModSample.exe"; WorkingDir: "{app}";
Name: "{commondesktop}\Chroma Discord App"; Filename: "{userpf}\Razer\CSDK_ChromaModSample\CSDK_ChromaModSample.exe"; WorkingDir: "{app}";
Name: "{group}\Uninstall Chroma Discord App"; Filename: "{uninstallexe}"

[Run]

Filename: "{userpf}\Razer\CSDK_ChromaModSample\CSDK_ChromaModSample.exe"; Description: "Launch C++ Chroma Mod Sample"; Flags: postinstall skipifsilent runascurrentuser nowait; WorkingDir: "{app}"
