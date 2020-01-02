# Microsoft Developer Studio Project File - Name="Bach3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Bach3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Bach3.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Bach3.mak" CFG="Bach3 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Bach3 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Bach3 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Bach3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"Release/Bach4.exe"

!ELSEIF  "$(CFG)" == "Bach3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"Debug/Bach4.exe"

!ENDIF 

# Begin Target

# Name "Bach3 - Win32 Release"
# Name "Bach3 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CApplication.cpp
# End Source File
# Begin Source File

SOURCE=.\CBMStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\CChord.cpp
# End Source File
# Begin Source File

SOURCE=.\CCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogChannelSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogChordProgression.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogChordSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogClickTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogCursorSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogDefineSection.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogDrumPart.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogDumpSong.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogGuitarChord.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogHarmonize.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogKeySettings.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogMacro.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogMelody.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogMidiInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogPatchSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogPercussion.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogSongSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogSongStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogTransposeDiatonic.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogTransposeNotes.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogVoiceSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\CLinkedList.cpp
# End Source File
# Begin Source File

SOURCE=.\CMeasure.cpp
# End Source File
# Begin Source File

SOURCE=.\CMidiEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CMidiFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CMidiNote.cpp
# End Source File
# Begin Source File

SOURCE=.\CMidiTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\CMusicalThing.cpp
# End Source File
# Begin Source File

SOURCE=.\CNote.cpp
# End Source File
# Begin Source File

SOURCE=.\CPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\CSection.cpp
# End Source File
# Begin Source File

SOURCE=.\CSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\CSong.cpp
# End Source File
# Begin Source File

SOURCE=.\CStaff.cpp
# End Source File
# Begin Source File

SOURCE=.\CString.cpp
# End Source File
# Begin Source File

SOURCE=.\CStructuredSong.cpp
# End Source File
# Begin Source File

SOURCE=.\CTick.cpp
# End Source File
# Begin Source File

SOURCE=.\CVoice.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogAboutBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogSongSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\Er_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalDefs.cpp
# End Source File
# Begin Source File

SOURCE=.\Table.cpp
# End Source File
# Begin Source File

SOURCE=.\WinMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\Bach.ico
# End Source File
# Begin Source File

SOURCE=.\Bach3.rc
# End Source File
# Begin Source File

SOURCE=.\Res\Document.ico
# End Source File
# End Group
# End Target
# End Project
