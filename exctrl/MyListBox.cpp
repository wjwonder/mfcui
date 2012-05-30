// MyListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "MyListBox.h"


// CMyListBox


HWND ListBoxType::Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl)
{
		CMyListBox* pList = new CMyListBox;
		if(pList == NULL)
			return NULL;

		pList->Create(WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP, CRect(0, 0, 0, 0), CWnd::FromHandle(hParent), 1000);		
		pControl = pList;
		return pList->GetSafeHwnd();
}

bool ListBoxType::Destroy(void* pControl)
{
	if(pControl == NULL)
		return false;
	CMyListBox* pList = (CMyListBox *)pControl;
	pList->DestroyWindow();
	delete pList;
	return true;
}

int ListBoxType::Attribute(void* pControl, lua_State* L, BOOL bGet, BOOL& bHandled)
{
	CMyListBox* pList = ChangeControlType(pControl);
	if(pList == NULL)
	{
		bHandled = FALSE;
		return 0;
	}

	bHandled = TRUE;
	CString strName = PAS(L, 1);	
	const char* pszName = PAS(L, 1);\
	//以下是处理控件属性，如果处理了，直接返回，否则不做操作
	if(strName == "count")
	{
		if(bGet)
		{
			lua_pushnumber(L, pList->GetCount());
			return 1;
		}
	}
	else if(strName == "cursel")
	{
		if(bGet)
		{
			lua_pushnumber(L, pList->GetCurSel());
			return 1;
		}
		else
		{
			lua_pushnumber(L, pList->SetCurSel(PAI(L, 2)));
			return 1;
		}
	}
	else if(strName == "topindex")
	{
		if(bGet)
		{
			lua_pushnumber(L, pList->GetTopIndex());
			return 1;
		}
		else
		{
			lua_pushnumber(L, pList->SetTopIndex(PAI(L, 2)));
			return 1;
		}
	}
	else if(strName == "item-height")
	{
		if(bGet)
		{
			lua_pushnumber(L, pList->GetItemHeight(0));
			return 1;
		}
	}
	else if(strName == "dir")
	{
		if(bGet)
		{

		}
		else
		{
			pList->Dir(DDL_HIDDEN | DDL_READONLY | DDL_DIRECTORY, PAS(L, 2));
			return 0;
		}
	}

	bHandled = FALSE;
	return 0;

}
//
CMyListBox::RegType CMyListBox::methods[] = 
{
	methodL(CMyListBox, AddString),
	methodL(CMyListBox, SetCurSel),
	methodL(CMyListBox, DeleteItem),
	methodL(CMyListBox, Empty),

	{0, 0}
};
IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{
	InitLuaEnv();
}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
//	ON_CONTROL_REFLECT(LBN_SELCHANGE, &CMyListBox::OnLbnSelchange)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnLbnSelchange)
//	ON_CONTROL_REFLECT(LBN_DBLCLK, &CMyListBox::OnLbnDblclk)
END_MESSAGE_MAP()

// CMyListBox 消息处理程序
void CMyListBox::OnLbnSelchange()
{
	lua_pushnumber(m_L, GetCurSel());
	BEGIN_LUA_CALLBACK
	CallLuaMsg( "onSelChange", 1);	
	END_LUA_CALLBACK

}

void CMyListBox::OnLbnDblclk()
{
	BEGIN_LUA_CALLBACK
	lua_pushnumber(m_L, GetCurSel());
	CallLuaMsg("onDblclk", 1);	
	END_LUA_CALLBACK
}


int CMyListBox::AddStringL(lua_State* L)
{
	if(lua_istable(L, 1))
	{
		int nTop = lua_gettop(L);
		lua_pushnil(L);
		while(lua_next(L, -2) != 0)
		{
			AddString(PAS(L, -1));
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}
	else if(lua_isnumber(L, 1) && !lua_isnoneornil(L, 2))
	{
		InsertString(PAI(L, 1), PAS(L, 2));
	}
	else
	{
		AddString(PAS(L, 1));
	}
	return 0;
}

int CMyListBox::DeleteItemL(lua_State* L)
{
	int nParams = lua_gettop(L);
	if(lua_istable(L, 1))
	{		
		lua_pushnil(L);
		while(lua_next(L, -2) != 0)
		{
			DeleteString(PAI(L, -1));
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}
	else
	{
		DeleteString(PAI(L, -1));	
	}
	return 0;
}

int CMyListBox::SetCurSelL(lua_State* L)
{
	lua_pushnumber(L, SetCurSel(PAI(L, 1)));
	return 1;
}

int CMyListBox::EmptyL(lua_State* L)
{
	ResetContent();
	return 0;
}