#ifndef _EXTEND_CONTROL_H
#define _EXTEND_CONTROL_H
/************************************************************************/
/* 扩展控件类型头文件                                                   */
/************************************************************************/
#include <Luna.h>
#include <string>
#include <map>
#include <htmlayout.h>

class CControlType
{
public:
	virtual const char*   GetTypeName() = 0;
	//生成控件
	virtual HWND  Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl) = 0;	

	//销毁控件
	virtual bool  Destroy(void* pControl) = 0;			

	//以下函数中pContrl是由Create函数生成的控件

	//属性操作
	//bGet指明是获得属性，还是设置属性
	//bGet = TRUE, 相当于  get(属性名)
	//bGet = FALSE,相当于, set(属性名，属性值)
	//如果处理了，那么bHandled赋值为TRUE,否则为FALSE
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet, BOOL& bHandled) {return 0;}

	//返回支持的函数调用,lua数组方式
	virtual int GetMethods(void* pControl, lua_State* L){return 0;}

	//调用方法 e:call_ctrl_method(name, arg1, arg2, ...);
	virtual int CallMethod(void* pControl, lua_State* L){return 0;}	//调用方法	

	//向控件注册绑定信息
	virtual int BindLuaTable(void* pControl, lua_State* L, int nTableRef) {return 0;}
};

template <typename T, typename CTRLT>
class CControlTypeT : public CControlType
{
public:
	CControlTypeT()
	{
	}	

	CTRLT*  ChangeControlType(void* pContext)
	{
		return (CTRLT*)pContext;
	}

	virtual int GetMethods(void* pControl, lua_State* L)
	{
		CTRLT*  pCtrl = ChangeControlType(pControl);
		if(pCtrl == NULL)
		{			
			return 0;
		}		
		return pCtrl->GetMethods(L);
	}

	//调用方法 e:call_method(name, arg1, arg2, ...);
	virtual int CallMethod(void* pControl, lua_State* L)
	{
		CTRLT*  pCtrl = ChangeControlType(pControl);
		if(pCtrl == NULL)
			return 0;

		if(lua_isnoneornil(L, 1) || PASL(L, 1) <= 0)
			return 0;

		const char* pszName = PAS(L, 1);
		lua_remove(L, 1);

		return pCtrl->CallMethod(pszName, L);
	}
	virtual int BindLuaTable(void* pControl, lua_State* L, int nTableRef)
	{
		CTRLT*  pCtrl = ChangeControlType(pControl);
		if(pCtrl == NULL)
			return 0;
		pCtrl->BindLuaTable(L, nTableRef);
		return 0;
	}
};


//控件消息通知脚本调用
//L, nBindTableRef,是控件的Lua运行环境和元素对应的Lua table
//通过BindLuaTable函数传递给控件，控件需要保留这两个值
//pszFuncName，回调函数名称
//回调参数个数，回调的参数应该在调用此函数之前，通过lua_pushXXXX相关函数放入LuaState堆栈中
inline BOOL SendCallbackMsg(lua_State* L, int nBindTableRef, const char* pszFuncName, int nParams = 0)
{
	if(L == NULL || nBindTableRef == LUA_REFNIL)
		return FALSE;;
	
	int nError = 0;
	int nTop = lua_gettop(L);


	do
	{
		lua_getglobal(L, "errormsg");
		if(!lua_isnil(L, -1))
		{					
			lua_insert(L, nTop - (nParams - 1));
			nTop = lua_gettop(L);
			nError = nTop - nParams;
		}
		else
			lua_remove(L, -1);

		lua_getref(L,	nBindTableRef);
		lua_getfield(L, -1, pszFuncName);
		if(!lua_isfunction(L, -1))
		{
			break;
		}		
		nTop = lua_gettop(L);

		lua_insert(L, nTop - (nParams + 1));
		lua_insert(L, nTop - nParams);
		if(0 == lua_pcall(L, nParams + 1, -1, nError))
		{
			return TRUE;
		}
	}
	while(0);	

	return FALSE;
}

//调用回调函数值钱使用该宏，不能跟其他任何符号
#define BEGIN_LUA_CALLBACK \
	if(m_L != NULL) \
	{\
		Lua_SaveState ss(m_L);

//结束回调时使用该宏，不能跟其他任何符号
#define END_LUA_CALLBACK }

//回调使用方式
/*
BEGIN_LUA_CALLBACK
//设置参数
lua_pushstring(m_L, ...);
...
//调用函数
if(CallLuaMsg(回调函数名称, 参数个数))
{
	回调成功的处理
	...
}
*/

//扩展控件支持lua
#define DECLARE_LUA_SUPPORT_CTRL(TClass)\
	lua_State*  m_L;\
	int         m_nBindTableRef;\
	typedef Lunar<TClass>::RegType RegType;\
	static RegType methods[];\
	void InitLuaEnv()\
	{\
		m_L = NULL;\
		m_nBindTableRef = LUA_REFNIL;\
	}\
	int GetMethods(lua_State* L)\
	{\
		lua_newtable(L);\
		int nIndex = 1;\
		for (RegType *l = methods; l->name; l++) \
		{\
			lua_pushstring(L, l->name);\
			lua_rawseti(L, -2, nIndex++);\
		}\
		return 1;\
	}\
	int CallMethod(const char* pszName, lua_State* L)\
	{\
		for (RegType *l = methods; l->name; l++) \
		{\
			if(0 == strcmp(l->name, pszName))\
			{\
				return (this->*(l->mfunc))(L); 			\
			}\
		}\
		return 0;\
	}\
	void BindLuaTable(lua_State* L, int nBindTableRef)\
	{\
		m_L = L;\
		m_nBindTableRef = nBindTableRef;\
	}\
	BOOL CallLuaMsg(const char* pszFuncName, int nParams = 0)\
	{\
		return SendCallbackMsg(m_L, m_nBindTableRef, pszFuncName, nParams);\
	}

#endif //_EXTEND_CONTROL_H