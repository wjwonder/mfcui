# Microsoft Developer Studio Project File - Name="libexctrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libexctrl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libexctrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libexctrl.mak" CFG="libexctrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libexctrl - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libexctrl - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libexctrl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libexctrl___Win32_Release"
# PROP BASE Intermediate_Dir "libexctrl___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libexctrl___Win32_Release"
# PROP Intermediate_Dir "libexctrl___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../" /I "../../../lua" /I "../../../htmlayout" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib\libexctrl.lib"

!ELSEIF  "$(CFG)" == "libexctrl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libexctrl___Win32_Debug"
# PROP BASE Intermediate_Dir "libexctrl___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libexctrl___Win32_Debug"
# PROP Intermediate_Dir "libexctrl___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../" /I "../../../lua" /I "../../../htmlayout" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib\libexctrl_d.lib"

!ENDIF 

# Begin Target

# Name "libexctrl - Win32 Release"
# Name "libexctrl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\exctrl\flashctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\exctrl\libexctrl.cpp
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
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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
# End Target
# End Project
