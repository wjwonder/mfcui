#include "StdAfx.h"
#include "ScintillaCtrl.h"
#include <htmlayout_dom.hpp>
//

// ScintillaType

HWND ScintillaType::Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl)
{
	CScintillaCtrl* pCtrl = new CScintillaCtrl();
	if(!pCtrl->Create(hParent))
	{
		delete pCtrl;
		return NULL;
	}

	pControl = pCtrl;
	return pCtrl->GetSafeHwnd();
}

bool ScintillaType::Destroy(void* pControl)
{
	if(pControl == NULL)
		return false;

	CScintillaCtrl* pList = (CScintillaCtrl *)pControl;	
	pList->Destroy();
	return true;
}


int ScintillaType::Attribute(void* pControl, lua_State* L, BOOL bGet, BOOL& bHandled)
{
	CScintillaCtrl* pCtrl = ChangeControlType(pControl);
	if(pCtrl == NULL)
	{
		bHandled = FALSE;
		return 0;
	}
	bHandled = FALSE;
	return 0;
}

//////////////////////////////////////////////////////////////////////////

CScintillaCtrl::RegType CScintillaCtrl::methods[] = 
{
	method(CScintillaCtrl, SendMsg),
	method(CScintillaCtrl, RegisterNotify),
	method(CScintillaCtrl, GetText),
	method(CScintillaCtrl, GetTextRange),
	method(CScintillaCtrl, GetLine),
	
	{0, 0}
};

BEGIN_MESSAGE_MAP(CScintillaCtrl, CWnd)	
		ON_WM_SIZE()
		ON_WM_ERASEBKGND()
		ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CScintillaCtrl::CScintillaCtrl()
	:m_hScintillaWnd(NULL),m_pFnDirect(NULL), m_ptr(NULL)
{		
	InitLuaEnv();
}


CScintillaCtrl::~CScintillaCtrl(void)
{	
}

#define DEFAULT_SCINTILLAID  990

BOOL CScintillaCtrl::Create(HWND hParent)
{
	m_nCtrlID = GetCtrlID();
	BOOL bCreated = CExctrlWrappedWnd::Create(hParent);
	if(!bCreated)
		return FALSE;

	HWND hWnd = CreateWindowEx(0,
		"Scintilla","", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
		0,0,0,0, m_hWnd, (HMENU)m_nCtrlID, NULL,NULL);

	if(hWnd == NULL)
	{
		this->DestroyWindow();
		return FALSE;
	}
	m_hScintillaWnd = hWnd;
	m_pFnDirect = (SciFnDirect)::SendMessage(m_hScintillaWnd,SCI_GETDIRECTFUNCTION,0,0);
	m_ptr =       (sptr_t)::SendMessage(m_hScintillaWnd,SCI_GETDIRECTPOINTER,0,0);

	::SetFocus(m_hScintillaWnd);
	return TRUE;
}

void CScintillaCtrl::OnSize(UINT nType, int cx, int cy)
{
	::SetWindowPos(m_hScintillaWnd, NULL, 0, 0, cx, cy, 0);
}

void CScintillaCtrl::OnSetFocus(CWnd* pOldWnd)
{
	::SetFocus(m_hScintillaWnd);
}

BOOL CScintillaCtrl::OnEarseBackground()
{
	return TRUE;
}
void CScintillaCtrl::Destroy()
{
	if(m_hScintillaWnd)
	{
		::DestroyWindow(m_hScintillaWnd);
		m_hScintillaWnd = NULL;
	}
	m_pFnDirect = NULL;
	m_ptr       = NULL;

	//先做自己的清除，再删除父对象
	CExctrlWrappedWnd::Destroy();
}

#define  SET_FIELD_NUMBER(L, n, name) lua_pushnumber(L, (n));lua_setfield(L, -2, (name));
#define  SET_FIELD_STRING(L, n, name) lua_pushstring(L, (n));lua_setfield(L, -2, (name));

void   CScintillaCtrl::Notify(SCNotification* pNotify)
{
	if(m_L == NULL || m_nBindTableRef == LUA_REFNIL)
		return;

	lua_newtable(m_L);
	SET_FIELD_NUMBER(m_L, pNotify->nmhdr.code, "code");

	SET_FIELD_NUMBER(m_L, pNotify->position, "pos");
	SET_FIELD_NUMBER(m_L, pNotify->ch, "ch");
	SET_FIELD_NUMBER(m_L, pNotify->modifiers, "modifiers");
	SET_FIELD_NUMBER(m_L, pNotify->modificationType, "modificationType");

	if(pNotify->text)
	{
		SET_FIELD_STRING(m_L, pNotify->text, "text");
	}
	
	SET_FIELD_NUMBER(m_L, pNotify->length, "length");
	SET_FIELD_NUMBER(m_L, pNotify->linesAdded, "linesAdded");
	SET_FIELD_NUMBER(m_L, pNotify->message, "message");
	SET_FIELD_NUMBER(m_L, pNotify->wParam, "wparam");
	SET_FIELD_NUMBER(m_L, pNotify->lParam, "lparam");
	SET_FIELD_NUMBER(m_L, pNotify->line,    "line");
	SET_FIELD_NUMBER(m_L, pNotify->foldLevelNow,     "foldLevelNow");
	SET_FIELD_NUMBER(m_L, pNotify->foldLevelPrev,    "foldLevelPrev");
	SET_FIELD_NUMBER(m_L, pNotify->margin,    "margin");
	SET_FIELD_NUMBER(m_L, pNotify->listType,    "listType");
	SET_FIELD_NUMBER(m_L, pNotify->x,    "x");
	SET_FIELD_NUMBER(m_L, pNotify->y,    "y");
	SET_FIELD_NUMBER(m_L, pNotify->token,    "token");
	SET_FIELD_NUMBER(m_L, pNotify->annotationLinesAdded,    "annotationLinesAdded");
	SET_FIELD_NUMBER(m_L, pNotify->updated,    "updated");
	SendCallbackMsg(m_L, m_nBindTableRef, "onNotify", 1);	
}

void CScintillaCtrl::RegisterNotify(long nNotifyCode)
{
	m_NotifyCodeMap[nNotifyCode] = 1;
}

LRESULT CScintillaCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_NOTIFY)
	{
		SCNotification* pNotify = (SCNotification *)(lParam);		
		if(pNotify && pNotify->nmhdr.idFrom == m_nCtrlID)
		{
			if(m_NotifyCodeMap.find(pNotify->nmhdr.code) != m_NotifyCodeMap.end())
			{
				this->Notify(pNotify);
				return 0L;
			}
		}
	}	
	return CWnd::WindowProc(message, wParam, lParam);
}


int CScintillaCtrl::SendMsg(lua_State* L)
{
	uptr_t wParam = 0;
	uptr_t lParam = 0;	
	if(lua_type(L, 2) == LUA_TNUMBER)
		wParam = (uptr_t)PAI(L, 2);
	else if(!lua_isnoneornil(L, 2))
		wParam = (uptr_t)PAS(L, 2);

	if(lua_type(L, 3) == LUA_TNUMBER)
		lParam = (uptr_t)PAI(L, 3);
	else if(!lua_isnoneornil(L, 3))
		lParam = (uptr_t)PAS(L, 3);

	lua_pushnumber(L, SendEditor(PAI(L, 1), wParam, lParam));
	return 1;
}


int CScintillaCtrl::RegisterNotify(lua_State* L)
{
	int nTop = lua_gettop(L);
	for(int i = 1; i <= nTop; ++i)
		RegisterNotify(PAI(L, i));

	return 0;
}

int CScintillaCtrl::GetText(lua_State* L)
{
	int nLength = SendEditor(SCI_GETTEXTLENGTH);
	char*  pText = new char[nLength+1];
	if(pText == NULL)
		return 0;

	memset(pText, 0, nLength + 1);	
	SendEditor(SCI_GETTEXT, nLength+1, (sptr_t)pText);
	lua_pushlstring(L, pText, nLength);
	return 1;
}

int CScintillaCtrl::GetTextRange(lua_State* L)
{

	TextRange tr;
	tr.chrg.cpMin	= PAI(L, 1);
	tr.chrg.cpMax   = PAI(L, 2);

	long  nLen = tr.chrg.cpMax - tr.chrg.cpMin;
	if(tr.chrg.cpMax == -1)
	{
		nLen = SendEditor(SCI_GETTEXTLENGTH) - tr.chrg.cpMin;
	}
	if(nLen <= 0)
		return 0;

	char* pText = new char[nLen + 1];
	if(pText == NULL)
		return 0;
	tr.lpstrText = pText;
	memset(pText, 0, nLen + 1);
	nLen = SendEditor(SCI_GETTEXTRANGE, 0, (sptr_t)&tr);
	if(nLen <= 0)
		return 0;
	lua_pushlstring(L, pText, nLen);
	delete[] pText;
	return 1;
}

int CScintillaCtrl::GetLine(lua_State* L)
{
	int nLine = PAI(L, 1);
	int nLineLength = SendEditor(SCI_LINELENGTH, nLine);

	char* pText = new char[nLineLength + 1];
	if(pText == NULL)
		return 0;
	memset(pText, 0, nLineLength + 1);
	SendEditor(SCI_GETLINE, nLine, (sptr_t)pText);
	lua_pushlstring(L, pText, nLineLength);
	delete[] pText;
	return 1;
}