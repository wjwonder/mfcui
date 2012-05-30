#include "stdafx.h"
#include "ExtendWidget.h"
#include "LuaHtmlayoutElment.h"
#include "htmlayout_behavior.hpp"


const char CLuaHtmlayoutElement::className[] = "LuaHtmlayoutElement";
Lunar<CLuaHtmlayoutElement>::RegType CLuaHtmlayoutElement::methods[] =
{
	method(CLuaHtmlayoutElement, is_valid),
	method(CLuaHtmlayoutElement, find_element),	
	method(CLuaHtmlayoutElement, find_all),	
	method(CLuaHtmlayoutElement, get_attribute),	
	method(CLuaHtmlayoutElement, set_attribute),	
	method(CLuaHtmlayoutElement, set_value),	
	method(CLuaHtmlayoutElement, get_value),
	method(CLuaHtmlayoutElement, create_element),
	method(CLuaHtmlayoutElement, get_style_attribute),
	method(CLuaHtmlayoutElement, set_style_attribute),
	method(CLuaHtmlayoutElement, set_html),
	method(CLuaHtmlayoutElement, get_html),

	method(CLuaHtmlayoutElement, swap),
	method(CLuaHtmlayoutElement, index),
	method(CLuaHtmlayoutElement, parent),
	method(CLuaHtmlayoutElement, move),
	method(CLuaHtmlayoutElement, children_count),
	method(CLuaHtmlayoutElement, child),
	method(CLuaHtmlayoutElement, next_sibling),
	method(CLuaHtmlayoutElement, prev_sibling),
	method(CLuaHtmlayoutElement, first_sibling),
	method(CLuaHtmlayoutElement, last_sibling),
	method(CLuaHtmlayoutElement, destroy),
	method(CLuaHtmlayoutElement, insert),
	method(CLuaHtmlayoutElement, append),
	method(CLuaHtmlayoutElement, show_popup),
	method(CLuaHtmlayoutElement, hide_popup),
	method(CLuaHtmlayoutElement, clone),
	method(CLuaHtmlayoutElement, detach),
	method(CLuaHtmlayoutElement, hide_element_hwnd),
	method(CLuaHtmlayoutElement, scroll_to_view),
	method(CLuaHtmlayoutElement, set_scroll_pos),
	method(CLuaHtmlayoutElement, get_scroll_info),

	method(CLuaHtmlayoutElement, update),
	method(CLuaHtmlayoutElement, refresh),
	method(CLuaHtmlayoutElement, load_html),
	method(CLuaHtmlayoutElement, clear),
	method(CLuaHtmlayoutElement, get_location),

	method(CLuaHtmlayoutElement, set_capture),
	method(CLuaHtmlayoutElement, release_capture),
	method(CLuaHtmlayoutElement, root),
	method(CLuaHtmlayoutElement, text),
	method(CLuaHtmlayoutElement, set_text),

	method(CLuaHtmlayoutElement, set_state),
	method(CLuaHtmlayoutElement, get_state),

	method(CLuaHtmlayoutElement, get_handle),
	
	method(CLuaHtmlayoutElement, combine_url),
	method(CLuaHtmlayoutElement, get_type),
	method(CLuaHtmlayoutElement, clear_all_style_attributes),

	method(CLuaHtmlayoutElement, start_timer),
	method(CLuaHtmlayoutElement, stop_timer),
	method(CLuaHtmlayoutElement, reload),

	method(CLuaHtmlayoutElement, find_nearest_parent),
	method(CLuaHtmlayoutElement, get_html_element),	

	method(CLuaHtmlayoutElement, get_ctrl_methods),	
	method(CLuaHtmlayoutElement, call_ctrl_method),	

	method(CLuaHtmlayoutElement, get_ctrl_attribute),
	method(CLuaHtmlayoutElement, set_ctrl_attribute),
	method(CLuaHtmlayoutElement, register_bind_table),

	method(CLuaHtmlayoutElement, get_uid),
	method(CLuaHtmlayoutElement, destroy_ctrl),
	method(CLuaHtmlayoutElement, get_root_hwnd),

	method(CLuaHtmlayoutElement, get_child_values),
	method(CLuaHtmlayoutElement, set_child_values),

	method(CLuaHtmlayoutElement, scroll_set_values),
	method(CLuaHtmlayoutElement, scroll_get_values),
	method(CLuaHtmlayoutElement, scroll_set_value),
	method(CLuaHtmlayoutElement, scroll_get_value),

	method(CLuaHtmlayoutElement, is_enabled),
	method(CLuaHtmlayoutElement, is_visible),

	method(CLuaHtmlayoutElement, post_event),
	method(CLuaHtmlayoutElement, send_event),

	method(CLuaHtmlayoutElement, sort),

	
	{0, 0}
};

int CLuaHtmlayoutElement::get_attribute(lua_State* L)
{
	if(m_pControlType && m_pContext)
	{
		BOOL bHandled = FALSE;
		int nRet = m_pControlType->Attribute(m_pContext, L, TRUE, bHandled);
		if(bHandled)
			return nRet;
	}

	if(lua_isnoneornil(L, 1))
	{
		lua_newtable(L);
		int nCount = m_domelement.get_attribute_count();
		for(int i = 0; i < nCount; ++i)
		{
			lua_pushstring(L, m_domelement.get_attribute_name(i));
			lua_pushstring(L, (const char*)aux::w2a(m_domelement.get_attribute(i)));
			lua_settable(L, -3);
		}
		return 1;
	}

	const wchar_t* pValue = m_domelement.get_attribute(PAS(L, 1));
	if(pValue == 0)
	{
		lua_pushnil(L);
	}
	else
	{
		lua_pushstring(L, (const char*)aux::w2a(pValue));
	}
	return 1;
}

int CLuaHtmlayoutElement::set_attribute(lua_State* L)
{
	if(m_pControlType && m_pContext)
	{
		BOOL bHandled = FALSE;
		int nRet = m_pControlType->Attribute(m_pContext, L, FALSE, bHandled);
		if(bHandled)
			return nRet;
	}

	if(lua_isnoneornil(L, 2))
	{
		m_domelement.set_attribute(PAS(L, 1), 0);
	}
	else
	{
		m_domelement.set_attribute(PAS(L, 1), aux::a2w(PAS(L, 2)));
	}
	return 0;
}

int CLuaHtmlayoutElement::set_style_attribute(lua_State* L)
{
	if(lua_isnoneornil(L, 2))
	{		
		m_domelement.clear_style_attribute(PAS(L, 1));
	}
	else
	{
		m_domelement.set_style_attribute(PAS(L, 1), aux::a2w(PAS(L, 2)));
	}
	return 0;
}

int CLuaHtmlayoutElement::get_style_attribute(lua_State* L)
{
	lua_pushstring(L, (const char*)aux::w2a(m_domelement.get_style_attribute(PAS(L, 1))));
	return 1;
}

int CLuaHtmlayoutElement::find_element(lua_State* L)
{
	HELEMENT h =  m_domelement.find_first(PAS(L, 1));		
	PushElement(L, h);
	return 1;
}

class Find_ALL : public htmlayout::dom::callback
{
public:  
	Find_ALL(lua_State* L, int nTableIndex)
		:m_L(L),m_nTableIndex(nTableIndex),m_nIndex(1)
	{}
	virtual bool on_element(HELEMENT he)
	{
		CLuaHtmlayoutElement::PushElement(m_L, he);
		lua_rawseti(m_L, m_nTableIndex, m_nIndex ++);
		return false;
	}
protected:
	lua_State* m_L;
	int   m_nTableIndex;
	int   m_nIndex;
};

int CLuaHtmlayoutElement::find_all(lua_State* L)
{	
	lua_newtable(L);
	Find_ALL fa(L, lua_gettop(L));
	m_domelement.select_elements(&fa, PAS(L, 1));
	return 1;

}
int CLuaHtmlayoutElement::set_value(lua_State* L)
{
	CTL_TYPE ty = m_domelement.get_ctl_type();
	json::value v;
	if(ty == CTL_DATE || ty == CTL_TIME)
	{
		v = StringToDate(PAS(L, 1));
	}
	else if(lua_isboolean(L, 1))
	{
		v = lua_toboolean(L, 1);
	}
	else
		v = json::value::from_string((const wchar_t*)aux::a2w(PAS(L, 1)));			

	htmlayout::set_value(m_domelement, v);
	return 0;
}

int CLuaHtmlayoutElement::get_value(lua_State* L)
{
	json::value v = m_domelement.get_value();
	if(v.is_bool())
	{
		lua_pushboolean(L, v.get(false));
	}
	else if(v.is_int())
	{
		lua_pushnumber(L, v.get(0));
	}
	else if(v.is_float())
	{
		lua_pushnumber(L, v.get(0.0f));
	}
	else
	{
		lua_pushstring(L, aux::w2a(v.to_string().c_str()));
	}
	return 1;
}

int CLuaHtmlayoutElement::create_element(lua_State* L)
{	
	htmlayout::dom::element h = NULL;
	if(lua_isnoneornil(L, 2))
	{
		h = htmlayout::dom::element::create(PAS(L, 1), 0);
	}
	else
	{
		h = htmlayout::dom::element::create(PAS(L, 1), aux::a2w(PAS(L, 2)));
	}
	HTMLayout_UseElement(h);
	PushElement(L, h);
	return 1;
}

int CLuaHtmlayoutElement::set_html(lua_State* L)
{	
	aux::w2utf f = aux::w2utf(aux::a2w(PAS(L, 1)));
	m_domelement.set_html((const byte *)f, f.length(), PAI(L, 2));
	return 0;
}

int CLuaHtmlayoutElement::get_html(lua_State* L)
{	
	CString sv = aux::w2a(aux::utf2w(m_domelement.get_html(lua_toboolean(L, 1))));
	lua_pushstring(L, sv);
	return 1;
}

int CLuaHtmlayoutElement::swap(lua_State* L)
{
	HELEMENT h = ToElementHanle(L, 1);
	if(h == NULL)
		return 0;

	m_domelement.swap(h);
	lua_pushboolean(L, true);
	return 1;
}

int CLuaHtmlayoutElement::index(lua_State* L)
{
	lua_pushnumber(L, m_domelement.index());
	return 1;
}

int CLuaHtmlayoutElement::parent(lua_State* L)
{
	PushElement(L, m_domelement.parent());
	return 1;
}

int CLuaHtmlayoutElement::move(lua_State* L)
{
	if(lua_isnoneornil(L, 3))
	{
		m_domelement.move(PAI(L, 1), PAI(L, 2));
	}
	else
	{
		m_domelement.move(PAI(L, 1), PAI(L, 2), PAI(L, 3), PAI(L, 4));
	}
	return 0;
}

int CLuaHtmlayoutElement::children_count(lua_State* L)
{
	lua_pushnumber(L, m_domelement.children_count());
	return 1;
}

int CLuaHtmlayoutElement::child(lua_State* L)
{
	PushElement(L, m_domelement.child(PAI(L, 1)));
	return 1;
}

int CLuaHtmlayoutElement::next_sibling(lua_State* L)
{
	PushElement(L, m_domelement.next_sibling());
	return 1;
}


int CLuaHtmlayoutElement::prev_sibling(lua_State* L)
{
	PushElement(L, m_domelement.prev_sibling());
	return 1;
}

int CLuaHtmlayoutElement::first_sibling(lua_State* L)
{
	PushElement(L, m_domelement.first_sibling());
	return 1;
}

int CLuaHtmlayoutElement::last_sibling(lua_State* L)
{
	PushElement(L, m_domelement.last_sibling());
	return 1;
}

int CLuaHtmlayoutElement::destroy(lua_State* L)
{
	m_domelement.destroy();
	return 0;
}

int CLuaHtmlayoutElement::insert(lua_State* L)
{
	HELEMENT h = ToElementHanle(L, 1);
	if(h == NULL)
		return 0;

	m_domelement.insert(h, PAI(L, 2));
	HTMLayout_UnuseElement(h);
	PushElement(L, h);
	return 1;
}

int CLuaHtmlayoutElement::append(lua_State* L)
{
	HELEMENT h = ToElementHanle(L, 1);
	if(h == NULL)
		return 0;
	
	m_domelement.append(h);
	HTMLayout_UnuseElement(h);
	PushElement(L, h);
	return 1;
}


int CLuaHtmlayoutElement::show_popup(lua_State* L)
{
	if(!lua_isnoneornil(L, 3))
	{
		POINT pt;
		pt.x = PAI(L, 1);
		pt.y = PAI(L, 2);
		m_domelement.show_popup(pt, PAI(L, 3));
	}
	else
	{		
		HELEMENT h = ToElementHanle(L, 1);
		if(h == NULL)
			return 0;

		HTMLayoutShowPopup(m_domelement, h, PAI(L, 2));
	}
	return 0;
}

int CLuaHtmlayoutElement::hide_popup(lua_State* L)
{
	m_domelement.hide_popup();
	return 0;
}

int CLuaHtmlayoutElement::clone(lua_State* L)
{
	htmlayout::dom::element e = m_domelement.clone();
	HTMLayout_UseElement(e);
	PushElement(L, e);
	
	return 1;
}

int CLuaHtmlayoutElement::detach(lua_State* L)
{
	m_domelement.detach();
	return 0;
}

int CLuaHtmlayoutElement::hide_element_hwnd(lua_State* L)
{
	BOOL bHide = FALSE;
	HWND hWnd = m_domelement.get_element_hwnd(false);
	if(::IsWindow(hWnd))
	{
		::ShowWindow(hWnd, SW_HIDE);
		bHide = TRUE;
	}
	lua_pushboolean(L, bHide);
	return 1;
}

int CLuaHtmlayoutElement::refresh(lua_State* L)
{
	m_domelement.refresh();
	return 0;
}

int CLuaHtmlayoutElement::update(lua_State* L)
{
	int nMode = PAI(L, 1);
	if(nMode == 0)
		nMode = REDRAW_NOW;
	m_domelement.update(nMode);
	return 0;
}

int CLuaHtmlayoutElement::load_html(lua_State* L)
{
	wchar_t uri[2048];
	wcsncpy(uri,aux::a2w(PAS(L, 1)),2048);
	m_domelement.combine_url(uri,2047);
	uri[2047] = 0;
	
	m_domelement.load_html(uri);
	return 0;
}

int CLuaHtmlayoutElement::clear(lua_State* L)
{
	m_domelement.clear();
	return 0;
}

int CLuaHtmlayoutElement::is_valid(lua_State* L)
{
	bool bValide = (HTMLayout_UnuseElement(m_domelement) == HLDOM_OK);
	lua_pushboolean(L,  bValide);
	if(bValide)
	{
		HTMLayout_UnuseElement(m_domelement);
	}

	return 1;
}

int CLuaHtmlayoutElement::scroll_to_view(lua_State* L)
{
	m_domelement.scroll_to_view(PAI(L, 1), PAI(L, 2));
	return 0;
}

int CLuaHtmlayoutElement::set_scroll_pos(lua_State* L)
{
	POINT pt;
	pt.x = PAI(L, 1);
	pt.y = PAI(L, 2);
	m_domelement.set_scroll_pos(pt, PAI(L, 3));
	return 0;
}

int CLuaHtmlayoutElement::get_scroll_info(lua_State* L)
{
	POINT pt;
	RECT  rt;
	SIZE  sz;
	m_domelement.get_scroll_info(pt, rt, sz);
	lua_pushnumber(L, pt.x);
	lua_pushnumber(L, pt.y);
	lua_pushnumber(L, sz.cx);
	lua_pushnumber(L, sz.cy);
	lua_pushnumber(L, rt.left);
	lua_pushnumber(L, rt.right);
	lua_pushnumber(L, rt.top);
	lua_pushnumber(L, rt.bottom);
	return 8;
}

int CLuaHtmlayoutElement::get_location(lua_State* L)
{
	RECT rect = m_domelement.get_location(PAI(L, 1));	
	lua_pushnumber(L, rect.left);
	lua_pushnumber(L, rect.top);
	lua_pushnumber(L, rect.right);
	lua_pushnumber(L, rect.bottom);
	return 4;
}


int CLuaHtmlayoutElement::set_capture(lua_State* L)
{
	m_domelement.set_capture();
	return 0;
}


int CLuaHtmlayoutElement::release_capture(lua_State* L)
{
	m_domelement.release_capture();
	return 0;
}

int CLuaHtmlayoutElement::clear_all_style_attributes(lua_State* L)
{
	m_domelement.clear_all_style_attributes();
	return 0;
}

int CLuaHtmlayoutElement::root(lua_State* L)
{
	CLuaHtmlayoutElement::PushElement(L, m_domelement.root());
	return 1;
}

int CLuaHtmlayoutElement::text(lua_State* L)
{
	lua_pushstring(L, aux::w2a(m_domelement.text().c_str()));
	return 1;
}

int CLuaHtmlayoutElement::set_text(lua_State* L)
{
	m_domelement.set_text(aux::a2w(PAS(L, 1)));
	return 0;
}

int CLuaHtmlayoutElement::get_type(lua_State* L)
{
	lua_pushstring(L, m_domelement.get_element_type());
	return 1;
}

int CLuaHtmlayoutElement::combine_url(lua_State* L)
{
	wchar_t uri[2048];
	wcsncpy(uri,aux::a2w(PAS(L, 1)),2048);
	m_domelement.combine_url(uri,2047);
	uri[2047] = 0;

	lua_pushstring(L, aux::w2a(uri));
	return 1;
}
int CLuaHtmlayoutElement::get_state(lua_State* L)
{
	lua_pushboolean(L, m_domelement.get_state((unsigned int)PAI(L, 1)));
	return 1;
}

int CLuaHtmlayoutElement::set_state(lua_State* L)
{
	m_domelement.toggle_state(PAI(L, 1), lua_toboolean(L, 2));
	return 0;
}

int CLuaHtmlayoutElement::get_handle(lua_State* L)
{
	lua_pushnumber(L, (long)(HELEMENT)m_domelement);
	return 1;
}

int CLuaHtmlayoutElement::find_nearest_parent(lua_State* L)
{
	PushElement(L, m_domelement.find_nearest_parent(PAS(L, 1)));
	return 1;
}

int CLuaHtmlayoutElement::get_html_element(lua_State* L)
{
	
	PushElement(L, m_domelement.find_nearest_parent("html"));
	return 1;
}

int CLuaHtmlayoutElement::start_timer(lua_State* L)
{
	m_domelement.start_timer(PAI(L, 1), PAI(L, 2));
	return 0;
}

int CLuaHtmlayoutElement::stop_timer(lua_State* L)
{
	m_domelement.stop_timer(PAI(L, 1));
	return 0;
}


int CLuaHtmlayoutElement::reload(lua_State* L)
{
	HWND hWnd = m_domelement.get_element_hwnd(true);
	if(!::IsWindow(hWnd))
		return 0;

	if(::HTMLayoutLoadFile(hWnd, aux::a2w(PAS(L, 1))))
	{
		CLuaHtmlayoutElement::PushElement(L, htmlayout::dom::element::root_element(hWnd));
		return 1;
	}
	return 0;
}

int CLuaHtmlayoutElement::call_ctrl_method(lua_State* L)
{
	if(m_pControlType == NULL || m_pContext == NULL)
		return 0;
	
	return m_pControlType->CallMethod(m_pContext, L);
}

int CLuaHtmlayoutElement::get_ctrl_methods(lua_State* L)
{
	if(m_pControlType == NULL || m_pContext == NULL)
		return 0;
	
	return m_pControlType->GetMethods(m_pContext,  L);
}

int CLuaHtmlayoutElement::get_ctrl_attribute(lua_State* L)
{
	if(m_pControlType == NULL || m_pContext == NULL)
		return 0;

	BOOL bHandled = FALSE;
	return m_pControlType->Attribute(m_pContext, L, TRUE, bHandled);
}

int CLuaHtmlayoutElement::set_ctrl_attribute(lua_State* L)
{
	if(m_pControlType == NULL || m_pContext == NULL)
		return 0;

	BOOL bHandled = FALSE;
	return m_pControlType->Attribute(m_pContext, L, FALSE, bHandled);
}

int CLuaHtmlayoutElement::post_event(lua_State* L)
{
	m_domelement.post_event(PAI(L, 1), PAI(L, 2), ToElementHanle(L, 3));
	return 0;
}

int CLuaHtmlayoutElement::send_event(lua_State* L)
{
	lua_pushboolean(L, m_domelement.send_event(PAI(L, 1), PAI(L, 2), ToElementHanle(L, 3)));
	return 1;
}

struct _lua_sort : public htmlayout::dom::element::comparator
{
	lua_State* m_L;
	int        m_nFuncIndex;
	_lua_sort(lua_State* L, int nFuncIndex)
		:m_L(L), m_nFuncIndex(nFuncIndex)
	{

	}
	virtual int compare(const htmlayout::dom::element& e1, const htmlayout::dom::element& e2)
	{
		lua_pushvalue(m_L, m_nFuncIndex);
		CLuaHtmlayoutElement::PushElement(m_L, e1);
		CLuaHtmlayoutElement::PushElement(m_L, e2);
		if(lua_pcall(m_L, 2, 1, 0) == 0)
		{
			return PAI(m_L, -1);
		}
		return 0;
	}
};

//¶Ô×ÓÔªËØÅÅÐò
int CLuaHtmlayoutElement::sort(lua_State* L)
{
	_lua_sort _s(L, 3);

	m_domelement.sort(_s, PAI(L, 1), PAI(L, 2));
	return 0;
}