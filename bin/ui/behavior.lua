module('ui.behavior', package.seeall);
require('ui.element');

local ui = ui;
--需要处理的消息类型
local HANDLE_MOUSE = 0x0001;              --/** mouse events */
local HANDLE_KEY = 0x0002;                --/** key events */
local HANDLE_FOCUS = 0x0004;              --/** focus events; if this flag is set it also means that element it attached to is focusable */
local HANDLE_SCROLL = 0x0008;             --/** scroll events */
local HANDLE_TIMER = 0x0010;              --/** timer event */
local HANDLE_SIZE = 0x0020;               --/** size changed event */
local HANDLE_DRAW = 0x0040;               --/** drawing request (event) */
local HANDLE_DATA_ARRIVED = 0x080;        --/** requested data () has been delivered */
local HANDLE_BEHAVIOR_EVENT = 0x0100;     --/** secondary, synthetic events:
									--BUTTON_CLICK, HYPERLINK_CLICK, etc.,
									--a.k.a. notifications from intrinsic behaviors */
local HANDLE_METHOD_CALL = 0x0200;        --/** behavior specific methods */
local HANDLE_EXCHANGE    = 0x1000;        --/** system drag-n-drop events */
local HANDLE_ALL = 0xFFFF;                --/** all of them */
local HANDLE_SELF = 0x10000000;		    --自定义，只是为了获得删除消息



local _event = {
---鼠标按键定义
MAIN_MOUSE_BUTTON = 1; --aka left button
PROP_MOUSE_BUTTON = 2; --aka right button
MIDDLE_MOUSE_BUTTON = 4;--

--鼠标操作定义
MOUSE_ENTER = 0;
MOUSE_LEAVE = 1;
MOUSE_MOVE  = 2;
MOUSE_UP    = 3;
MOUSE_DOWN  = 4;
MOUSE_DCLICK = 5;
MOUSE_WHEEL = 6;
MOUSE_TICK  = 7; -- mouse pressed ticks
MOUSE_IDLE  = 8; -- mouse stay idle for some time

DROP        = 9;   -- item dropped, target is that dropped item
DRAG_ENTER  = 0xA; -- drag arrived to the target element that is one of current drop targets.
DRAG_LEAVE  = 0xB; -- drag left one of current drop targets. target is the drop target element.
DRAG_REQUEST = 0xC;  -- drag src notification before drag start. To cancel - return true from handler.

DRAGGING = 0x100; -- This flag is 'ORed' with MOUSE_ENTER..MOUSE_DOWN codes if dragging operation is in effect.
				-- E.g. event DRAGGING | MOUSE_MOVE is sent to underlying DOM elements while dragging.

DRAGGING_ENTER = 0x100;
DRAGGING_LEAVE = 0X101;
DRAGGING_MOVE  = 0X102;
DRAGGING_UP    = 0X103;
DRAGGING_DOWN  = 0x104;

--键盘消息
KEY_DOWN = 0,
KEY_UP = 1,
KEY_CHAR = 2,
--滚动条消息类型
SCROLL_HOME = 0;
SCROLL_END  = 1;
SCROLL_STEP_PLUS = 2;
SCROLL_STEP_MINUS = 3;
SCROLL_PAGE_PLUS = 4;
SCROLL_PAGE_MINUS = 5;
SCROLL_POS = 6;
SCROLL_SLIDER_RELEASED = 7;

--on_event消息类型
BUTTON_CLICK = 0,              -- click on button
BUTTON_PRESS = 1,              -- mouse down or key down in button
BUTTON_STATE_CHANGED = 2,      -- checkbox/radio/slider changed its state/value 
EDIT_VALUE_CHANGING = 3,       -- before text change
EDIT_VALUE_CHANGED = 4,        -- after text change
SELECT_SELECTION_CHANGED = 5,  -- selection in <select> changed
SELECT_STATE_CHANGED = 6,      -- node in select expanded/collapsed, heTarget is the node

POPUP_REQUEST   = 7,           -- request to show popup just received, 
			     --     here DOM of popup element can be modifed.
POPUP_READY     = 8,           -- popup element has been measured and ready to be shown on screen,
			     --     here you can use functions like ScrollToView.
POPUP_DISMISSED = 9,           -- popup element is closed,
			     --     here DOM of popup element can be modifed again - e.g. some items can be removed
			     --     to free memory.

MENU_ITEM_ACTIVE = 0xA,        -- menu item activated by mouse hover or by keyboard,
MENU_ITEM_CLICK = 0xB,         -- menu item click, 
			     --   BEHAVIOR_EVENT_PARAMS structure layout
			     --   BEHAVIOR_EVENT_PARAMS.cmd - MENU_ITEM_CLICK/MENU_ITEM_ACTIVE   
			     --   BEHAVIOR_EVENT_PARAMS.heTarget - the menu item, presumably <li> element
			     --   BEHAVIOR_EVENT_PARAMS.reason - BY_MOUSE_CLICK | BY_KEY_CLICK

CONTEXT_MENU_SETUP   = 0xF,    -- evt.he is a menu dom element that is about to be shown. You can disable/enable items in it.      
CONTEXT_MENU_REQUEST = 0x10,   -- "right-click", BEHAVIOR_EVENT_PARAMS::he is current popup menu HELEMENT being processed or NULL.
			     -- application can provide its own HELEMENT here (if it is NULL) or modify current menu element.

VISIUAL_STATUS_CHANGED = 0x11, -- broadcast notification, sent to all elements of some container being shown or hidden   
DISABLED_STATUS_CHANGED = 0x12,-- broadcast notification, sent to all elements of some container that got new value of :disabled state

POPUP_DISMISSING = 0x13,       -- popup is about to be closed


-- "grey" event codes  - notfications from behaviors from this SDK 
HYPERLINK_CLICK = 0x80,        -- hyperlink click
TABLE_HEADER_CLICK =0x81,            -- click on some cell in table header, 
			     --     target = the cell, 
			     --     reason = index of the cell (column number, 0..n)
TABLE_ROW_CLICK =0x82,               -- click on data row in the table, target is the row
			     --     target = the row, 
			     --     reason = index of the row (fixed_rows..n)
TABLE_ROW_DBL_CLICK =0x83,           -- mouse dbl click on data row in the table, target is the row
			     --     target = the row, 
			     --     reason = index of the row (fixed_rows..n)

ELEMENT_COLLAPSED = 0x90,      -- element was collapsed, so far only behavior:tabs is sending these two to the panels
ELEMENT_EXPANDED =0x91,              -- element was expanded,

ACTIVATE_CHILD =0x92,                -- activate (select) child, 
			     -- used for example by accesskeys behaviors to send activation request, e.g. tab on behavior:tabs. 

DO_SWITCH_TAB =0x92,-- command to switch tab programmatically, handled by behavior:tabs 
			     -- use it as HTMLayoutPostEvent(tabsElementOrItsChild, DO_SWITCH_TAB, tabElementToShow, 0);

INIT_DATA_VIEW = 0x93,                -- request to virtual grid to initialize its view

ROWS_DATA_REQUEST = 0x94,             -- request from virtual grid to data source behavior to fill data in the table
			     -- parameters passed throug DATA_ROWS_PARAMS structure.

UI_STATE_CHANGED = 0x95,              -- ui state changed, observers shall update their visual states.
			     -- is sent for example by behavior:richtext when caret position/selection has changed.

FORM_SUBMIT = 0x96,                   -- behavior:form detected submission event. BEHAVIOR_EVENT_PARAMS::data field contains data to be posted.
			     -- BEHAVIOR_EVENT_PARAMS::data is of type T_MAP in this case key/value pairs of data that is about 
			     -- to be submitted. You can modify the data or discard submission by returning TRUE from the handler.
FORM_RESET = 0x97,                    -- behavior:form detected reset event (from button type=reset). BEHAVIOR_EVENT_PARAMS::data field contains data to be reset.
			     -- BEHAVIOR_EVENT_PARAMS::data is of type T_MAP in this case key/value pairs of data that is about 
			     -- to be rest. You can modify the data or discard reset by returning TRUE from the handler.
			     
DOCUMENT_COMPLETE = 0x98,             -- behavior:frame have complete document.

HISTORY_PUSH = 0x99,                  -- behavior:history stuff
HISTORY_DROP = 0x9A,                     
HISTORY_PRIOR = 0x9B,
HISTORY_NEXT = 0x9C,

HISTORY_STATE_CHANGED = 0x9D,         -- behavior:history notification - history stack has changed

CLOSE_POPUP = 0x9E,                   -- close popup request,
REQUEST_TOOLTIP = 0x9F,               -- request tooltip, BEHAVIOR_EVENT_PARAMS.he <- is the tooltip element.

ANIMATION         = 0xA0,      -- animation started (reason=1) or ended(reason=0) on the element.


FIRST_APPLICATION_EVENT_CODE = 0x100 
}


_G.Event = _event;	--全局的消息类型

_G.DRAGGING_TYPE = 
{
	NO_DRAGGING = 0;
    DRAGGING_MOVE = 1;
    DRAGGING_COPY = 2;
};
--全局变量
local _Global = {
	--get_location 参数定义
	ROOT_RELATIVE = 0x01;       -- - or this flag if you want to get HTMLayout window relative coordinates,
							  --   otherwise it will use nearest windowed container e.g. popup window.
	SELF_RELATIVE = 0x02;       -- - "or" this flag if you want to get coordinates relative to the origin
							  --   of element iself.
	CONTAINER_RELATIVE = 0x03;  -- - position inside immediate container.
	VIEW_RELATIVE = 0x04;       -- - position relative to view - HTMLayout window

	CONTENT_BOX = 0x00;   -- content (inner)  box
	PADDING_BOX = 0x10;   -- content + paddings
	BORDER_BOX  = 0x20;   -- content + paddings + border
	MARGIN_BOX  = 0x30;   -- content + paddings + border + margins

	BACK_IMAGE_AREA = 0x40; -- relative to content origin - location of background image (if it set no-repeat)
	FORE_IMAGE_AREA = 0x50; -- relative to content origin - location of foreground image (if it set no-repeat)

	SCROLLABLE_AREA = 0x60;   -- scroll_area - scrollable area in content box

	--set_html 参数
	SIH_REPLACE_CONTENT   = 0; --SIH_REPLACE_CONTENT - replace content of the element                              
	SIH_INSERT_AT_START   = 1; --SIH_INSERT_AT_START - insert html before first child of the element               
	SIH_APPEND_AFTER_LAST = 2; --SIH_APPEND_AFTER_LAST - insert html after last child of the element               
				                                                                                     
	SOH_REPLACE           = 3; --SOH_REPLACE - replace element by html, a.k.a. element.outerHtml = "something"     
	SOH_INSERT_BEFORE     = 4; --SOH_INSERT_BEFORE - insert html before the element                                 
	SOH_INSERT_AFTER      = 5; --SOH_INSERT_AFTER - insert html after the element   
	--*   ATTN: SOH_*** operations do not work for inline elements like <SPAN> or <INPUT> ...
};
_G.Htmlayout = _Global;
-------------------------------------------------------------------------------

local GetElementObj = ui.element.GetElementObj;
require('bit');
local msg = 
{
	['on_mouse'] = HANDLE_MOUSE;
	['on_size']  = HANDLE_SIZE;
	['on_event'] = HANDLE_BEHAVIOR_EVENT;
	['on_controlevent'] = HANDLE_BEHAVIOR_EVENT;
	['on_key']   = HANDLE_KEY;
	['on_focus'] = HANDLE_FOCUS;
	['on_draw']  = HANDLE_DRAW;
	['on_timer'] = HANDLE_TIMER;
	['on_scroll']= HANDLE_SCROLL;
	['on_exchange'] = HANDLE_EXCHANGE;

	['on_detach']=HANDLE_SELF;
};

--函数别名
local func_rename = 
{
	['on_controlevent'] = 'on_event';
};
--空函数
local empty_func = function() end;

--判断是否有回调函数
local function JudgeHaveCallback(t, func, flag)
	if type(t[func]) == 'function' then	
		func = func_rename[func] or func;
		rawset(t, '_' .. func, t['base_' .. func]);
		return flag;
	elseif t['_' .. func] == nil then
		rawset(t, '_' .. func, '#');
	end
	
	return 0;
end


base = {};
base.InitBehavior = empty_func;
function base:_InitBehavior(he, handler)
	self._handler = handler;	--程序中的处理句柄
	he = GetElementObj(he);	

	if self.on_attach then
		self:on_attach(he);
	end

	local handle_msg = 0;
	for k, v in pairs(msg) do
		handle_msg = bit.bor(handle_msg, JudgeHaveCallback(self, k, v));
	end

	self._handle_event = handle_msg;	--处理的类型
	return handle_msg;
end

function base:refresh_behavior(name, value)
	if msg[name] == nil or type(value) ~= 'function' then
		return;
	end
	
	local handle_msg = 0;
	for k, v in pairs(msg) do
		handle_msg = bit.bor(handle_msg, JudgeHaveCallback(self, k, v));
	end

	if handle_msg == self._handle_event then
		--print(os.date(), 'no change');
		return;
	end
	--print(os.date(), 'change behavior', string.format('0x%X', handle_msg));
	self._handle_event = handle_msg;	--处理的类型	
	bind_behavior(self:get_handle(),  self._handle_event, self._handler, self);
end

--判断鼠标滚轮消息
local max_int = 2^31 - 1;
local function wheel_change(event_type, v)
	if bit.band(event_type, 0xFFF) ~= _event.MOUSE_WHEEL then return nil; end
	if v > max_int then
		return v - 2 ^ 32;
	end
	return v;
end

--判断是否返回鼠标按键状态
local function judge_button(event_type, mousebtns, flagbit)
	if bit.band(event_type, 0xFFF) == _event.MOUSE_WHEEL then return nil; end
	return bit.band(mousebtns, flagbit) > 0;
end

function base:base_on_mouse(he, target, event_type, x, y, mousebtns, keyboardStates, cursor_type, is_on_icon, dragging_mode, dragging)	
	local evt = {
		type = event_type,
		x        = x;
		y        = y;

		issink   = (bit.band(event_type, 0x8000) > 0);
		ishandled= (bit.band(event_type, 0x10000) > 0);
		target   = GetElementObj(target),
		altKey   = (bit.band(keyboardStates, 0x4) > 0),	--判断Alt键是否按下
		ctrlKey  = (bit.band(keyboardStates, 0x1) > 0),	--判断Ctrl键是否按下
		shiftKey = (bit.band(keyboardStates, 0x2) > 0),	--判断Shift键是否按下

		wheelDelta = wheel_change(event_type, mousebtns);
		mainButton = judge_button(event_type, mousebtns, 0x01),	--判断鼠标左键是否按下
		propButton = judge_button(event_type, mousebtns, 0x02),		--判断鼠标右键是否按下
		middleButton=judge_button(event_type, mousebtns, 0x04),		--判断鼠标中键是否按下
		x1Button=    judge_button(event_type, mousebtns, 0x08),		--判断鼠标x1键是否按下
		x2Button=    judge_button(event_type, mousebtns, 0x10),		--判断鼠标x2键是否按下

		cursor_type = cursor_type;
		is_on_icon  = is_on_icon;
		dragging_mode = dragging_mode;
		dragging  = dragging;
	};
	return self:on_mouse(evt), evt.cursor_type, evt.is_on_icon, evt.dragging_mode, ui.element.GetElementHandle(evt.dragging);
end

function base:base_on_size(he)
	return self:on_size();
end

function base:base_on_event(he, target, event_type, reason)
	target = GetElementObj(target);
	if bit.band(event_type, 0x8000) > 0  then
		return false;
	end

	if bit.band(event_type, 0x10000) >= 0  and self.on_controlevent then
		event_type = bit.band(event_type, bit.bnot(0x10000));
		return self:on_controlevent{
			type = event_type,
			target = target;
			reason = reason;
		};
	end

	if self.on_event then
		return self:on_event{
			type = event_type,
			target = target;
			reason = reason;
		};
	end

	return false;
end

function base:base_on_key(he, target, event_type, code, keyboardStates)
	return self:on_key{
		type = event_type,

		target   = GetElementObj(target),
		altKey   = (bit.band(keyboardStates, 0x4) > 0),	--判断Alt键是否按下
		ctrlKey  = (bit.band(keyboardStates, 0x1) > 0),	--判断Ctrl键是否按下
		shiftKey = (bit.band(keyboardStates, 0x2) > 0),	--判断Shift键是否按下

		keyCode  = code;

	};
end

function base:base_on_draw(he, draw_type, hdc, left, top, right, bottom)
	if draw_type ~= 1 then
		return false;
	end

	if self.graphics_handle then	
		local g = self.graphics_handle;		
		g:blit(hdc, left, top, 0, 0, math.min(right-left, g:get_width()), math.min(bottom-top, g:get_height()));
	end
	return false;
end

function base:base_on_timer(he, tid)
	if self:onAnimate(tid) then return true; end;
	return self:on_timer(tid);
end

function base:base_on_detach()
	ui.element.FreeElementObj(self);
end

function base:base_on_scroll(he, target, cmd, pos, isvertical)
	local evt = {
		type = cmd;
		target = GetElementObj(target),
	};

	if cmd == _event.SCROLL_POS then
		evt.scrollPos = pos;
	end
	evt.isvertical = isvertical;
	return self:on_scroll(evt);
end

function base:base_on_exchange(he, cmd, target, posx, posy, viewx, viewy, data_types, drag_cmd)
	return self:on_exchange{
		target = GetElementObj(target);
		cmd    = cmd;
		pos = {
			x = posx, 
			y = posy,
		},
		pos_view = {
			x = viewx, 
			y = viewy,
		},
		data_types = data_types;
		drag_cmd   = drag_cmd;
	};
end