#ifndef _LUA_BEHAVIOR_H
#define _LUA_BEHAVIOR_H
#include "Luna.h"

#include "htmlayout.h"
#include "htmlayout_dom.h"
#include "htmlayout_behavior.h"
#include "LuaHtmlayoutElment.h"
#include "htmlayout_graphin.h"

namespace htmlayout 
{
 
/** behavior:sizer, 
 * allows to resize elements 
 **/
    
struct lua_behavior: public event_handler
{
    lua_State* m_L;
	image*      surface;
    int m_nBindTableRef;
	int m_nError;	

	bool m_bHave_on_mouse;
	bool m_bHave_on_key;

	bool m_bNoDelete;
    // ctor
    lua_behavior(lua_State* L, HELEMENT he, int nRef): m_L(L), m_nBindTableRef(nRef), m_nError(0), event_handler(0)
	{
		m_bHave_on_key = true;
		m_bHave_on_mouse = true;
		surface = NULL;
		m_bNoDelete = false;

		//获得支持的消息
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_InitBehavior"))
		{			
			CLuaHtmlayoutElement::PushElement(m_L, he);
			lua_pushnumber(m_L, (long)this);
			CallFunction(2);
			subscribed_to = PAI(m_L, -1);
		}		
	}
	virtual void detached  (HELEMENT he ) 
    { 	
		//printf("detach element %d %p\n", he, this);
		if(!m_bNoDelete)
			delete this;
    } 

	virtual ~lua_behavior()
	{
		do
		{
			Lua_SaveState ss(m_L);
			if(GetTableFunction("_on_detach"))
			{
				CallFunction(0);
			}
		}while(0);

		lua_unref(m_L, m_nBindTableRef);
		if(surface)
			delete surface;

		//printf("c++ delete behavior: %p\n", this);
	}
	//获得调用函数
	BOOL GetTableFunction(LPCTSTR lpszFuncname)
	{
		if(m_nBindTableRef == LUA_REFNIL)
		{
			return FALSE;
		}
		
		m_nError = 0;
		
		do
		{
			lua_getglobal(m_L, "errormsg");
			if(!lua_isnil(m_L, -1))
			{
				m_nError = lua_gettop(m_L);
			}
			else
				lua_remove(m_L, -1);
			
			
			lua_getref(m_L,		  m_nBindTableRef);
			lua_getfield(m_L, -1, lpszFuncname);
			if(!lua_isfunction(m_L, -1))
			{
				break;
			}		
			lua_pushvalue(m_L, -2);
			lua_remove(m_L, -3);			
			
			return TRUE;
		}
		while(0);		
		return FALSE;
	}

	//调用对象函数
	void CallFunction(int narg)
	{
		lua_pcall(m_L, 1 + narg, -1, m_nError);
	}
	
	//鼠标消息
	virtual BOOL handle_mouse  (HELEMENT he, MOUSE_PARAMS& params )
	{
		if(!m_bHave_on_mouse)
			return FALSE;

		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_mouse"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			if(he == params.target)
				lua_pushvalue(m_L, -1);
			else
				CLuaHtmlayoutElement::PushElement(m_L, params.target);

			lua_pushnumber(m_L, params.cmd);
			lua_pushnumber(m_L, params.pos.x);
			lua_pushnumber(m_L, params.pos.y);
			lua_pushnumber(m_L, params.button_state);
			lua_pushnumber(m_L, params.alt_state);
			
			lua_pushnumber(m_L, params.cursor_type);
			lua_pushboolean(m_L, params.is_on_icon);
			lua_pushnumber(m_L, params.dragging_mode);

			if(params.dragging == NULL)
				lua_pushnil(m_L);
			else
				CLuaHtmlayoutElement::PushElement(m_L, params.dragging);

			CallFunction(11);
			BOOL bHandled = lua_toboolean(m_L, -5);
			params.cursor_type = PAI(m_L, -4);
			params.is_on_icon  = lua_toboolean(m_L, -3);
			params.dragging_mode = PAI(m_L, -2);
			params.dragging = (HELEMENT)PAI(m_L, -1);
			return bHandled;
		}	
		m_bHave_on_mouse = false;
		return FALSE; 
	}
    virtual BOOL on_key    (HELEMENT he, HELEMENT target, UINT event_type, UINT code, UINT keyboardStates ) 
	{ 
		if(!m_bHave_on_key)
			return FALSE;

		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_key"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			if(he == target)
				lua_pushvalue(m_L, -1);
			else
				CLuaHtmlayoutElement::PushElement(m_L, target);
			
			lua_pushnumber(m_L, event_type);
			lua_pushnumber(m_L, code);
			lua_pushnumber(m_L, keyboardStates);
			
			CallFunction(5);
			BOOL bHandled = lua_toboolean(m_L, -1);
			return bHandled;
		}	
		m_bHave_on_key = false;
		return FALSE; 
	}
    virtual BOOL on_focus  (HELEMENT he, HELEMENT target, UINT event_type ) 
	{ 
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_focus"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			if(he == target)
				lua_pushvalue(m_L, -1);
			else
				CLuaHtmlayoutElement::PushElement(m_L, target);
			
			lua_pushnumber(m_L, event_type);
			CallFunction(3);
			BOOL bHandled = lua_toboolean(m_L, -1);
			return bHandled;
		}	
		return FALSE; 
	}
    virtual BOOL on_timer  (HELEMENT he ) 
	{ 
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_timer"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			CallFunction(1);
			BOOL bHandled = lua_toboolean(m_L, -1);
			return bHandled;
		}	
		return FALSE; 
	}
    virtual BOOL on_timer  (HELEMENT he, UINT_PTR extTimerId ) 
	{ 
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_timer"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			lua_pushnumber(m_L, extTimerId);
			CallFunction(2);
			BOOL bHandled = lua_toboolean(m_L, -1);
			return bHandled;
		}	
		return FALSE; /*stop this timer*/ 
	}
    virtual BOOL on_draw   (HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc ) 
	{ 		
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_draw"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			lua_pushnumber(m_L, draw_type);
			lua_pushnumber(m_L, (long)hdc);
			lua_pushnumber(m_L, rc.left);
			lua_pushnumber(m_L, rc.top);
			lua_pushnumber(m_L, rc.right);
			lua_pushnumber(m_L, rc.bottom);

			CallFunction(7);			
			BOOL bHandled = lua_toboolean(m_L, -1);
			return bHandled;
		}	
		return FALSE; /*do default draw*/ 
	}
    virtual void on_size   (HELEMENT he ) 
	{ 
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_size"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			CallFunction(1);			
		}	
	}
	
    virtual BOOL on_method_call (HELEMENT he, UINT methodID, METHOD_PARAMS* params ) 
	{ 
		return FALSE; /*not handled*/ 
	}
	
    // calls from CSSS! script. Override this if you want your own methods to the CSSS! namespace.
    // Follwing declaration:
    // #my-active-on {
    //    when-click: r = self.my-method(1,"one");
    // } 
    // will end up with on_script_call(he, "my-method" , 2, argv, retval ); 
    // where argv[0] will be 1 and argv[1] will be "one". 
    virtual BOOL on_script_call(HELEMENT he, LPCSTR name, UINT argc, json::value* argv, json::value& retval) 
	{
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_script_call"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			lua_pushstring(m_L, name);
			
			for(int i = 0; i < argc; ++i)
			{				
				json::value& v = argv[i];
				if(v.is_null() || v.is_undefined())
					lua_pushnil(m_L);
				else if(v.is_bool())
					lua_pushboolean(m_L, v.get(false));
				else if(v.is_int())
					lua_pushnumber(m_L, v.get(0));
				else if(v.is_float())
					lua_pushnumber(m_L, v.get(0.0));
				else if(v.is_string())				
					lua_pushstring(m_L, (const char*)aux::w2a(argv[i].to_string(CVT_SIMPLE).c_str()));
				else if(v.is_dom_element())
				{
					HELEMENT h = (HELEMENT)v.get_object_data();					
					CLuaHtmlayoutElement::PushElement(m_L, h);
				}
				else
				{
					lua_pushnil(m_L);
				}
			}
			
			CallFunction(argc + 2);
			retval = lua_tostring(m_L, -1);
			return TRUE;
		}	
		return FALSE; 	
	}
	
    // notification events from builtin behaviors - synthesized events: BUTTON_CLICK, VALUE_CHANGED
    // see enum BEHAVIOR_EVENTS
    virtual BOOL on_event (HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason ) 
	{ 
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_event"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			if(he == target)
				lua_pushvalue(m_L, -1);
			else
				CLuaHtmlayoutElement::PushElement(m_L, target);

			lua_pushnumber(m_L, type);
			lua_pushnumber(m_L, reason);
			
			CallFunction(4);
			BOOL bHandled = lua_toboolean(m_L, -1);
			return bHandled;
		}	
		return FALSE; 
	}
	
    // notification event: data requested by HTMLayoutRequestData just delivered
    virtual BOOL on_data_arrived (HELEMENT he, HELEMENT initiator, LPCBYTE data, UINT dataSize, UINT dataType ) 
	{
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_data_arrived"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			if(he == initiator)
				lua_pushvalue(m_L, -1);
			else
				CLuaHtmlayoutElement::PushElement(m_L, initiator);
			
			lua_pushlstring(m_L, (const char*)data, dataSize);
			lua_pushnumber(m_L, dataType);
			
			CallFunction(4);
			BOOL bHandled = lua_toboolean(m_L, -1);
			return bHandled;
		}	
		return FALSE; 
	}
	
    virtual BOOL on_scroll( HELEMENT he, HELEMENT target, SCROLL_EVENTS cmd, INT pos, BOOL isVertical ) 
	{
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_scroll"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			if(he == target)
				lua_pushvalue(m_L, -1);
			else
				CLuaHtmlayoutElement::PushElement(m_L, target);
			
			lua_pushnumber(m_L, cmd);
			lua_pushnumber(m_L, pos);
			lua_pushboolean(m_L, isVertical);
			
			CallFunction(5);
			BOOL bHandled = lua_toboolean(m_L, -1);
			return bHandled;
		}	
		
		return FALSE; 
	}

	virtual BOOL handle_exchange (HELEMENT he, EXCHANGE_PARAMS& params)
	{
		Lua_SaveState ss(m_L);
		if(GetTableFunction("_on_exchange"))
		{
			CLuaHtmlayoutElement::PushElement(m_L, he);
			lua_pushnumber(m_L, params.cmd);
			CLuaHtmlayoutElement::PushElement(m_L, params.target);
			lua_pushnumber(m_L, params.pos.x);
			lua_pushnumber(m_L, params.pos.y);

			lua_pushnumber(m_L, params.pos_view.x);
			lua_pushnumber(m_L, params.pos_view.y);

			lua_pushnumber(m_L, params.data_types);
			lua_pushnumber(m_L, params.drag_cmd);

			CallFunction(9);
			BOOL bHandled = lua_toboolean(m_L, -1);
			return bHandled;
		}	
		return FALSE; 
	}

	static int ChangeBehavior(lua_State* L)
	{
		HELEMENT he = (HELEMENT)PAI(L, 1);
		UINT   nSubScription = PAI(L, 2);
		lua_behavior* pTag = (lua_behavior *)PAI(L, 3);

		if(he == 0)
			return 0;
		
		if(pTag)
		{
			pTag->m_bNoDelete = true;
			detach_event_handler(he, pTag);
			attach_event_handler(he, pTag, nSubScription);
			pTag->m_bNoDelete = false;

			//printf("change behavior %d %p\n", he, pTag);
		}
		else
		{
			if(lua_istable(L, -1))
			{
				int nRef = lua_ref(L, true);
				pTag = new lua_behavior(L, he, nRef); 
				attach_event_handler(he, pTag, nSubScription);
			}
		}
		return 0;
	}
};
}

#endif //_LUA_BEHAVIOR_H
