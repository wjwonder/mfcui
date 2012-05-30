#ifndef _LUA_HTMLAYOUT_ELEMENT_H
#define _LUA_HTMLAYOUT_ELEMENT_H
/************************************************************************/
/* lua包装htmlayout element对象操作                                     */
/************************************************************************/

#include <htmlayout.h>
#include <htmlayout_dom.hpp>
#include <aux-cvt.h>
#include <Luna.h>
#include "exwidget.h"

class CLuaHtmlayoutElement
{
public:
	static const char className[];					//类名
	static Lunar<CLuaHtmlayoutElement>::RegType methods[];	//需要导出的函数
	
protected:
	HWND  m_hSelfHwnd;				//自定义控件的窗口句柄
	void* m_pContext;				//自定义控件的上下文句柄
	CControlType* m_pControlType;	//自动控件的类型
	int   m_nBindTableRef;			//当前元素绑定的lua table引用
	htmlayout::dom::element  m_domelement;	//htmlayout中的Dom操作对象
	lua_State*	m_L;
	CLuaHtmlayoutElement(){};
public:
	CLuaHtmlayoutElement(HELEMENT h)
		:m_domelement(h)
	{		
		check_widget();
	}

	CLuaHtmlayoutElement(lua_State *L)
		:m_domelement((HELEMENT)PAI(L, 1))
		{
			check_widget();
		};
	
	~CLuaHtmlayoutElement()
	{
		if(m_nBindTableRef != LUA_REFNIL)
		{	
			lua_unref(m_L, m_nBindTableRef);
			m_nBindTableRef = LUA_REFNIL;
		}		
	}

public:
	static void PushElement(lua_State* L, HELEMENT h)
	{
		if(h == NULL)
		{
			lua_pushnil(L);
			return;
		}

		//CLuaHtmlayoutElement* ph = new CLuaHtmlayoutElement(h);
		//Lunar<CLuaHtmlayoutElement>::push(L, ph, true);		
		lua_pushnumber(L, (long)h);
	}

	static CLuaHtmlayoutElement* ToElement(lua_State* L, int nIndex)
	{
		return Lunar<CLuaHtmlayoutElement>::check(L, nIndex);
	}

	static HELEMENT ToElementHanle(lua_State* L, int nIndex)
	{
		return (HELEMENT)PAI(L, nIndex);
	}
public:
	void detach(htmlayout::event_handler* pHandle)
	{
		m_domelement.detach(pHandle);
	}

	//检查自定义控件的相关属性
	void check_widget()
	{
		m_hSelfHwnd = NULL;
		m_pContext  = NULL;
		m_pControlType  = NULL;
		m_nBindTableRef = LUA_REFNIL;

		const wchar_t* pValue = m_domelement.get_attribute("_self_hwnd_");
		if(pValue)
			m_hSelfHwnd = (HWND)wcstol(pValue, NULL, 16);

		pValue = m_domelement.get_attribute("_self_widget_");
		if(pValue)
			m_pContext =  (void *)wcstol(pValue, NULL, 16);

		if(m_hSelfHwnd)
		{
			pValue = m_domelement.get_attribute("type");
			m_pControlType = GetWidgetType(aux::w2a(pValue ? pValue : L""));
		}
	}
public:
	//获得属性值
	//value = e:get_attribute(name)
	//属性不存在的时候返回nil
	int get_attribute(lua_State* L);

	//设置属性值
	//e:set_attribute(string name, [string value])
	//value 为nil时，删除属性
	int set_attribute(lua_State* L);

	//设置Style属性
	//e:set_style_attribute(string name, [string value])
	//value 为nil时，删除属性
	int set_style_attribute(lua_State* L);

	//获得Style属性
	//value = e:get_style_attribute(string name)
	//属性不存在的时候返回nil
	int get_style_attribute(lua_State* L);

	//查找dom元素，返回找到的第一个
	//PatterString 参考css选择器
	//e:find_element(selector)
	int find_element(lua_State* L);

	//查找所有匹配的元素，返回数组table
	//t = e:find_all(selector)
	int find_all(lua_State* L);

	//设置元素值
	//e:set_value(string value)
	int set_value(lua_State* L);

	//获得元素值
	//value = e:get_avlue();
	int get_value(lua_State* L);

	//创建新元素
	//ne = e:create_element(string tagName, [string text]);
	int create_element(lua_State* L);

	//设置元素html内容
	//e:set_html(string htmlstring, [number mode])
	int set_html(lua_State* L);

	//获取元素html内容
	//e:get_html([boolean bouter]);
	int get_html(lua_State* L);

	//将当前元素和制定元素交换
	//e:swap(e2)
	int swap(lua_State* L);

	//获得当前元素在父元素中的序号
	//index = e:index()
	int index(lua_State* L);

	//返回父元素
	//he = e:parent();
	int parent(lua_State* L);

	//移动元素
	//e:move(x, y, [w], [h]);
	int move(lua_State* L);

	//获得子元素个数
	//count = e:children_count();
	int children_count(lua_State* L);

	//获得第n个子元素,从0开始
	//he = e:child(number n)
	int child(lua_State* L);

	//下一个兄弟元素
	//he = e:next_sibling();
	int next_sibling(lua_State* L);

	//前一个兄弟元素
	//he = e:prev_sibling();
	int prev_sibling(lua_State* L);

	//第一个兄弟元素
	//he = e:first_sibling();
	int first_sibling(lua_State* L);

	//最后一个兄弟元素
	//he = e:last_sibling();
	int last_sibling(lua_State* L);

	//删除当前元素
	//e:destroy();
	int destroy(lua_State* L);

	//插入新元素
	//e:insert(ne, index);
	int insert(lua_State* L);

	//追加元素
	//e:append(ne);
	int append(lua_State* L);

	//弹出显示元素
	//e:show_popup(x, y, banimate);
	int show_popup(lua_State* L);

	//隐藏弹出显示
	//e:hide_popup();
	int hide_popup(lua_State* L);

	//复制当前元素
	//ne = e:clone();
	int clone(lua_State* L);

	//将当前元素从父元素中脱离
	//e:detach()
	int detach(lua_State* L);

	//隐藏和此元素相关的窗口
	int hide_element_hwnd(lua_State* L);

	//更新
	//e:update();
	int update(lua_State* L);

	//刷新
	//e:refresh([bool bNeedMeasure]);
	int refresh(lua_State* L);

	//加载页面文件
	// e:load_html(url);
	int load_html(lua_State* L);

	//清空元素下的HTML内容
	// e:clear();
	int clear(lua_State* L);

	//判断元素是否有效
	//boo e:is_valid();
	int is_valid(lua_State* L);

	//滚动以便显示当前元素
	//e:scroll_to_view(bTopOfView = false, bSmooth = false)
	int scroll_to_view(lua_State* L);

	//设置滚动条位置
	//e:set_scroll_pos(x, y);
	int set_scroll_pos(lua_State* L);

	//获得滚动条信息
	//x, y = e:get_scroll_info();
	int get_scroll_info(lua_State* L);

	//获得元素的位置信息
	//left,top, right, bottom = e:get_location(type)
	int get_location(lua_State* L);

	//捕获鼠标消息
	//e:set_capture();
	int set_capture(lua_State* L);

	//释放鼠标捕获
	//e:release_capture();
	int release_capture(lua_State* L);

	//清除所有通过set_style_attribute设置的属性
	//e:cleare_all_style_attributes();
	int clear_all_style_attributes(lua_State* L);

	//获得根元素
	//e:root();
	int root(lua_State* L);

	//获得文本
	//text = e:text();
	int text(lua_State* L);

	//设置文本
	//e:set_text(txt);
	int set_text(lua_State* L);

	//获得元素类型
	//t = e:type();
	int get_type(lua_State* L);

	//合并url
	//url = e:combine_url(url)
	int combine_url(lua_State* L);
	
	//获取状态
	//bool  e:get_state(state);
	int get_state(lua_State* L);

	//设置状态
	//e:set_state(state, on[off]);
	int set_state(lua_State* L);

	//获得元素句柄
	//handle = e:get_handle();
	int get_handle(lua_State* L);

	//获得最近的html元素
	//e:get_html_element();
	int get_html_element(lua_State* L);

	//启动定时器
	//e:start_timer(millisecond, [tid])
	int start_timer(lua_State* L);

	//关闭定时器
	//e:stop_timer([tid])
	int stop_timer(lua_State* L);

	//页面重新加载
	//e:reload(url);
	int reload(lua_State* L);

	//查找最近的符合条件的父元素
	//he = e:find_nearest_parent(selector);
	int find_nearest_parent(lua_State* L);

	//调用控件方法
	int call_ctrl_method(lua_State* L);

	//获得控件的方法
	int get_ctrl_methods(lua_State* L);

	//获得控件属性
	int get_ctrl_attribute(lua_State* L);

	//设置控件属性
	int set_ctrl_attribute(lua_State* L);

	//发送post消息
	int post_event(lua_State* L);

	//发送send消息
	int send_event(lua_State* L);

	//向程序注册绑定的lua table信息
	//e:register_bind_table(t);
	int register_bind_table(lua_State* L)
	{
		if(!lua_istable(L, 1))
			return 0;

		m_L = L;
		if(m_nBindTableRef != LUA_REFNIL)
		{
			lua_unref(L, m_nBindTableRef);
			m_nBindTableRef = LUA_REFNIL;
		}

		m_nBindTableRef = lua_ref(L, true);
		if(m_nBindTableRef != LUA_REFNIL && m_pControlType && m_pContext)
		{
			m_pControlType->BindLuaTable(m_pContext, L, m_nBindTableRef);
		}
		return 0;	
	}

	//获得自增数字
	int get_uid(lua_State* L)
	{
		static long sNO = 1;
		InterlockedIncrement(&sNO);
		lua_pushnumber(L, sNO);
		return 1;
	}

	//删除自定义窗口
	int destroy_ctrl(lua_State* L)
	{
		if(m_pControlType == NULL || m_pContext == NULL)
			return 0;
		
		lua_pushboolean(L, m_pControlType->Destroy(m_pContext));
		return 1;
	}

	//获得容器窗口句柄
	int get_root_hwnd(lua_State* L)
	{
		lua_pushnumber(L, (long)m_domelement.get_element_hwnd(true));
		return 1;
	}

	//判断当前元素是否可见
	//he:visible():bool
	int is_visible(lua_State* L)
	{
		lua_pushboolean(L, m_domelement.visible());
		return 1;
	}

	//
	int is_enabled(lua_State* L)
	{
		lua_pushboolean(L, m_domelement.enabled());		
		return 1;
	}

	int scroll_set_values(lua_State* L)
	{
		htmlayout::dom::scrollbar s = m_domelement;
		s.set_values(PAI(L, 1), PAI(L, 2),PAI(L, 3),PAI(L, 4),PAI(L, 5));
		return 0;
	}

	int scroll_get_values(lua_State* L)
	{
		htmlayout::dom::scrollbar s = m_domelement;
		int  v[5];
		s.get_values(v[0], v[1], v[2], v[3], v[4]);
		lua_pushnumber(L, v[0]);
		lua_pushnumber(L, v[1]);
		lua_pushnumber(L, v[2]);
		lua_pushnumber(L, v[3]);
		lua_pushnumber(L, v[4]);
		return 5;
	}

	int scroll_set_value(lua_State* L)
	{
		htmlayout::dom::scrollbar s = m_domelement;
		s.set_value(PAI(L, 1));
		return 0;
	}

	int scroll_get_value(lua_State* L)
	{
		htmlayout::dom::scrollbar s = m_domelement;
		lua_pushnumber(L, s.get_value());
		return 1;
	}

	//对子元素排序
	int sort(lua_State* L);

	//获得所有带name属性的子元素的值
	int get_child_values(lua_State* L)
	{
		htmlayout::named_values vals;
		if(!htmlayout::get_values(m_domelement, vals))
			return 0;

		lua_newtable(L);
		htmlayout::named_values_iterator iter = vals.begin();
		while(iter != vals.end())
		{
			lua_pushstring(L, aux::w2a(iter->second.to_string().c_str()));
			lua_setfield(L, -2, aux::w2a(iter->first.c_str()));
			++iter;
		}
		return 1;
	}

	json::value  StringToDate(const char* pszValue)
	{
		int wYear  = 2011;
		int wMonth = 12;
		int wDay   = 7;
		int wHour  = 14;
		int wMin   = 0;
		int wSecond = 0;

		bool bDate = false;
		bool bTime = false;
		do
		{
			if(sscanf(pszValue, "%4d-%2d-%2d", &wYear, &wMonth, &wDay) == 3)
			{
				bDate = true;
				break;
			}

			if(sscanf(pszValue, "%4d%2d%2d", &wYear, &wMonth, &wDay) == 3)
			{
				bDate = true;
				break;
			}

			if(sscanf(pszValue, "%2d:%2d:%2d", &wHour, &wMin, &wSecond) == 3)
			{
				bTime = true;
				break;
			}

			if(sscanf(pszValue, "%2d:%2d", &wHour, &wMin) == 2)
			{
				bTime = true;
				break;
			}

		}while(0);
		SYSTEMTIME st;
		FILETIME   ft;
		GetSystemTime(&st);
		if(bDate)
		{
			st.wYear = wYear;
			st.wMonth = wMonth;
			st.wDay = wDay;
		}
		if(bTime)
		{
			st.wHour = wHour;
			st.wMinute = wMin;
			st.wSecond = wSecond;
		}
		SystemTimeToFileTime(&st, &ft);
		return json::value::date(ft);
	}

	//设置元素值，相同名称可以同时设置
	inline void _set_value(const char* pszName, const char* pszValue)
	{
		char szSelector[256] = {0};
		_snprintf(szSelector, sizeof(szSelector), "[name='%s']", pszName);

		json::value value = json::value::from_string((const wchar_t*)aux::a2w(pszValue));			

		htmlayout::selected_cb selected;
		m_domelement.find_all(&selected, szSelector);

		bool bCheckedSet = false;
		bool bRadioSet   = false;
		for( unsigned int n = 0; n < selected.elements.size(); ++n )
		{			
			htmlayout::dom::element& t = selected.elements[n];
			if( !t.get_style_attribute("behavior") )
			{
				const wchar_t* pszData = aux::a2w(pszValue);
				aux::w2utf utf8(pszData);
				t.set_html( utf8, utf8.length() );				
				continue;
			}
			CTL_TYPE ty = t.get_ctl_type();
			if(ty == CTL_CHECKBOX)
			{
				if(bCheckedSet)
					continue;
				bCheckedSet = true;
			}
			else if(ty == CTL_RADIO)
			{
				if(bRadioSet)
					continue;
				bRadioSet = true;
			}	
			else if(ty == CTL_DATE || ty == CTL_TIME)
			{				
				htmlayout::set_value(t, StringToDate(pszValue));
				continue;
			}
			htmlayout::set_value(t, value);
		}
		return ;
	}
	int set_child_values(lua_State* L)
	{					
		int nIndex = 0;
		lua_pushnil(L);  /* first key */
		while (lua_next(L, 1) != 0) {
			_set_value(PAS(L, -2), PAS(L,-1));
			lua_pop(L, 1);  /* removes `value'; keeps `key' for next iteration */
		}
		lua_pop(L, 1);		
		return 0;
	}

};
#endif
