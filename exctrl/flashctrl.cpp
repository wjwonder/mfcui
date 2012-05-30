#include "stdafx.h"
#include "flashctrl.h"

//FlashCtrlType
HWND FlashCtrlType::Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl)
{	
	CFlashCtrl* pCtrl = new CFlashCtrl();
	if(!pCtrl->Create(hParent, h))
	{
		delete pCtrl;
		return NULL;
	}
	pControl = pCtrl;	
	return pCtrl->GetSafeHwnd();
}

bool FlashCtrlType::Destroy(void* pControl)
{
	CFlashCtrl* pCtrl = ChangeControlType(pControl);
	if(pCtrl == NULL)
		return false;
	
	pCtrl->DestroyWindow();
	return true;
}


int FlashCtrlType::Attribute(void* pControl, lua_State* L, BOOL bGet, BOOL& bHandled)
{
	CFlashCtrl* pCtrl = ChangeControlType(pControl);
	if(pCtrl == NULL)
	{
		bHandled = FALSE;
		return 0;
	}
	bHandled = FALSE;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
CFlashCtrl::RegType CFlashCtrl::methods[] = 
{
	methodL(CFlashCtrl, LoadMovie),
	methodL(CFlashCtrl, Play),
	method(CFlashCtrl,  CallFunction),
	{0, 0}
};

BEGIN_MESSAGE_MAP(CFlashCtrl, CWnd)	
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFlashCtrl, CWnd)
	ON_EVENT(CFlashCtrl, 1001, 150/*FSCommand*/,  OnFSCommand, VTS_BSTR VTS_BSTR)
	ON_EVENT(CFlashCtrl, 1001, 197/*FSCommand*/,  OnFlashCall, VTS_BSTR)
END_EVENTSINK_MAP()


CFlashCtrl::CFlashCtrl()
{
	InitLuaEnv();
}

BOOL CFlashCtrl::Create(HWND hParent, HELEMENT h)
{
	if(!CExctrlWrappedWnd::Create(hParent))
	{
		return FALSE;
	}

	if(!m_flashCtrl.Create("FlashCtrl", "FlashCtrl", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), CWnd::FromHandle(m_hWnd), 1001))	
	{		
		DestroyWindow();
		return FALSE;
	}			
	m_flashCtrl.SetScale("ShowAll");
	m_flashCtrl.SetScaleMode(0);

	htmlayout::dom::element e = h;
	wchar_t uri[1048] = {0};
	const wchar_t* szSrc = e.get_attribute("src");
	if(szSrc)
	{
		wcsncpy(uri, szSrc, 1048);
		e.combine_url(uri,1047);
		m_flashCtrl.LoadMovie(0, aux::w2a(uri));
		m_flashCtrl.Play();
	}
	return TRUE;
}

void CFlashCtrl::OnSize(UINT nType, int cx, int cy)
{
	CExctrlWrappedWnd::OnSize(nType, cx, cy);
	if(m_flashCtrl.GetSafeHwnd())
	{
		m_flashCtrl.MoveWindow(0, 0, cx, cy, TRUE);
	}
}

void CFlashCtrl::Destroy()
{
	m_flashCtrl.DestroyWindow();
	CExctrlWrappedWnd::Destroy();
}
BOOL CFlashCtrl::OnEarseBackground(CDC* pDC)
{
	return TRUE;
}
int CFlashCtrl::LoadMovieL(lua_State* L)
{
	m_flashCtrl.LoadMovie(PAI(L, 2), PAS(L, 1));
	m_flashCtrl.Play();
	return 0;
}

int CFlashCtrl::PlayL(lua_State *L)
{
	m_flashCtrl.Play();
	m_flashCtrl.PostMessage(WM_PAINT);
	return 0;
}

int CFlashCtrl::CallFunction(lua_State* L)
{
	lua_pushstring(L, m_flashCtrl.CallFunction(PAS(L, 1)));
	return 1;
}
void CFlashCtrl::OnFSCommand(LPCTSTR command, LPCTSTR args)
{
	BEGIN_LUA_CALLBACK
	lua_pushstring(m_L, args);
	CallLuaMsg(command, 1);	
	END_LUA_CALLBACK
	return;
}

void CFlashCtrl::OnFlashCall(LPCTSTR request)
{
	BEGIN_LUA_CALLBACK
	lua_pushstring(m_L, request);
	CallLuaMsg("OnFlashCall", 1);	
	END_LUA_CALLBACK
	return;
}

