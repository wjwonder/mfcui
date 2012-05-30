require('ui');
local loaded = package.loaded;

local ui = ui;
local GetElementObj = ui.element.GetElementObj;

--html_handle 根元素句柄
local function FormatModuleName(html_handle) 
	return string.format('htmlayout_lua_M%08X', html_handle);
end

--获得当前元素所在的脚本模块空间名称
local function GetElementModuleName(e)
	local html = e:get_html_element();
	if html == nil then	--
		return '';
	end
	return FormatModuleName(html:get_handle());
end

--获得frame标签的脚本模块名称
local function GetFrameModuleName(frame) 
	if frame == nil or (frame.tag ~= 'frame'  and frame.tag ~= 'html') then
		return nil;
	end
	local first = frame.first;
	if first == nil then
		return nil;
	end

	return GetElementModuleName(first);
end


--删除当前元素包含的脚本空间，主要是给frame用
function DeleteFrameModule(e)
	local modulename = GetFrameModuleName(e);
	if modulename == nil  then
		return;
	end

	local allframe = e:find_all('frame');
	for k, v in ipairs(allframe) do
		DeleteFrameModule(v);
		ui.element.DeleteFrameElementTree(v);		
	end

	loaded[modulename] = nil;
	_G[modulename] = nil;

	collectgarbage();
end

--删除模块
--html_handle 模块ID
function DeleteHtmlModule(html_handle)
	local modulename = FormatModuleName(html_handle);
	--print('delete module', modulename);
	loaded[modulename] = nil;
	_G[modulename] = nil;

	collectgarbage();
end

function GetModuleTable(html_handle) 
	local modulename = FormatModuleName(html_handle);
	return _G[modulename];
end

--获得当前元素包含的脚本空间 table,只对frame标签有效使用
local function GetFrameModule(e) 
	local modulename = GetFrameModuleName(e);
	if modulename == nil  then
		return nil;
	end

	return loaded[modulename];
end

--页面脚本加载模板
local load_module_txt = 'module("%s"); \r\n%s';
local function LoadModuleScript(mn, txt, name)
	if mn == nil or txt == nil then return end;
	
	local txt = string.format(load_module_txt, mn, txt);
	local f, msg = loadstring(txt, name);		
	if f then
		local r, err = pcall(f);
		if not r then
			local msg = err;
			print(debug.traceback(string.format('%s 脚本错误: %s', os.date('%Y%m%d %H:%M:%S'),msg)));
		end
	else
		print('脚本错误:', msg);
	end
end

local event_2_name =
{
  [0] = 'onClick',--    BUTTON_CLICK = 0,              -- click on button
  [1] = 'onPress',--    BUTTON_PRESS = 1,              -- mouse down or key down in button
  [2] = 'onStateChanged',--    BUTTON_STATE_CHANGED = 2,      -- checkbox/radio/slider changed its state/value 
  [3] = 'onChanging',--   EDIT_VALUE_CHANGING = 3,       -- before text change
  [4] = 'onChange', --      EDIT_VALUE_CHANGED = 4,        -- after text change
  [5] = 'onChange', --    SELECT_SELECTION_CHANGED = 5,  -- selection in <select> changed
  [6] = 'onStateChanged',--    SELECT_STATE_CHANGED = 6,      -- node in select expanded/collapsed, heTarget is the node

  [7] = 'onRequest',--    POPUP_REQUEST   = 7,           -- request to show popup just received, 
                                     --     here DOM of popup element can be modifed.
  [8] = 'onRead', --     POPUP_READY     = 8,           -- popup element has been measured and ready to be shown on screen,
                                     --     here you can use functions like ScrollToView.
  [9] = 'onDismissed',--      POPUP_DISMISSED = 9,           -- popup element is closed,
                                     --     here DOM of popup element can be modifed again - e.g. some items can be removed
                                     --     to free memory.

  [0xA] = 'onItemActive', --    MENU_ITEM_ACTIVE = 0xA,        -- menu item activated by mouse hover or by keyboard,
  [0xB] = 'onClick',--    MENU_ITEM_CLICK = 0xB,         -- menu item click, 
                                     --   BEHAVIOR_EVENT_PARAMS structure layout
                                     --   BEHAVIOR_EVENT_PARAMS.cmd - MENU_ITEM_CLICK/MENU_ITEM_ACTIVE   
                                     --   BEHAVIOR_EVENT_PARAMS.heTarget - the menu item, presumably <li> element
                                     --   BEHAVIOR_EVENT_PARAMS.reason - BY_MOUSE_CLICK | BY_KEY_CLICK

  [0xF] = 'onMenuSetup',--     CONTEXT_MENU_SETUP   = 0xF,    -- evt.he is a menu dom element that is about to be shown. You can disable/enable items in it.      
  [0x10]= 'onMenuRequest',--   CONTEXT_MENU_REQUEST = 0x10,   -- "right-click", BEHAVIOR_EVENT_PARAMS::he is current popup menu HELEMENT being processed or NULL.
                                     -- application can provide its own HELEMENT here (if it is NULL) or modify current menu element.
      
  [0x11]= 'onStatusChanged',--    VISIUAL_STATUS_CHANGED = 0x11, -- broadcast notification, sent to all elements of some container being shown or hidden   


      -- "grey" event codes  - notfications from behaviors from this SDK 
  [0x80] = 'onClick',--      HYPERLINK_CLICK = 0x80,        -- hyperlink click
  [0x81] = 'onHeaderClick',--   TABLE_HEADER_CLICK,            -- click on some cell in table header, 
                                     --     target = the cell, 
                                     --     reason = index of the cell (column number, 0..n)
  [0x82] = 'onRowClick',--    TABLE_ROW_CLICK,               -- click on data row in the table, target is the row
                                     --     target = the row, 
                                     --     reason = index of the row (fixed_rows..n)
  [0x83] = 'onRowDblClick',--    TABLE_ROW_DBL_CLICK,           -- mouse dbl click on data row in the table, target is the row
                                     --     target = the row, 
                                     --     reason = index of the row (fixed_rows..n)

  [0x90] = 'onCollapsed',--    ELEMENT_COLLAPSED = 0x90,      -- element was collapsed, so far only behavior:tabs is sending these two to the panels
  [0x91] = 'onExpanded',--    ELEMENT_EXPANDED,              -- element was expanded,

  [0x92] = 'onActivateChild',--    ACTIVATE_CHILD,                -- activate (select) child, 
                                     -- used for example by accesskeys behaviors to send activation request, e.g. tab on behavior:tabs. 

   --   DO_SWITCH_TAB = ACTIVATE_CHILD,-- command to switch tab programmatically, handled by behavior:tabs 
                                     -- use it as HTMLayoutPostEvent(tabsElementOrItsChild, DO_SWITCH_TAB, tabElementToShow, 0);

  [0x93] = 'onInitDataView',--   INIT_DATA_VIEW,                -- request to virtual grid to initialize its view
      
  [0x94] = 'onRowsDataRequest',--  ROWS_DATA_REQUEST,             -- request from virtual grid to data source behavior to fill data in the table
                                     -- parameters passed throug DATA_ROWS_PARAMS structure.

  [0x95] = 'onUIStateChanged',--    UI_STATE_CHANGED,              -- ui state changed, observers shall update their visual states.
                                     -- is sent for example by behavior:richtext when caret position/selection has changed.

  [0x96] = 'onSubmit',--    FORM_SUBMIT,                   -- behavior:form detected submission event. BEHAVIOR_EVENT_PARAMS::data field contains data to be posted.
                                     -- BEHAVIOR_EVENT_PARAMS::data is of type T_MAP in this case key/value pairs of data that is about 
                                     -- to be submitted. You can modify the data or discard submission by returning TRUE from the handler.
  [0x97] = 'onReset',--    FORM_RESET,                    -- behavior:form detected reset event (from button type=reset). BEHAVIOR_EVENT_PARAMS::data field contains data to be reset.
                                     -- BEHAVIOR_EVENT_PARAMS::data is of type T_MAP in this case key/value pairs of data that is about 
                                     -- to be rest. You can modify the data or discard reset by returning TRUE from the handler.
                                     
  [0x98] = 'onDocumentComplete',--    DOCUMENT_COMPLETE,             -- behavior:frame have complete document.

  [0x99] = 'onHistoryPush',--    HISTORY_PUSH,                  -- behavior:history stuff
  [0x9A] = 'onHistoryDrop',--    HISTORY_DROP,                     
  [0x9B] = 'onHistoryPrior',--    HISTORY_PRIOR,
  [0x9C] = 'onHistoryNext',--    HISTORY_NEXT,

  [0x9D] = 'onHistoryChanged',--    HISTORY_STATE_CHANGED,         -- behavior:history notification - history stack has changed

  [0x9E] = 'onClosePopup',--    CLOSE_POPUP,                   -- close popup request,
  [0x9F] = 'onRequestTooltip',--    REQUEST_TOOLTIP,               -- request tooltip, BEHAVIOR_EVENT_PARAMS.he <- is the tooltip element.

  [0xA0] = 'onAnimation',--   ANIMATION         = 0xA0,      -- animation started (reason=1) or ended(reason=0) on the element.


  --  FIRST_APPLICATION_EVENT_CODE = 0x100 
      -- all custom event codes shall be greater
      -- than this number. All codes below this will be used
      -- solely by application - HTMLayout will not intrepret it 
      -- and will do just dispatching.
      -- To send event notifications with  these codes use
      -- HTMLayoutSend/PostEvent API.

  };


--获得自定义消息的ID
local _self_event_id = 0x1000;
local _self_event = {};
function  GetMsgID(event_name) 
	local mid =  _self_event[event_name];
	if mid then
		return mid;
	end
	_self_event_id = _self_event_id + 1;
	mid = _self_event_id; 

	_self_event[event_name] = mid;
	event_2_name[mid] = event_name;

	return mid;
end

function PrintEachElement(e)
	print('**********Element Tree*********');
	local index = 0;
	local r = e;
	while(true) do
		if r then
			print(string.rep('  ', index) .. tostring(r));
		else
			break;
		end
		r = r.parent;
		index = index + 1;
	end
	print('******************');
end

local doc = {}
doc.GetFrameScript = GetFrameModule;

function doc.ReLoad(m, root)
	return function(url)
		url = root:combine_url(url);

		local frame = root:find_nearest_parent('frame');
		if frame then
			DeleteFrameModule(frame);
			ui.element.DeleteFrameElementTree(frame);
			frame:load_html(url);
		else	
			local view  = m['view'];
			local nroot = root:reload(url);
			if nroot  then
				root = GetElementObj(nroot);			
				onHtmlayoutDocumentComplete(view, root);
			end
			collectgarbage();
		end
	end
end
function doc.Wrapper_E(root)	
	return function(selector)
		return root:find_element(selector);
	end
end

function doc.GetParent(d)
	local p = d.root.parent;
	if p == nil then 
		return nil;
	end

	local modulename = GetElementModuleName(p);
	local v = _G[modulename];
	d['parent'] = v;
	return v;
end

function doc.SetValues(root) 
	return function(...)
		return root:set_child_values(...);
	end
end

function doc.GetValues(root)
	return function()
		return root:get_child_values() or {};
	end
end

function doc.NewWindow(view, root) 
	return function(url, ismodal, ...)
		return mfc.NewWindowByParent(root:combine_url(url), ismodal, root:get_root_hwnd(), false, ...);
	end
end

function doc.NewSkinWindow(view, root)
	return function(url, ismodal, ...)
		return mfc.NewWindowByParent(root:combine_url(url), ismodal, root:get_root_hwnd(), true, ...);
	end
end

function _G.GetScriptHandle(hwnd)
	return GetModuleTable(htmlayout.get_hwnd_root_element(hwnd));
end

doc.__index = function(t, key)	
	if key == 'E' then
		return doc.Wrapper_E(t.root);
	elseif key == 'parent' then
		return doc.GetParent(t);
	elseif key == 'ReLoad' then
		return doc.ReLoad(t, t.root);
	elseif key == 'SetValues' then
		return doc.SetValues(t.root);
	elseif key == 'GetValues' then
		return doc.GetValues(t.root);
	elseif key == 'NewWindow' then
		return doc.NewWindow(t.view, t.root);
	elseif key == 'NewSkinWindow' then
		return doc.NewSkinWindow(t.view, t.root);
	end

	local v = doc[key] or  _G[key];
	if v then 
		return v; 
	end;

	local r = t['root'];
	v = r:find_element('#' .. key);
	if v == nil then
		return nil;
	end
	t[key] = v;
	v['_element_name_'] = key;
	return v;
end


--统一处理页面消息的分发
function HandleHtmlayoutEvent(he, target, event_type, reason)
	local event_name = event_2_name[event_type];
	if event_name == nil then return false; end;

	he     = GetElementObj(he);
	target = GetElementObj(target);
	if he == nil then
		return false;
	end

	local modulename = GetElementModuleName(he);
	local m = loaded[modulename]
	if m == nil then
		return false;
	end

	if he[event_name] and type(he[event_name]) == 'function' then
		he[event_name](he, target, event_type, reason);
		return true;
	end

	local func = he.attributes[event_name];
	if func then
		m['this'] = he;
		LoadModuleScript(modulename, func, string.format('%s#%s:%s', he.tag or '', he.id or '', event_name or ''));
		return true;
	end

	if type(he['onEvent']) == 'function' then
		return he['onEvent'](he, event_name, target, event_type, reason);
	end
	return false;
end

--处理数据缓冲
--load_type= 0查找资源
--load_type= 1资源加载成功 
--data_type (0-html 1-image 2-style 3-cursor 4-script)
function HandleDataLoad(load_type, data_type, uri, data) 
	if load_type == 0 then
		return ui.pkg.LoadData(uri);
	end
end
--全局函数，当页面加载完成之后调用
function onHtmlayoutDocumentComplete(view, hroot, ...)
	view = ui.view.CreateView(view);
	hroot = GetElementObj(hroot);
	--print('Begin Load Script', hroot);
	local title  = hroot:find_element('title');
	if title then
		view:set_title(title.text);
	end

	InitScriptModule(view, hroot, ...);

	local t = hroot:find_all("script[type='text/lua']");
	for k, v in ipairs(t) do
		binit = true;
		LoadHtmlayoutScript(v, ...);
	end

	local ft = hroot:find_all('frame');
	for k, v in ipairs(ft) do
		LoadFrameScript(v);
	end

	AutoLoadBehavior(hroot);
end


--判断是否需要做元素行为自动绑定
local function NeedAttachBehavior(h)
	local sb = htmlayout.get_element_style(h, '-prototype')
	local wb = htmlayout.get_element_attribute(h, 'widget_prototype');

	return sb or wb, (sb or '') .. ' ' .. (wb or '');
end
--给元素绑定行为对象
--h 元素句柄
--m 元素所在的模块
--root_html 根元素句柄
function AttachBehavior(h, m, root_html) 
	m = m or GetModuleTable(root_html) or {};	
	local need, b = NeedAttachBehavior(h);
	if need then
		local blist = {};
		for bb in string.gfind(b, '(%S+)') do
			local pro = m[bb] or _G[bb];
			if pro then
				table.insert(blist, 1, pro);	--设置在前的对象优先级高
			end
		end

		if #blist > 0 then
			local e = GetElementObj(h);
			for k, v in ipairs(blist) do
				e.prototype = v;
			end
		end
	end
end

--对于有behavior属性的元素，自动实例化
function AutoLoadBehavior(hroot)
	hroot = GetElementObj(hroot);
	if hroot == nil then return end;

	local all = hroot:base_find_all('*');
	for k, h in ipairs(all) do
		local e = GetElementObj(h, true);
		if e == nil then --只处理没有实例化过的元素
			if NeedAttachBehavior(h) then
				GetElementObj(h);
			end
		end
	end
end

--加载frame中的脚本
function LoadFrameScript(frame, ...) 
	if frame:get_type() ~= 'frame' then
		return;
	end
	local t = frame:find_all("script[type='text/lua']");
	local binit = false;
	local html = frame:find_element('html');	

	if html then
		InitScriptModule(ui.view.CreateView(htmlayout.get_view(html:get_handle())), html,unpack(arg));
	end

	for k, v in ipairs(t) do
		LoadHtmlayoutScript(v);
	end

	if html then				
		AutoLoadBehavior(htmlayout.get_html_element(html:get_handle()));
	end
end

--窗口关闭时的处理
function onWindowClose(hroot)	
	ui.element.DeleteModuleFromRootHandle(hroot);
end


--删除自定义控件的元素
function onDestroyControl(he)
	he = GetElementObj(he, true);
	if he == nil then
		return;
	end

	he:destroy_ctrl();
end


function InitScriptModule(realview, e, ...) 
	e = GetElementObj(e);
	local modulename = GetElementModuleName(e);
	
	local t = _G[modulename];
	if t == nil then
		LoadModuleScript(modulename, '');
		t = _G[modulename]
		t['root']   = e:get_html_element();
		t['arg']    = arg;
		t['self']   = t;
		t['view']   = realview;
		setmetatable(_G[modulename], doc);
	end
end

--加载页面中的脚本
function LoadHtmlayoutScript(e, ...)	
	e = GetElementObj(e);
	local modulename = GetElementModuleName(e);
	
	--加载模块
	local reqlist = e.attributes['require'];
	if reqlist then
		for m in string.gfind(reqlist, '(%S+)') do
			require(m);
		end
	end

	--加载src指定的文件内容
	local url = e:combine_url(e.attributes['src']);	
	local script_data = ui.pkg.LoadData(url);
	if script_data then
		LoadModuleScript(modulename, script_data, e.attributes['src']);
	end

	--加载<script> tag中的脚本内容
	LoadModuleScript(modulename, e.text, '<script>');
end

--创建新窗口
local _Window = {
	__index=function(t, k)
		local hwnd = rawget(t, 'hwnd');
		local hs   = GetScriptHandle(hwnd);
		if hs == nil then return nil; end
		
		return hs[k];
	end;

	__tostring = function(t)
		local hwnd = rawget(t, 'hwnd');
		return string.format('HWND: 0x%X lua table: %s', hwnd, tostring(GetScriptHandle(hwnd)));
	end;

};

function _Window:new (hwnd)
	local o = {		
		isvalid = function(t)
			local hwnd = rawget(t, 'hwnd');
			local hs   = GetScriptHandle(hwnd);
			return hs ~= nil;
		end		
	};
	o.hwnd = hwnd;	

	setmetatable(o, _Window)
	return o
end

--创建带父窗口的新窗口
function mfc.NewWindowByParent(url, ismodal, parent, bskin, ...) 
	if ismodal then
		return mfc.domodaldlg(url, parent, bskin, ...);
	else
		local hwnd =  mfc.showdialog(url, parent, bskin, ...);
		if hwnd == nil  then return nil; end
		return _Window:new(hwnd);
	end
end

--创建新窗口，不带父窗口
function mfc.NewWindow(url, ismodal, ...)	
	return mfc.NewWindowByParent(url, ismodal, 0, false, ...);
end

function mfc.NewSkinWindow(url, ismodal, ...)
	return mfc.NewWindowByParent(url, ismodal, 0, true, ...);
end

