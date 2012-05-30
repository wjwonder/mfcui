#include "stdafx.h"
#include "webctrl.h"

//IEWebBrowserType
HWND IEWebBrowserType::Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl)
{	
	CWebCtrl* pCtrl = new CWebCtrl();
	if(!pCtrl->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 30, 30), CWnd::FromHandle(hParent), 999))	
	{
		delete pCtrl;
		return NULL;
	}
	pControl = pCtrl;
	return pCtrl->GetSafeHwnd();
}

bool IEWebBrowserType::Destroy(void* pControl)
{
	if(pControl == NULL)
		return false;

	CWebCtrl* pList = (CWebCtrl *)pControl;	
	pList->DestroyWindow();
	return true;
}


int IEWebBrowserType::Attribute(void* pControl, lua_State* L, BOOL bGet, BOOL& bHandled)
{
	CWebCtrl* pCtrl = ChangeControlType(pControl);
	if(pCtrl == NULL)
	{
		bHandled = FALSE;
		return 0;
	}
	bHandled = FALSE;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
CWebCtrl::RegType CWebCtrl::methods[] = 
{
	methodL(CWebCtrl, Navigate),
	methodL(CWebCtrl, Write),
	methodL(CWebCtrl, Clear),
	methodL(CWebCtrl, GoBack),
	methodL(CWebCtrl, GoForward),
	methodL(CWebCtrl, GoHome),
	methodL(CWebCtrl, Stop),
	methodL(CWebCtrl, Refresh),
	{0, 0}
};

CWebCtrl::CWebCtrl()
{
	InitLuaEnv();
}

int CWebCtrl::NavigateL(lua_State* L)
{
	this->Navigate(PAS(L, 1));
	return 0;
}

int CWebCtrl::WriteL(lua_State* L)
{
	this->Write(PAS(L, 1));
	return 0;
}

int CWebCtrl::ClearL(lua_State* L)
{
	this->Clear();
	return 0;
}

void CWebCtrl::OnTitleChange(CString text)
{
	SimpleBrowser::OnTitleChange(text);	
	BEGIN_LUA_CALLBACK
	lua_pushstring(m_L, text);
	CallLuaMsg( "onTitleChange", 1);	
	END_LUA_CALLBACK
	return;
}

void CWebCtrl::OnDocumentComplete(CString	URL)
{
	SimpleBrowser::OnDocumentComplete(URL);
	BEGIN_LUA_CALLBACK

	lua_pushstring(m_L, URL);	
	CallLuaMsg("onDocumentComplete", 1);

	END_LUA_CALLBACK
	return;
}

bool CWebCtrl::OnBeforeNavigate2(CString	URL,
							   CString	frame,
							   void		*post_data,int post_data_size,
							   CString	headers)
{
	SimpleBrowser::OnBeforeNavigate2(URL, frame, post_data, post_data_size, headers);

	BEGIN_LUA_CALLBACK
	lua_pushstring(m_L, URL);
	lua_pushstring(m_L, frame);
	lua_pushstring(m_L, headers);
	if(post_data && post_data_size > 0)
		lua_pushlstring(m_L, (const char*)post_data, post_data_size);
	else
		lua_pushnil(m_L);

	if(CallLuaMsg("onBeforeNavigate", 4))
	{
		return lua_toboolean(m_L, -1);
	}

	END_LUA_CALLBACK

	return false;
}

bool CWebCtrl::OnNewWindow3(CString strURL, CString strURLContext, DWORD dwFlags)
{
	SimpleBrowser::OnNewWindow3(strURL, strURLContext, dwFlags);
	BEGIN_LUA_CALLBACK
	lua_pushstring(m_L, strURL);
	lua_pushstring(m_L, strURLContext);
	lua_pushnumber(m_L, dwFlags);
	if(CallLuaMsg("onNewWindow", 3))
	{
		return lua_toboolean(m_L, -1);
	}
	END_LUA_CALLBACK
	return false;
}


int CWebCtrl::GoBackL(lua_State* L)
{
	GoBack();
	return 0;
}

int CWebCtrl::GoForwardL(lua_State *L)
{
	GoForward();
	return 0;
}

int CWebCtrl::GoHomeL(lua_State *L)
{
	GoHome();
	return 0;
}

int CWebCtrl::RefreshL(lua_State *L)
{
	Refresh();
	return 0;
}

int CWebCtrl::StopL(lua_State *L)
{
	Stop();
	return 0;
}
