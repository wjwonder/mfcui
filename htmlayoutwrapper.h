#ifndef HTMLAYOUT_WRAPPER_H
#define HTMLAYOUT_WRAPPER_H
#pragma once

#include <htmlayout.h>
#include <htmlayout_dom.hpp>


template<typename T>
class CHtmalyoutWrapper : public htmlayout::event_handler
{
public:
	CHtmalyoutWrapper():event_handler(HANDLE_ALL)
	{	
	}

	virtual ~CHtmalyoutWrapper()
	{

	}

public:
	///从内存中装载页面
	BOOL LoadHtml(LPCBYTE pb, DWORD nBytes)
	{
		ASSERT(::IsWindow(GetThis()->m_hWnd));
		return ::HTMLayoutLoadHtml(GetThis()->m_hWnd, pb, nBytes);
	}

	///加载url
	BOOL LoadUrl(LPCWSTR url)
	{
		ASSERT(::IsWindow(GetThis()->m_hWnd));
		return ::HTMLayoutLoadFile(GetThis()->m_hWnd, url);
	}

	BOOL LoadUrl(const char* url)
	{
		return LoadUrl(t2w(url));
	}

	//窗口加载资源html文件
	BOOL LoadHtmlResource(LPCTSTR name)
	{	
		PBYTE pb = NULL;
		DWORD cb = 0;
		HMODULE hModule = AfxGetResourceHandle();
		HRSRC hrsrc = ::FindResource( hModule, name, MAKEINTRESOURCE(RT_HTML));
		
		if(!hrsrc) 
			return FALSE;
		
		HGLOBAL hgres = ::LoadResource(hModule, hrsrc);
		if(!hgres) return FALSE;
		
		pb = (PBYTE)::LockResource(hgres); if (!pb) return FALSE;
		cb = ::SizeofResource(hModule, hrsrc); if (!cb) return FALSE;
		
		return LoadHtml(pb,cb);
	}
	
	//页面中加载资源文件
	inline bool load_resource_data(LPCWSTR uri, PBYTE& pb, DWORD& cb )
	{
		
		if (!uri || !uri[0]) return LOAD_DISCARD;
		// Retrieve url specification into a local storage since FindResource() expects 
		// to find its parameters on stack rather then on the heap under Win9x/Me
		WCHAR achURL[MAX_PATH]; wcsncpy(achURL, uri, MAX_PATH);
		
		LPWSTR pszName = achURL;
		
		// Separate extension if any
		LPWSTR pszExt = wcsrchr(pszName, '.'); if (pszExt) *pszExt++ = '\0';
		
		// Find specified resource and leave if failed. Note that we use extension
		// as the custom resource type specification or assume standard HTML resource 
		// if no extension is specified
		
		HRSRC hrsrc = 0;
		bool  isHtml = false;
		if( pszExt == 0 || wcsicmp(pszExt,L"HTML") == 0)
		{
			hrsrc = ::FindResourceW(0, pszName, MAKEINTRESOURCEW(23));
			isHtml = true;
		}
		else
			hrsrc = ::FindResourceW(0, pszName, pszExt);
		
		if (!hrsrc) return false; // resource not found here - proceed with default loader
		
		// Load specified resource and check if ok
		
		HGLOBAL hgres = ::LoadResource(0, hrsrc);
		if (!hgres) return false;
		
		// Retrieve resource data and check if ok
		
		pb = (PBYTE)::LockResource(hgres); if (!pb) return FALSE;
		cb = ::SizeofResource(0, hrsrc); if (!cb) return FALSE;
		
		// Report data ready
		
		return true;
	}

	// implement what is needed in your application

	virtual LRESULT OnCreateControl( LPNMHL_CREATE_CONTROL nm) { return 0; }
	virtual LRESULT OnControlCreated( LPNMHL_CREATE_CONTROL nm) { 
		return 0; 
	}
	virtual LRESULT OnDestroyControl( LPNMHL_DESTROY_CONTROL nm) { return 0; }
	virtual LRESULT OnLoadData( LPNMHL_LOAD_DATA pnmld)
	{
		wprintf(L"onLoadData %s\n", pnmld->uri);
		PBYTE pb; DWORD cb;
        if(load_resource_data(pnmld->uri, pb, cb))
          ::HTMLayoutDataReady(pnmld->hdr.hwndFrom, pnmld->uri, pb,  cb);
        return LOAD_OK;
	}
	virtual LRESULT OnDataLoaded( LPNMHL_DATA_LOADED nm) { 		
		return 0; 
	}
	virtual LRESULT OnDocumentComplete(){return 0;}
	virtual LRESULT OnAttachBehavior( LPNMHL_ATTACH_BEHAVIOR lpab )
	{
		return htmlayout::behavior::handle(lpab);
	}
	
protected:
	///返回子类指针
	T*  GetThis(){
		return (T*)this;
	}

	
	//消息回调函数，使用模板函数，以便对应CHtmalyoutWrapper模板类
	static LRESULT CALLBACK HTMLayoutNotifyHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LPVOID vParam)
	{
		// all HTMLayout notification are comming here.
		NMHDR*  phdr = (NMHDR*)lParam;
		
		CHtmalyoutWrapper<T>* pWrapper = static_cast<CHtmalyoutWrapper<T>*>(vParam);
		
		switch(phdr->code)
		{
		case HLN_CREATE_CONTROL:    return pWrapper->OnCreateControl((LPNMHL_CREATE_CONTROL) lParam);
		case HLN_CONTROL_CREATED:   return pWrapper->OnControlCreated((LPNMHL_CREATE_CONTROL) lParam);
		case HLN_DESTROY_CONTROL:   return pWrapper->OnDestroyControl((LPNMHL_DESTROY_CONTROL) lParam);
		case HLN_LOAD_DATA:         return pWrapper->OnLoadData((LPNMHL_LOAD_DATA) lParam);
		case HLN_DATA_LOADED:       return pWrapper->OnDataLoaded((LPNMHL_DATA_LOADED)lParam);
		case HLN_DOCUMENT_COMPLETE: return pWrapper->OnDocumentComplete();
		case HLN_ATTACH_BEHAVIOR:   return pWrapper->OnAttachBehavior((LPNMHL_ATTACH_BEHAVIOR)lParam );
		case HLN_DOCUMENT_LOADED:   
			{
				LPNMHL_DATA_LOADED X = (LPNMHL_DATA_LOADED)lParam;
				return 0;
			}
		}
		return 0;
	}

	///子窗口创建的时候调用
	void OnCreateCall()
	{
		HTMLayoutSetCallback(GetThis()->m_hWnd, HTMLayoutNotifyHandler, this); // attach handler of htmlayout events
		htmlayout::attach_event_handler(GetThis()->m_hWnd,this); // attach handler of DOM events
	}

	///子窗口中的WindowProc过程中调用
	LRESULT OnWindowProcCall(UINT message, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return HTMLayoutProcND(GetThis()->m_hWnd,message,wParam,lParam, &bHandled);		
	}
};
#endif //HTMLAYOUT_WRAPPER_H