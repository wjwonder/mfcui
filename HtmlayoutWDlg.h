// HtmlayoutWDlg.h : header file
//

#if !defined(AFX_HTMLAYOUTWDLG_H__D1707EB0_2419_4B57_8064_4221320E0AD5__INCLUDED_)
#define AFX_HTMLAYOUTWDLG_H__D1707EB0_2419_4B57_8064_4221320E0AD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHtmlayoutWDlg dialog
#include "luna.h"
#include <htmlayoutwrapper.h>
#include "afxwin.h"
#include "ExtendWidget.h"

//将htmlayout对话框作为扩展控件使用代码
class CHtmlayoutWDlg;
struct HtmalyoutType : public CControlTypeT<HtmalyoutType, CHtmlayoutWDlg>
{ 
public:
	HtmalyoutType()
	{		
	};
	virtual const char* GetTypeName() {
		return "htmlayoutw";
	}
	//生成控件
	virtual HWND Create(lua_State* L, HELEMENT h,  HWND hParent, void*& pControl);	

	//销毁控件
	virtual bool  Destroy(void* pControl);			

	//以下函数中pContrl是由Create函数生成的控件
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet,  BOOL& bHandled){return 0;}

	virtual int BindLuaTable(void* pControl, lua_State* L, int nTableRef){return 0;}	//通知控件绑定lua 对应table
};

//////////////////////////////////////////////////////////////////////////
class CHtmlayoutWDlg : public CDialog, public CHtmalyoutWrapper<CHtmlayoutWDlg>
{
// Construction
public:
	static const char className[];						//类名	
	DECLARE_LUA_SUPPORT_CTRL(CHtmlayoutWDlg);
	CHtmlayoutWDlg(lua_State* L){};
	CHtmlayoutWDlg(lua_State* L,  LPCTSTR lpszUrl, CWnd* pParent = NULL, BOOL bCreate= FALSE, UINT uDLGID = 0, bool bSkinWin = false, int nParams = 0);	// standard constructor
	~CHtmlayoutWDlg();
	typedef CHtmalyoutWrapper<CHtmlayoutWDlg>  basehtmalyout;
// Dialog Data
	//{{AFX_DATA(CHtmlayoutWDlg)
	enum { IDD = 102 };	
	//}}AFX_DATA

	CString m_strURL;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlayoutWDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	

public:
	virtual BOOL on_event (HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason );
	virtual LRESULT OnCreateControl( LPNMHL_CREATE_CONTROL nm);
	virtual LRESULT OnDestroyControl( LPNMHL_DESTROY_CONTROL nm);
	virtual LRESULT OnAttachBehavior( LPNMHL_ATTACH_BEHAVIOR lpab );
	virtual LRESULT OnLoadData( LPNMHL_LOAD_DATA pnmld);
	virtual LRESULT OnDataLoaded( LPNMHL_DATA_LOADED nm);
	BOOL Load(LPCTSTR lpszURL);

protected:
	//查找函数索引
	int    FindFunction(const char* pszFuncName);

	//调用函数前准备，返回Error函数的索引，小于0,表示函数不能调用
	int	   BeginCallFunc(int nFunRef);

	//获得调用绑定函数
	BOOL GetTableFunction(LPCTSTR lpszFuncname);
	//调用绑定函数
	void CallFunction(int narg, int nError = 0);


// Implementation
protected:	
	int		m_nEventRef;		//Event handle function ref
	int     m_nErrorRef;		//错误信息函数ref
	int		m_nHandleLoadRef;	//处理数据加载
	int		m_nHandleNcHitRef;	//处理NcHitTest函数
	int     m_nDestroyCtrlRef;	//删除控件函数

	BOOL	m_bModal;			//是否是模态对话框

	int		m_nParams;			//启动参数个数

	int		m_nBindViewRef;		//对于的View Table

	int		m_bSkinWindow;		//是否是自定义窗口，是则不显示边框

	// Generated message map functions
	//{{AFX_MSG(CHtmlayoutWDlg)
	virtual BOOL OnInitDialog();		
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnClose();
	afx_msg void OnDestroy();


	//导出到Lua中的函数
public:	
	int load_url(lua_State* L);
	int set_title(lua_State* L);
	int get_title(lua_State* L);
	int close(lua_State* L);	
	int resize(lua_State* L);	
	int show_window(lua_State* L);
	int modify_style(lua_State* L);
	int set_icon(lua_State* L);
	int get_size(lua_State* L);
	int get_pos(lua_State* L);
	int find_element(lua_State* L);
	int get_cursor_pos(lua_State* L);
	int move(lua_State* L);
	int center(lua_State* L);
	int transparent(lua_State* L);
	int transparent_color(lua_State* L);
	int set_size_limit(lua_State* L);
	int drag_anywhere(lua_State* L);
	int drag_rect(lua_State* L);
	int animate_window(lua_State* L);	
	int can_drag_file(lua_State* L);
	int message_box(lua_State* L);
	int get_handle(lua_State* L);
	int register_bind(lua_State* L);
	int update(lua_State* L);
	int set_glass(lua_State* L);

protected:
	CSize  m_MinSize;
	CSize  m_MaxSize;
	BOOL   m_bDragAnywhere;	//点击任何位置拖动窗口
	CRect  m_DragRect;		//此范围点击拖动窗口
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLAYOUTWDLG_H__D1707EB0_2419_4B57_8064_4221320E0AD5__INCLUDED_)
