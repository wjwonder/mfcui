module('ui.element', package.seeall);
require('ui.comm');
require('ui.behavior');

--包装C++中定义的LuaHtmlayoutElement对象---------------------------------------
local PRE_COLOR = 
{
    aliceblue = '#f0f8ff',
    antiquewhite = '#faebd7',
    aqua = '#00ffff',
    aquamarine = '#7fffd4',
    azure = '#f0ffff',
    beige = '#f5f5dc',
    bisque = '#ffe4c4',
    black = '#000000',
    blanchedalmond = '#ffebcd',
    blue = '#0000ff',
    blueviolet = '#8a2be2',
    brown = '#a52a2a',
    burlywood = '#deb887',
    cadetblue = '#5f9ea0',
    chartreuse = '#7fff00',
    chocolate = '#d2691e',
    coral = '#ff7f50',
    cornflowerblue = '#6495ed',
    cornsilk = '#fff8dc',
    crimson = '#dc143c',
    cyan = '#00ffff',
    darkblue = '#00008b',
    darkcyan = '#008b8b',
    darkgoldenrod = '#b8860b',
    darkgray = '#a9a9a9',
    darkgreen = '#006400',
    darkkhaki = '#bdb76b',
    darkmagenta = '#8b008b',
    darkolivegreen = '#556b2f',
    darkorange = '#ff8c00',
    darkorchid = '#9932cc',
    darkred = '#8b0000',
    darksalmon = '#e9967a',
    darkseagreen = '#8fbc8f',
    darkslateblue = '#483d8b',
    darkslategray = '#2f4f4f',
    darkturquoise = '#00ced1',
    darkviolet = '#9400d3',
    deeppink = '#ff1493',
    deepskyblue = '#00bfff',
    dimgray = '#696969',
    dodgerblue = '#1e90ff',
    firebrick = '#b22222',
    floralwhite = '#fffaf0',
    forestgreen = '#228b22',
    fuchsia = '#ff00ff',
    gainsboro = '#dcdcdc',
    ghostwhite = '#f8f8ff',
    gold = '#ffd700',
    goldenrod = '#daa520',
    gray = '#808080',
    green = '#008000',
    greenyellow = '#adff2f',
    honeydew = '#f0fff0',
    hotpink = '#ff69b4',
    indianred = '#cd5c5c',
    indigo = '#4b0082',
    ivory = '#fffff0',
    khaki = '#f0e68c',
    lavender = '#e6e6fa',
    lavenderblush = '#fff0f5',
    lawngreen = '#7cfc00',
    lemonchiffon = '#fffacd',
    lightblue = '#add8e6',
    lightcoral = '#f08080',
    lightcyan = '#e0ffff',
    lightgoldenrodyellow = '#fafad2',
    lightgreen = '#90ee90',
    lightgray = '#d3d3d3',
    lightpink = '#ffb6c1',
    lightsalmon = '#ffa07a',
    lightseagreen = '#20b2aa',
    lightskyblue = '#87cefa',
    lightslategray = '#778899',
    lightsteelblue = '#b0c4de',
    lightyellow = '#ffffe0',
    lime = '#00ff00',
    limegreen = '#32cd32',
    linen = '#faf0e6',
    magenta = '#ff00ff',
    maroon = '#800000',
    mediumaquamarine = '#66cdaa',
    mediumblue = '#0000cd',
    mediumorchid = '#ba55d3',
    mediumpurple = '#9370db',
    mediumseagreen = '#3cb371',
    mediumslateblue = '#7b68ee',
    mediumspringgreen = '#00fa9a',
    mediumturquoise = '#48d1cc',
    mediumvioletred = '#c71585',
    midnightblue = '#191970',
    mintcream = '#f5fffa',
    mistyrose = '#ffe4e1',
    moccasin = '#ffe4b5',
    navajowhite = '#ffdead',
    navy = '#000080',
    oldlace = '#fdf5e6',
    olive = '#808000',
    olivedrab = '#6b8e23',
    orange = '#ffa500',
    orangered = '#ff4500',
    orchid = '#da70d6',
    palegoldenrod = '#eee8aa',
    palegreen = '#98fb98',
    paleturquoise = '#afeeee',
    palevioletred = '#db7093',
    papayawhip = '#ffefd5',
    peachpuff = '#ffdab9',
    peru = '#cd853f',
    pink = '#ffc0cb',
    plum = '#dda0dd',
    powderblue = '#b0e0e6',
    purple = '#800080',
    red = '#ff0000',
    rosybrown = '#bc8f8f',
    royalblue = '#4169e1',
    saddlebrown = '#8b4513',
    salmon = '#fa8072',
    sandybrown = '#f4a460',
    seagreen = '#2e8b57',
    seashell = '#fff5ee',
    sienna = '#a0522d',
    silver = '#c0c0c0',
    skyblue = '#87ceeb',
    slateblue = '#6a5acd',
    slategray = '#708090',
    snow = '#fffafa',
    springgreen = '#00ff7f',
    steelblue = '#4682b4',
    tan = '#d2b48c',
    teal = '#008080',
    thistle = '#d8bfd8',
    tomato = '#ff6347',
    turquoise = '#40e0d0',
    violet = '#ee82ee',
    wheat = '#f5deb3',
    white = '#ffffff',
    whitesmoke = '#f5f5f5',
    yellow = '#ffff00',
    yellowgreen = '#9acd32',
};
_G.RGB = _G.RGB or function(r, g, b, a)
	if type(r) == 'string' then 
		return r; 
	end;

	return string.format('rgb(%d, %d, %d, %d)', r or 0, g or 0, b or o, a or 0);
end;

--包装返回值是element的函数
local function CreateReturnWrapper(lib, func)
	local base_func = lib[func];
	if base_func == nil then return end;

	lib[func] = function(...)
		local he = base_func(unpack(arg));
		if he == nil then return nil; end;
		return GetElementObj(he);
	end
end

function GetElementHandle(e)
	if type(e) == 'table' and e['__element_flag__'] then
		return e.obj:get_handle();
	end
	return e;
end
--包装参数中带element元素的函数
local function CreateParamWrapper(lib, func)
	local base_func = lib[func];
	if base_func == nil then return end;
	lib[func] = function(...)
		for k = 2, #arg do
			arg[k] = GetElementHandle(arg[k]);
		end
		return base_func(unpack(arg));
	end
end


local exElement = LuaHtmlayoutElement or {};

--发送消息
exElement.base_post_event = exElement.post_event;
function exElement:post_event(event_name, reason, source)
	local mid = GetMsgID(event_name);
	return self:base_post_event(mid, reason, source);
end

exElement.base_send_event = exElement.send_event;
function exElement:send_event(event_name, reason, source)
	local mid = GetMsgID(event_name);
	return self:base_send_event(mid, reason, source);
end

local function WrapperSortFunc(f)
	return function(e1, e2)
		local r = f(GetElementObj(e1), GetElementObj(e2));
		if type(r) == 'boolean' then
			if r then
				return -1;
			else
				return 1;
			end
		end
		return r;
	end
end

exElement.base_sort = exElement.sort;
function  exElement:sort(a1, a2, a3)
	local b, e, f = a1, a2, a3;
	--对所有元素排序
	if a2 == nil then
		b, e, f = 0, -1, a1;
	--只指定起始位置
	elseif a3 == nil then
		b, e, f = a1, -1, a2;
	end
	self:base_sort(b, e, WrapperSortFunc(f));
end

local need_change_return = {'find_element', 'child', 'next_sibling', 'prev_sibling', 'first_sibling', 'last_sibling',
			    'parent', 'root', 'get_html_element', 'find_nearest_parent', 'insert', 'append'};
local need_change_param  = {'swap', 'show_popup', 'insert', 'append', 'post_event', 'send_event'};

for k, v in ipairs(need_change_param) do
	CreateParamWrapper(exElement, v);
end

for k, v in ipairs(need_change_return) do
	CreateReturnWrapper(exElement, v);
end

exElement.base_find_all = exElement.find_all;
function exElement:find_all(...)
	local t = self:base_find_all(unpack(arg));
	for k, v in ipairs(t) do
		t[k] = GetElementObj(v);
	end
	return t;
end

exElement.base_load_html = exElement.load_html;
function exElement:load_html(url, ...)
	local EObj = GetElementObj(self:get_handle());
	DeleteFrameModule(EObj);
	DeleteFrameElementTree(EObj);
	
	self:base_load_html(url);
	if self:get_type() == 'frame' then
		LoadFrameScript(self, ...);
	end
end

exElement.F = exElement.find_element;
exElement.select = exElement.find_element;

--在元素末尾增加html
function exElement:append_html(html)
	self:set_html(html, 2);
end
--在元素头部增加html
function exElement:insert_html(html)
	self:set_html(html, 1);
end



---------------
local exG = LuaGraphics or {};
exG.base_drawImage=exG.drawImage;
function exG:drawImage(img, ...)
	if img == nil then return end;
	return self:base_drawImage(img:get_handle(), ...);
end

exG.base_blitImage=exG.blitImage;
function exG:blitImage(img, ...)
	if img == nil then return end;
	return self:base_blitImage(img:get_handle(), ...);
end

exG.base_blendImage=exG.blendImage;
function exG:blendImage(img, ...)
	if img == nil then return end;
	return self:base_blendImage(img:get_handle(), ...);
end

--颜色转换
exG.base_lineColor = exG.lineColor;
function exG:lineColor(c)
	return self:base_lineColor(PRE_COLOR[c] or c);
end

exG.base_fillColor = exG.fillColor;
function exG:fillColor(c)
	return self:base_fillColor(PRE_COLOR[c] or c);
end


local _Graphics = function(width, height, func)	
	local gfx = LuaGraphics(width, height);
	if func then
		func(gfx);
	end

	return gfx;
end
_G.Graphics = _Graphics;



-------------------------------------------------------------------------------

local state_change = 
{
   [':link']		= 0x00000001;--STATE_LINK             = 0x00000001,
   [':hover']		= 0x00000002;--STATE_HOVER            = 0x00000002,
   [':active']		= 0x00000004;--STATE_ACTIVE           = 0x00000004,
   [':focus']		= 0x00000008;--STATE_FOCUS            = 0x00000008,
   [':visited']		= 0x00000010;--STATE_VISITED          = 0x00000010,
   [':current']		= 0x00000020;--STATE_CURRENT          = 0x00000020,  // current (hot) item 
   [':checked']		= 0x00000040;--STATE_CHECKED          = 0x00000040,  // element is checked (or selected)
   [':disabled']	= 0x00000080;--STATE_DISABLED         = 0x00000080,  // element is disabled
   [':readonly']	= 0x00000100;--STATE_READONLY         = 0x00000100,  // readonly input element 
   [':expaned']		= 0x00000200;--STATE_EXPANDED         = 0x00000200,  // expanded state - nodes in tree view 
   [':collapsed']	= 0x00000400;--STATE_COLLAPSED        = 0x00000400,  // collapsed state - nodes in tree view - mutually exclusive with
   [':incomplete']	= 0x00000800;--STATE_INCOMPLETE       = 0x00000800,  // one of fore/back images requested but not delivered
   [':animating']	= 0x00001000;--STATE_ANIMATING        = 0x00001000,  // is animating currently
   [':focusable']	= 0x00002000;--STATE_FOCUSABLE        = 0x00002000,  // will accept focus
   [':anchor']		= 0x00004000;--STATE_ANCHOR           = 0x00004000,  // anchor in selection (used with current in selects)
   [':synthetic']	= 0x00008000;--STATE_SYNTHETIC        = 0x00008000,  // this is a synthetic element - don't emit it's head/tail
   [':owns-popup']	= 0x00010000;--STATE_OWNS_POPUP       = 0x00010000,  // this is a synthetic element - don't emit it's head/tail
   [':tabfocus']	= 0x00020000;--STATE_TABFOCUS         = 0x00020000,  // focus gained by tab traversal
   [':empty']		= 0x00040000;--STATE_EMPTY            = 0x00040000,  // empty - element is empty (text.size() == 0 && subs.size() == 0) 
									--  if element has behavior attached then the behavior is responsible for the value of this flag.
   [':busy']		= 0x00080000;--STATE_BUSY             = 0x00080000,  // busy; loading
   
   [':drag-over']	= 0x00100000;--STATE_DRAG_OVER        = 0x00100000,  // drag over the block that can accept it (so is current drop target). Flag is set for the drop target block
   [':drop-target']	= 0x00200000;--STATE_DROP_TARGET      = 0x00200000,  // active drop target.
   [':moving']		= 0x00400000;--STATE_MOVING           = 0x00400000,  // dragging/moving - the flag is set for the moving block.
   [':copying']		= 0x00800000;--STATE_COPYING          = 0x00800000,  // dragging/copying - the flag is set for the copying block.
   [':drag-source']	= 0x01000000;--STATE_DRAG_SOURCE      = 0x01000000,  // element that is a drag source.
   [':drop-marker']	= 0x02000000;--STATE_DROP_MARKER      = 0x02000000,  // element is drop marker
   
   [':pressed']		= 0x04000000;--STATE_PRESSED          = 0x04000000,  // pressed - close to active but has wider life span - e.g. in MOUSE_UP it 
									--   is still on; so behavior can check it in MOUSE_UP to discover CLICK condition.
   [':popup']		= 0x08000000;--STATE_POPUP            = 0x08000000,  // this element is out of flow - popup 
   [':ltr']		= 0x10000000;--STATE_IS_LTR           = 0x10000000,  // the element or one of its containers has dir=ltr declared
   [':rtl']		= 0x20000000;--STATE_IS_RTL           = 0x20000000,  // the element or one of its containers has dir=rtl declared	
};

--生成支持函数代理的lua导出对象的扩展对象
local function set_element_attribute(t, k, v)
	if k == 'value' then
		t.obj:set_value(v);
	elseif k == 'text' then
		t.obj:set_text(v);
	elseif k == 'html' then
		t.obj:set_html(v);
		AutoLoadBehavior(t);
		
	elseif k == 'prototype' then
		if type(v) == 'table' then
			local finit = v.init;
			for k2, v2 in pairs(v) do
				t[k2] = v2;
			end
			if finit then
				finit(t);
			end

			local proto_list = rawget(t, '_prototype');
			if proto_list then
				table.insert(proto_list, v);				
			else
				rawset(t, '_prototype', {v});
			end
		end
	else
		--print('设置新值', tostring(t), k);
		rawset(t, k, v);
		t:refresh_behavior(k, v);
	end
end

local DEF_LOCATION = 0x24;
local E = {
	__index=function(t, k)
		local obj = rawget(t, 'obj');

		if k == 'value' then
			return obj:get_value();
		elseif k == 'text' then
			return obj:text();
		elseif k == 'html' then
			return obj:get_html();
		elseif k == 'root' then
			return obj:root();
		elseif k == 'parent' then
			return obj:parent();
		elseif k == 'tag' then
			return obj:get_type();
		elseif k == 'id' then
			return obj:get_attribute('id');
		elseif k == 'next' then
			return obj:next_sibling();
		elseif k == 'prior' then
			return obj:prev_sibling();
		elseif k == 'first' then
			return obj:child(0);
		elseif k == 'last' then
			return obj:child(obj:children_count() - 1);		
		elseif k == 'index' then
			return obj:index();
		elseif k == 'childcount' then
			return obj:children_count();
		elseif k == 'x' then
			local x, y, right, bottom  = obj:get_location(DEF_LOCATION);
			return x;
		elseif k == 'y' then
			local x, y, right, bottom  = obj:get_location(DEF_LOCATION);
			return y;
		elseif k == 'width' then
			local x, y, right, bottom  = obj:get_location(DEF_LOCATION);
			return right - x;
		elseif k == 'height' then
			local x, y, right, bottom  = obj:get_location(DEF_LOCATION);
			return bottom - y;
		elseif type(k) == 'number' then
			return obj:child(k);
		end

		local v = rawget(t, k);
		if v  then
			return v;
		end

		local mt  = getmetatable(obj);
		if mt and mt[k] then
			return function(self, ...)									
				local obj = self.obj;
				return obj[k](obj, ...);
			end
		end

		return nil;
	end;

	__tostring = function(t)
		return string.format('E: %08X T: <%s #%s> ', tostring(t.obj:get_handle()), t.obj:get_type(), t['id'] or t['name'] or '');
	end
};


function E:new (obj)
	local o = {};
	o['__element_flag__'] = true;	--元素对象标志
	o.obj = obj;	
	
	--将控件的方法导入到元素的方法中
	for k, v in ipairs(obj:get_ctrl_methods() or {}) do
		o[v] = function(self, ...)
			local obj = self.obj;
			return obj['call_ctrl_method'](obj, v, ...);
		end
	end

	setmetatable(o, E)
	return o
end

--扩展导出的dom元素功能
local ExE = {};

--获得绘图句柄
function ExE:graphics(destroy)
	if destroy  then
		if self.graphics_handle then
			self.graphics_handle = nil;
			collectgarbage();
		end
		return;
	end
	if self.graphics_handle == nil then		
		--print('get graphics', self.tag, self.id, self.width, self.height);
		self.graphics_handle = LuaGraphics(self.width, self.height);
		local needUpBehavior = false;
		if self.on_draw == nil then
			self.on_draw = function() end;			
		end
		if self.on_size == nil then
			self.on_size = function() end;
		end
	end
	return self.graphics_handle;
end

function ExE:loadImage(url)
	url = self:combine_url(url);
	if not string.find(url, '^file://') then return nil; end;

	local f = io.open(string.sub(url, 8), 'rb');
	if f  == nil then return nil; end;
	local d = f:read('*a');
	f:close();

	local g = LuaGraphics();
	if g:loadImageFromBytes(d) then
		return g;
	end
	g = nil;
	return nil;
end

--注册帮助信息
function ExE:add_help(a, v)
	local help = self._help or {};
	if type(a) == 'table' then
		for k, v in pairs(a) do
			help[k] = v;
		end
	else
		help[a] = v;
	end
	self._help = help;
end

--获得帮助信息
function ExE:get_help(a)
	if self._help == nil then
		return nil;
	end

	if a == nil then
		return self._help;
	else
		return self._help[a];
	end
end

--调用animate函数
local function CallAnimateFunc(func) 
	local t = func();
	if t == nil or type(t) ~= 'number' or t <= 0 then
		return 0;
	end
	return t;
end

--onTimer的回调函数
function ExE:onAnimate(tid)
	local alist = self.animate_list;
	if alist == nil or  alist[tid] == nil then return false; end;

	local t = CallAnimateFunc(alist[tid].callback);
	if t <= 0 then
		self:stop_timer(tid);
		alist[tid] = nil;
	elseif t ~= alist[tid].timer then
		self:stop_timer(tid);
		self:start_timer(t, tid);
	end
	return true;
end
function ExE:animate(func, delay) 
	local t = delay or CallAnimateFunc(func);
	if t <= 0 then
		return;
	end

	self.animate_list = self.animate_list or {};
	local tid = self:get_uid();
	self.animate_list[tid] = 
	{
		timer    = t;
		callback = func;
	}
	
	if self.on_timer == nil then
		self.on_timer = function() end;
	end
	self:start_timer(t, tid);
end

--遍历子元素
--用法 for e, k in p:children() do ... end
--第一个返回值是子元素，第二个返回值是子元素的索引（base 0）
function ExE:children()
	local i = 0;
	local n = self.childcount;
	return function()
		if i < n  then
			local  obj = self[i];
			i = i + 1;
			return obj, i - 1;
		end
	end
end

--对每个子元素执行回调函数f(child, index);
function ExE:foreach(f)
	for c, i in self:children() do
		f(c, i);
	end
end

--生成特定的元素metatable
local function CreateObjMetaTable(luaobj, obj, mt)
	local t  = {obj = obj, luaobj = luaobj};
	setmetatable(t, mt);
	return t;
end

--取style属性包装
local _style_mt = {
	__index=function(t, k)
		local mt  = getmetatable(t);
		if mt and mt[k] then
			return mt[k];
		end

		return t.obj:get_style_attribute(k);
	end;

	__newindex = function(t, k, v)
		t.obj:set_style_attribute(k, v);
		local onChange = rawget(t.luaobj, 'OnStyleChange');
		if onChange then
			onChange(t.luaobj, k, v);
		end
	end;

	clear = function(self)
		self.obj:clear_all_style_attributes();
	end;
};

--取属性包装
local _attr_mt = {
	__index=function(t, k)
		if k == 'value' then
			return t.obj:get_value();
		elseif k == 'text' then
			return t.obj:text();
		elseif k == 'html' then
			return t.obj:get_html();
		end
		return t.obj:get_attribute(k);
		
	end;
	__newindex = function(t, k, v)
		if k == 'value' then
			t.obj:set_value(v);
		elseif k == 'text' then
			t.obj:set_text(v);
		elseif k == 'html' then
			t.obj:set_html(v);
		else
			t.obj:set_attribute(k, v);
		end		

		local onChange = rawget(t.luaobj, 'OnAttributeChange');
		if onChange then
			onChange(t.luaobj, k, v);
		end
	end;
};

--取状态包装
local _state_mt = {
	__index=function(t, k)
		return t.obj:get_state(state_change[k] or 0);		
	end;
	__newindex = function(t, k, v)
		t.obj:set_state(state_change[k] or 0, v);
		local onChange = rawget(t.luaobj, 'OnStateChange');
		if onChange then
			onChange(t.luaobj, k, v);
		end
	end;
};

--自定义控件属性包装
local _ctrl_mt = {
	__index=function(t, k)
		return t.obj:get_ctrl_attribute(k);
	end;
	__newindex = function(t, k, v)
		t.obj:set_ctrl_attribute(k, v);
	end;
};

local  _root = {};	--所有元素的根
local  _elements = {};	--所有元素的索引

function ShowAllElement()
	local count = 0;
	local root_cnt = 0;
	for k, v in pairs(_elements) do
		print('root element', k);
		root_cnt = root_cnt + 1;
		for k2, v2 in pairs(v) do
			print(k2, v2, v2.tag, v2.id);
			count = count + 1;
		end
	end
	print('root count:', root_cnt, 'all count:', count);
end

function ShowAllChild(he)
	for i = 0, he.childcount - 1 do
		local child = he[i];
		print(string.format('child[%d]: %s tag:%s id:%s', i, tostring(child), child.tag or '', child.id or ''));
	end
end

local function FormatTag(he)
	local id = he.id;
	if id then
		id = ':#' .. id;
	end
	local name = he.attributes['name'];
	if name then
		name = ' name=' .. name;
	end

	return {handle=he:get_handle(), text=string.format('<%s%s%s>', he.tag or '',id or '', name or '')};
end

function GetParentElement(he)
	local t = {};
	local p = he;
	table.insert(t, 1, FormatTag(he));
	while(true) do
		p = p.parent;
		if p == nil or p.tag == 'html' then
			break;
		end
		table.insert(t, 1, FormatTag(p));
	end
	return t;
end

--删除当前元素
function FreeElementObj(he)
	if type(he) ~= 'table' then
		return;
	end
	
	local he_handle = he:get_handle();
	local root_html = htmlayout.get_html_element(he:get_handle());

	if he_handle == 0 or root_html == 0 then
		return;
	end

	--删除页面中的缓存元素
	local m = GetModuleTable(root_html);
	if m and he['_element_name_'] then
		m[he['_element_name_']] = nil;
	end

	he:destroy_ctrl();
	he:clear_all_parent();
	he = nil;


	local root_elements = _elements[root_html];
	if root_elements == nil then
		return;
	end

	root_elements[he_handle] = nil;
	local isempty = true;
	for k, v in pairs(root_elements) do
		isempty = false;
		break;
	end

	if isempty then
		_elements[root_html] = nil;
		DeleteHtmlModule(root_html);
	end
end

--删除frame元素
function DeleteFrameElementTree(frame)
	if frame:get_type() == 'frame' then
		local f = frame:child(0);
		if f and f:get_type() == 'html' then
			DeleteModuleFromRootHandle(f:get_handle());
		end
		
	end
end

--删除一个根元素
function DeleteModuleFromRootHandle(root_html)
	--print('*****************', 'FreeRoot', root_html, '*******************');
	--ShowAllElement();
	local root_elements = _elements[root_html];
	DeleteFrameModule(GetElementObj(root_html));
	--DeleteHtmlModule(root_html);

	if root_elements == nil then
		return;
	end

	for k, v in pairs(root_elements) do
		v._on_detach = nil;
		v:graphics(true);
		if v.destroy_ctrl then
			v:destroy_ctrl();
		end
		
		v:clear_all_parent();
		v = nil;
	end
	
	_elements[root_html] = nil;
	collectgarbage();
	--ShowAllElement();
end

--创建或者获得元素对应的lua table
--he 元素句柄， 
--behavior 对象，可以为nil
--nocreate 为true是只去已经存在的元素，不自动创建
function GetElementObj(he, nocreate, root_html)
	if he == nil or he == 0 or type(he) == 'table' then
		return he;
	end

	root_html = root_html or htmlayout.get_html_element(he);
	local root_elements = _elements[root_html];

	if root_html == 0 then
		return nil;
	end
	if nocreate and root_elements == nil then
		return nil;
	end

	if root_elements == nil then
		root_elements = {};
		_elements[root_html] = root_elements;

	end
	
	local v = root_elements[he];
	if v then
		return v;
	end

	if nocreate then
		return nil;
	end

	local nhe = E:new(LuaHtmlayoutElement(he));
	local nc = ui.createClass(ui.behavior.base, ExE, nhe);
	nc.__newindex = set_element_attribute;
	v = nc:new();
	nc = nil;

	v.style = CreateObjMetaTable(v, nhe, _style_mt);
	v.attributes  = CreateObjMetaTable(v, nhe, _attr_mt);
	v.state = CreateObjMetaTable(v, nhe, _state_mt);
	v.ctrl  = CreateObjMetaTable(v, nhe, _ctrl_mt);
	v:register_bind_table(v);
	root_elements[he] = v;

	AttachBehavior(he, nil, root_html);

	v.on_detach = function() end;
	return v;
end
