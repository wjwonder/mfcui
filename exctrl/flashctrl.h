#ifndef _FLASHCTRL_H_
#define _FLASHCTRL_H_
#pragma once
//IE 控件封装

#include "ExtendWidget.h"
#include "shockwaveflash.h"
#include "wrapperwnd.h"

class CFlashCtrl;
//定义注册类型
struct FlashCtrlType : public CControlTypeT<FlashCtrlType, CFlashCtrl>
{
public:
	FlashCtrlType()
	{		
	};
	virtual const char* GetTypeName() {
		return "flashctrl";
	}
	//生成控件
	virtual HWND Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl);	

	//销毁控件
	virtual bool  Destroy(void* pControl);			

	//以下函数中pContrl是由Create函数生成的控件
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet,  BOOL& bHandled);
};

class CFlashCtrl : public CExctrlWrappedWnd {
public:
	CFlashCtrl();
	DECLARE_LUA_SUPPORT_CTRL(CFlashCtrl);

public:
	virtual BOOL Create(HWND hParent, HELEMENT h);
	virtual void Destroy();
protected:
	CShockwaveFlash		m_flashCtrl;	//内嵌的flash控件
protected:
	int LoadMovieL(lua_State* L);
	int PlayL(lua_State* L);
	int CallFunction(lua_State* L);
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEarseBackground(CDC* pDC);
	afx_msg void OnFSCommand(LPCTSTR command, LPCTSTR args);
	afx_msg void OnFlashCall(LPCTSTR request);
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

#endif //_WEBCTRL_H_