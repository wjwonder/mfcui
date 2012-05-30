// exctrl.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "exctrl.h"
#include "ExtendWidget.h"
#include <vector>


#include "MyListBox.h"
#include "ScintillaCtrl.h"
#include "webctrl.h"
#include "flashctrl.h"

#include <lua.h>
static std::vector<CControlType*>  s_ControlList;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CExCtrlApp, CWinApp)
END_MESSAGE_MAP()


// CtsApp 构造

CExCtrlApp::CExCtrlApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CtsApp 对象

CExCtrlApp theApp;


// CtsApp 初始化

BOOL CExCtrlApp::InitInstance()
{
	CWinApp::InitInstance();

	//初始化控件
	s_ControlList.push_back( new ListBoxType);

	//Scintilla控件
	HMODULE hmod = LoadLibrary("SciLexer.dll");
	if(hmod)
		s_ControlList.push_back(new ScintillaType);

	//IE控件
//	s_ControlList.push_back(new IEWebBrowserType);

	//Flash控件
	s_ControlList.push_back(new FlashCtrlType);

	//最后一个一定要填入NULL
	s_ControlList.push_back((CControlType*)NULL);

	return TRUE;
}



extern "C"
CControlType** WINAPI open_mfc_exctrl(lua_State* L)
{	
	if(s_ControlList.empty())
		return NULL;

	CExctrlWrappedWnd::Register();
	lua_getglobal(L,  "require");
	lua_pushstring(L, "exctrl");
	lua_pcall(L, 1, -1, 0);

	return &s_ControlList[0];
	
}
