# Microsoft Developer Studio Project File - Name="mfcui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=mfcui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mfcui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mfcui.mak" CFG="mfcui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mfcui - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mfcui - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mfcui - Win32 Release Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mfcui - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\lua" /I "..\..\htmlayout" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 lua5.1.lib htmlayout.lib libexctrl.lib /nologo /subsystem:windows /dll /machine:I386 /out:"bin/mfc.dll" /libpath:"..\..\lib" /libpath:"lib"

!ELSEIF  "$(CFG)" == "mfcui - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\\" /I "..\..\lua" /I "..\..\htmlayout" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 lua5.1.lib htmlayout.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"bin/mfc_d.dll" /pdbtype:sept /libpath:"..\..\lib"

!ELSEIF  "$(CFG)" == "mfcui - Win32 Release Static"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mfcui___Win32_Release_Static"
# PROP BASE Intermediate_Dir "mfcui___Win32_Release_Static"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Static"
# PROP Intermediate_Dir "Release_Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\..\\" /I "..\..\lua" /I "..\..\htmlayout" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\\" /I "..\..\lua" /I "..\..\htmlayout" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua5.1.lib htmlayout.lib /nologo /subsystem:windows /dll /machine:I386 /out:"bin/mfc.dll" /libpath:"..\..\lib"
# ADD LINK32 lua5.1.lib htmlayout.lib libexctrl.lib /nologo /subsystem:windows /dll /machine:I386 /out:"bin/mfc_s.dll" /libpath:"..\..\lib" /libpath:"lib"

!ENDIF 

# Begin Target

# Name "mfcui - Win32 Release"
# Name "mfcui - Win32 Debug"
# Name "mfcui - Win32 Release Static"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\HtmlayoutWDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\LuaGraphics.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\LuaHtmlayoutElment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\mfc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\mfc.def
# End Source File
# Begin Source File

SOURCE=..\..\mfc.rc
# End Source File
# Begin Source File

SOURCE=..\..\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\behavior_lua.h
# End Source File
# Begin Source File

SOURCE=..\..\ExtendWidget.h
# End Source File
# Begin Source File

SOURCE=..\..\exwidget.h
# End Source File
# Begin Source File

SOURCE=..\..\HtmlayoutWDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\htmlayoutwrapper.h
# End Source File
# Begin Source File

SOURCE=..\..\LuaGraphics.h
# End Source File
# Begin Source File

SOURCE=..\..\LuaHtmlayoutElment.h
# End Source File
# Begin Source File

SOURCE=..\..\mfc.h
# End Source File
# Begin Source File

SOURCE=..\..\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\..\targetver.h
# End Source File
# Begin Source File

SOURCE=..\..\util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "behavior"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\htmlayout\behaviors\behavior_accesskeys.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\htmlayout\behaviors\behavior_grid.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\htmlayout\behaviors\behavior_shellicon.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\htmlayout\behaviors\behavior_tabs.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
