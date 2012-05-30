#ifndef _EXCTRL_WRAPPERWND_H
#define _EXCTRL_WRAPPERWND_H
/************************************************************************/
/* 扩展控件包装窗口                                                      */
/* 提供给其他需要接受消息的窗口做父窗口使用								*/
/* Date:    2012-03-05													*/
/************************************************************************/

#define EXCTRL_WRAPPED_HWND  "ExCtrlWrappedWindow"
class CExctrlWrappedWnd: public CWnd
{
public:
	CExctrlWrappedWnd(){};
	~CExctrlWrappedWnd(void){};
	
public:
	static BOOL		Register()
	{
		HINSTANCE hInstance;
		hInstance = AfxGetInstanceHandle();
		WNDCLASS wc;	

		if (!(::GetClassInfo(hInstance, EXCTRL_WRAPPED_HWND, &wc)))  {
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
			wc.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			wc.hIcon   = NULL;
			wc.hInstance = hInstance;
			wc.lpfnWndProc = ::DefWindowProc;
			wc.lpszClassName = EXCTRL_WRAPPED_HWND;
			wc.lpszMenuName = NULL;
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

			if (!AfxRegisterClass(&wc))  {			
				return FALSE;
			}
		}
		return TRUE;
	}

	//生成控件ID
	static long    GetCtrlID()
	{
		static long nCtrlID = 1999;
		InterlockedIncrement(&nCtrlID);
		return nCtrlID;
	}

	//生成包装窗口
	virtual BOOL	Create(HWND hParent)
	{
		BOOL bCreated = CWnd::Create(
			EXCTRL_WRAPPED_HWND,
			_T(""),
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			CRect(0, 0, 0, 0),
			CWnd::FromHandle(hParent),
			GetCtrlID()
			);
		return bCreated;
	}

	//删除窗口
	virtual void Destroy()
	{
		this->DestroyWindow();
		delete this;
	}
};
#endif //_EXCTRL_WRAPPERWND_H