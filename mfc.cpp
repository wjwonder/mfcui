// mfc.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdisp.h>
#include "mfc.h"
#include "behavior_lua.h"
#include "LuaGraphics.h"
#include "Luna.h"

#include "ExtendWidget.h"
#include "HtmlayoutWDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************************************************************************/
static int get_html_element(lua_State* L)
{
	htmlayout::dom::element e = (HELEMENT)PAI(L, 1);
	if(!e.is_valid())
		return 0;

	lua_pushnumber(L, (long)e.find_nearest_parent("html"));
	return 1;
}

static int get_element_style(lua_State* L)
{
	htmlayout::dom::element e = (HELEMENT)PAI(L, 1);
	const wchar_t* pAttr = e.get_style_attribute(PAS(L, 2));
	if(pAttr == NULL)
		return 0;

	lua_pushstring(L, aux::w2a(pAttr));
	return 1;
}

static int get_element_attribute(lua_State* L)
{
	htmlayout::dom::element e = (HELEMENT)PAI(L, 1);
	const wchar_t* pAttr = e.get_attribute(PAS(L, 2));
	if(pAttr == NULL)
		return 0;

	lua_pushstring(L, aux::w2a(pAttr));
	return 1;
}

static int get_hwnd_root_element(lua_State* L)
{
	lua_pushnumber(L, (long)htmlayout::dom::element::root_element((HWND)PAI(L, 1)));
	return 1;
}

static int get_view(lua_State* L)
{
	htmlayout::dom::element e = (HELEMENT)PAI(L, 1);
	if(!e.is_valid())
		return 0;
	CHtmlayoutWDlg* pView = (CHtmlayoutWDlg *)CWnd::FromHandle(e.get_element_hwnd(true));
	if(pView == NULL)
		return 0;

	Lunar<CHtmlayoutWDlg>::push(L, pView, false);
	return 1;
}

static int url_unescape(lua_State* L)
{
	wchar_t uri[2048] = {0};	
	HTMLayoutUrlUnescape(PAS(L, 1), uri, 2048);
	lua_pushstring(L, aux::w2a(uri));
	return 1;
}


static int url_escape(lua_State* L)
{
	wchar_t uri[2048];
	wcsncpy(uri,aux::a2w(PAS(L, 1)),2048);
	uri[2047] = 0;

	char url[4096] = {0};

	HTMLayoutUrlEscape(uri, TRUE, url, 4096);
	lua_pushstring(L, url);
	return 1;
}

const luaL_Reg htmlayout_lib[] = 
{
	{"get_html_element", get_html_element},
	{"get_element_style", get_element_style},
	{"get_element_attribute", get_element_attribute},
	{"get_hwnd_root_element", get_hwnd_root_element},
	{"get_view",			  get_view},
	{"url_unescape",			  url_unescape},
	{"url_escape",			  url_escape},
	
	{0, 0}
};
/************************************************************************/
//线程消息
#define   WM_USERMSG  (WM_USER + 0xF000)

struct ThreadInfo
{
	lua_State* m_ThreadLuaState;
	DWORD      m_dwThreadID;
};

struct PostData
{
	char* m_pData;	//数据
	long  m_lLen;	//数据长度
	PostData(const char* pszData, long lLen)
	{
		m_pData = NULL;
		m_lLen = lLen;
		if(m_lLen > 0)
		{
			m_pData = new char[lLen];
			memcpy(m_pData, pszData, lLen);
		}
	}
	~PostData()
	{
		if(m_pData)
		{
			delete[] m_pData;
		}
	}
};
class CLuaWinThread : public CWinApp
{
public:
	CLuaWinThread(): CWinApp()
	{
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
			return;		

		HINSTANCE hHandle = GetModuleHandle("mfc.dll");
		AfxSetResourceHandle(hHandle);
	};	

	int RunMessageLoop()
	{
		return CWinThread::Run();
	}
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(pMsg->message == WM_USERMSG)
		{
			lua_State* L = (lua_State* )pMsg->wParam;
			lua_getglobal(L, "onLoop");
			if(lua_isfunction(L, -1))
			{
				PostData* pData = (PostData *)pMsg->lParam;
				if(pData)
				{
					lua_pushlstring(L, pData->m_pData, pData->m_lLen);
					delete pData;
				}
				else
				{
					lua_pushnil(L);
				}
				lua_pcall(L, 1, 0, 0);			
				return TRUE;
			}
		}

		return CWinApp::PreTranslateMessage(pMsg);
	}
};



//
static CLuaWinThread threadapp;
static lua_State* m_TimerL = NULL;
DWORD s_dwThreadID = 0;

typedef std::map<std::string, ThreadInfo*>  CMFCThreadMap;
static CMFCThreadMap s_ThreadMap;

static int loop(lua_State* L)
{	
	threadapp.RunMessageLoop();	
	return 0;
}

//初始化mfc环境
//mfc.init([线程名称]);
static int mfc_init(lua_State* L)
{
	::OleInitialize(NULL);
	AfxEnableControlContainer();

	lua_register(L, "bind_behavior", htmlayout::lua_behavior::ChangeBehavior);		

	Lunar<CLuaHtmlayoutElement>::Register(L);
	Lunar<CLuaGraphics>::Register(L);	
	Lunar<CHtmlayoutWDlg>::Register(L);
	
	lua_getglobal(L, "require");
	lua_pushstring(L, "ui");
	lua_pcall(L, 1, -1, 0);
#ifndef _AFXDLL
	lua_getglobal(L,  "require");
	lua_pushstring(L, "exctrl");
	lua_pcall(L, 1, -1, 0);
#endif
	if(lua_isstring(L, 1))
	{
		std::string strName = PAS(L, 1);
		ThreadInfo* pInfo = new ThreadInfo;
		pInfo->m_ThreadLuaState = L;
		pInfo->m_dwThreadID = GetCurrentThreadId();
		s_ThreadMap[strName] = pInfo;		
	}
	return 0;
}

static int mfc_exit(lua_State* L)
{
	AfxPostQuitMessage(0);
	return 0;
}

static int DoModalDlg(lua_State *L)
{
	CString strUrl = PAS(L, 1);
	CWnd*   pParent = CWnd::FromHandle((HWND)PAI(L, 2));
	bool	bSkinWindow = lua_toboolean(L, 3);
	//删除之前的三个参数
	lua_remove(L, 1);
	lua_remove(L, 1);
	lua_remove(L, 1);

	CHtmlayoutWDlg dlg(L, strUrl, pParent, 0, 0, bSkinWindow,  lua_gettop(L));	
	lua_pushboolean(L, IDOK == dlg.DoModal());
	return 1;
}

static int showdialog(lua_State *L)
{
	CString strUrl = PAS(L, 1);
	CWnd*   pParent = CWnd::FromHandle((HWND)PAI(L, 2));
	bool	bSkinWindow = lua_toboolean(L, 3);

	//删除之前的三个参数
	lua_remove(L, 1);
	lua_remove(L, 1);
	lua_remove(L, 1);

	CHtmlayoutWDlg* pPage = new CHtmlayoutWDlg(L, strUrl, pParent, TRUE, 0, bSkinWindow, lua_gettop(L));
	if(pPage == NULL)
		return 0;

	pPage->CenterWindow();
	lua_pushnumber(L, (long)pPage->GetSafeHwnd());
	return 1;
}

static int post_message(lua_State* L)
{
	if(lua_isnoneornil(L, 1))
		return 0;

	std::string strName = PAS(L, 1);
	CMFCThreadMap::iterator iter = s_ThreadMap.find(PAS(L, 1));
	if(iter == s_ThreadMap.end())
		return 0;
	
	void* pData = NULL;
	if(!lua_isnoneornil(L, 2))
	{
		pData = new PostData(PAS(L, 2), PASL(L, 2));
	}
	
	lua_pushboolean(L, PostThreadMessage(iter->second->m_dwThreadID, WM_USERMSG, (WPARAM)iter->second->m_ThreadLuaState, (LPARAM)pData));	
	return 1;
}


typedef CControlType** (WINAPI *pfnOpenExCtrl)(lua_State* L);

//注册扩展控件
static int register_exctrl(lua_State* L)
{
#ifndef _AFXDLL	
		return 0;
#endif

	HINSTANCE hInstance = LoadLibrary(PAS(L, 1));
	if(hInstance == NULL)
		return 0;

	do
	{
		pfnOpenExCtrl pfnFunc = (pfnOpenExCtrl)GetProcAddress(hInstance, "open_mfc_exctrl");		
		if(pfnFunc == NULL)
			break;

		CControlType** pList = pfnFunc(L);
		if(pList == NULL)
			break;

		lua_newtable(L);
		int nIndex = 0;		
		for(CControlType* pCtrl = pList[nIndex]; pCtrl != NULL; pCtrl = pList[++nIndex])
		{
			RegisterWidget(pCtrl);
			lua_pushstring(L, pCtrl->GetTypeName());
			lua_rawseti(L, -2, nIndex + 1);
		}
		return 1;

	}while(0);
	FreeLibrary(hInstance);
	return 0;
}

static int lua_filedialog(lua_State* L)
{
	BOOL bOpen = lua_isnoneornil(L, 1) ? TRUE : lua_toboolean(L, 1);
	LPCTSTR lpszFilter  = lua_isnoneornil(L, 2) ? NULL: PAS(L, 2);
	LPCTSTR lpszFileName = lua_isnoneornil(L, 3) ? NULL : PAS(L, 3);

	CFileDialog dlg(bOpen, NULL, lpszFileName, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		lpszFilter, NULL);	

	static char szBuf[1024 * 10] = {0};
	if(bOpen)
	{
		dlg.m_ofn.lpstrFile = szBuf;
		dlg.m_ofn.nMaxFile  = sizeof(szBuf);
		dlg.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	}
	dlg.m_ofn.Flags |= OFN_ENABLEINCLUDENOTIFY;	
	
	if(dlg.DoModal() != IDOK)
		return 0;

	lua_newtable(L);	
	int count = 0;
	CString strPath;
	POSITION pos = dlg.GetStartPosition();
	while(pos != NULL)
	{
		strPath = dlg.GetNextPathName(pos);

		lua_pushnumber(L, count + 1);
		lua_pushstring(L, strPath);

		lua_settable(L, -3);
		count ++;
	}
	return 1;
}

//获得当前路径
static int lua_get_currentdir(lua_State *L)
{
	char szPath[MAX_PATH] = {0};
	if(::GetCurrentDirectory(MAX_PATH, szPath) > 0)
	{
		lua_pushstring(L, szPath);
		return 1;
	}
	return 0;
}

//获得主程序所在路径
static char szWorkDir[MAX_PATH] = {0};
static int lua_get_workdir(lua_State* L)
{
	if(szWorkDir[0] == '\0')
	{
		DWORD dwSize = GetModuleFileName(NULL, szWorkDir, MAX_PATH);
		for(int i = dwSize - 1; i >= 0; --i)
		{
			if(szWorkDir[i] == '\\')
			{
				szWorkDir[i] = '\0';
				break;
			}
		}

	}
	lua_pushstring(L, szWorkDir);
	return 1;
}

//执行外部程序
static int lua_shellexecute(lua_State* L)
{
	ShellExecute((HWND)PAI(L, 5), luaL_optstring(L, 4, "open"),  PAS(L, 1), lua_isnoneornil(L, 2) ? NULL : PAS(L, 2), lua_isnoneornil(L, 3) ? NULL : PAS(L, 3), SW_SHOWNORMAL);
	return 0;
}

const luaL_Reg l_lib[] = 
{
	{"loop",	   loop},
	{"init",	   mfc_init},
	{"exit",	   mfc_exit},
	{"showdialog", showdialog},	
	{"domodaldlg", DoModalDlg},
	{"register", register_exctrl},
	{"post_message", post_message},
	{"filedialog", lua_filedialog},
	{"currentdir", lua_get_currentdir},
	{"workdir",    lua_get_workdir},
	{"shell_execute", lua_shellexecute},
	{0, 0}
};

#ifndef _AFXDLL	
extern void InitExCtrl(lua_State* L);
#endif

static BOOL  s_bInited = FALSE;
extern "C"
int luaopen_mfc(lua_State* L)
{	
	if(!s_bInited)
	{
		RegisterWidget(new HtmalyoutType);
		//静态链接的话，直接调用exctrl相关初始化函数
#ifndef _AFXDLL	
		InitExCtrl(L);
#endif
		s_bInited = TRUE;
	}
	luaL_register(L, "mfc", l_lib);
	luaL_register(L, "htmlayout", htmlayout_lib);
	return 0;
}

 