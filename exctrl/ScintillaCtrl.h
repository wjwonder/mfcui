#ifndef _SCINTILLA_H_
#define _SCINTILLA_H_
#pragma once

#include "ExtendWidget.h"
#include "Scintilla.h"
#include "wrapperwnd.h"

class CScintillaCtrl;
//定义注册类型
struct ScintillaType : public CControlTypeT<ScintillaType, CScintillaCtrl>
{
public:
	ScintillaType()
	{		
	};
	virtual const char* GetTypeName() {
		return "scintilla";
	}
	//生成控件
	virtual HWND Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl);	

	//销毁控件
	virtual bool  Destroy(void* pControl);			

	//以下函数中pContrl是由Create函数生成的控件
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet,  BOOL& bHandled);
};

//////////////////////////////////////////////////////////////////////////
class CScintillaCtrl: public CExctrlWrappedWnd
{
public:
	CScintillaCtrl();
	~CScintillaCtrl(void);

	DECLARE_LUA_SUPPORT_CTRL(CScintillaCtrl);
protected:
	HWND	    m_hScintillaWnd;
	SciFnDirect m_pFnDirect;	//直接调用功能函数
	sptr_t	    m_ptr;			//上下文
	int			m_nCtrlID;
	
	typedef std::map<long, long> CNotifyCodeMap;
	CNotifyCodeMap	m_NotifyCodeMap;
public:

	virtual BOOL	Create(HWND hParent);
	//删除窗口
	virtual void Destroy();

	//向控件发送消息
	sptr_t  SendEditor(unsigned int iMessage, uptr_t wParam = 0, sptr_t lParam = 0)
	{
		if(m_pFnDirect == NULL || m_ptr == NULL)
			return NULL;

		return (*m_pFnDirect)(m_ptr, iMessage, wParam, lParam);
	}

	//注册需要处理的通知代码
	void RegisterNotify(long nNotifyCode);

protected:
	int  SendMsg(lua_State* L);			//发送消息
	int  RegisterNotify(lua_State* L);	//注册notify消息
	int  GetText(lua_State* L);			//获得所有内容
	int  GetTextRange(lua_State* L);	//获得指定位置的字符串
	int  GetLine(lua_State* L);			//获得某行的字符
protected:
	void    Notify(SCNotification* pNotify);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEarseBackground();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
};

#endif //_SCINTILLA_H_