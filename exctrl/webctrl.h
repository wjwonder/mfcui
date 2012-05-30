#ifndef _WEBCTRL_H_
#define _WEBCTRL_H_
#pragma once
//IE 控件封装

#include "ExtendWidget.h"
#include "SimpleBrowser.h"

class CWebCtrl;
//定义注册类型
struct IEWebBrowserType : public CControlTypeT<IEWebBrowserType, CWebCtrl>
{
public:
	IEWebBrowserType()
	{		
	};
	virtual const char* GetTypeName() {
		return "iebrowser";
	}
	//生成控件
	virtual HWND Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl);	

	//销毁控件
	virtual bool  Destroy(void* pControl);			

	//以下函数中pContrl是由Create函数生成的控件
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet,  BOOL& bHandled);
};

class CWebCtrl : public SimpleBrowser {
public:
	CWebCtrl();
	DECLARE_LUA_SUPPORT_CTRL(CWebCtrl);

	virtual void OnTitleChange(CString text);

	virtual void OnDocumentComplete(CString	URL);

	virtual bool OnBeforeNavigate2(CString	URL,
		CString	frame,
		void		*post_data,int post_data_size,
		CString	headers);

	virtual bool OnNewWindow3(CString strURL, CString strURLContext, DWORD dwFlags);

protected:
	//转到指定的url
	int NavigateL(lua_State* L);

	//当前页面加载内存页面
	int WriteL(lua_State* L);

	//清空当前页面
	int ClearL(lua_State* L);

	int GoBackL(lua_State* L);

	int GoForwardL(lua_State *L);

	int GoHomeL(lua_State *L);

	int RefreshL(lua_State *L);

	int StopL(lua_State *L);
};

#endif //_WEBCTRL_H_