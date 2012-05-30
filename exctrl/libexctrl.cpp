//扩展控件采用静态链接方式的初始化函数
//wj 2012.01.05

#include "stdafx.h"
#include "MyListBox.h"
#include "ScintillaCtrl.h"
#include "webctrl.h"
#include "flashctrl.h"
#include "../exwidget.h"

void InitExCtrl(lua_State* L)
{
	CExctrlWrappedWnd::Register();

	RegisterWidget(new ListBoxType);
	
	//Scintilla控件
	HMODULE hmod = LoadLibrary("SciLexer.dll");
	if(hmod)		
		RegisterWidget(new ScintillaType);

//	RegisterWidget(new IEWebBrowserType);

	RegisterWidget(new FlashCtrlType);

	
	lua_getglobal(L,  "require");
	lua_pushstring(L, "exctrl");
	lua_pcall(L, 1, -1, 0);
}