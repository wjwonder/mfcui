#include "Luna.h"
#include "LuaHtmlayoutElment.h"
//窗口关闭的时候处理脚本
inline int  WindowCloseHandle(lua_State* L, HWND hWnd)
{
	Lua_SaveState ss(L);
	int nError = 0;

	do
	{
		lua_getglobal(L, "errormsg");
		if(lua_isnil(L, -1))
		{
			lua_pop(L, 1);
		}
		else
		{
			nError = lua_gettop(L);
		}

		lua_getglobal(L, "onWindowClose");
		if(!lua_isfunction(L, -1))
			break;

		CLuaHtmlayoutElement::PushElement(L, htmlayout::dom::element::root_element(hWnd));
		lua_pcall(L, 1, 0, nError);
	}while(0);
	return 0;
}