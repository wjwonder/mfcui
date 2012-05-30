# Microsoft Developer Studio Project File - Name="exctrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=exctrl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "exctrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "exctrl.mak" CFG="exctrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "exctrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "exctrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "exctrl - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../" /I "../../../lua" /I "../../../htmlayout" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 lua5.1.lib htmlayout.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../bin/exctrl.dll" /libpath:"../../../lib"

!ELSEIF  "$(CFG)" == "exctrl - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../" /I "../../../lua" /I "../../../htmlayout" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ    /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 lua5.1.lib htmlayout.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../bin/exctrl_d.dll" /pdbtype:sept /libpath:"../../../lib"

!ENDIF 

# Begin Target

# Name "exctrl - Win32 Release"
# Name "exctrl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\exctrl\exctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\exctrl.def
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\exctrl.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\flashctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\MyListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\ScintillaCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\shockwaveflash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\exctrl\exctrl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\flashctrl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\MyListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\SciLexer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\Scintilla.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\ScintillaCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\shockwaveflash.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\targetver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\wrapperwnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
