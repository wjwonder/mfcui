--tab页
TabStrip = {};

function TabStrip:init()	
    self.html = [[<widget.tabs ></widget>
      <!--<widget.ctl.off-strip visibled title='活动文件'>7<menu.popup /></widget>-->
	  <widget.ctl.off-strip title='活动文件' style="display:none;">7<menu.popup /></widget>
      <widget.ctl.close disabled title='关闭当前页'>r</widget>
    ]];
end

--msg
--切换tab时的消息
function TabStrip:onTabSwitch(new, old)
end

--删除tab时的消息
function TabStrip:onTabDelete(tab) 
end
--

function TabStrip:on_size()
    local offstripButton = self:select("widget.off-strip");  
    local offstripList = self:select("widget.off-strip menu");
    local tabs = self:select("widget.tabs");

    --将tab移到下拉框中
    function moveToOffstrip(tab)
      if offstripList.childcount == 0 then
        --offstripButton.attributes['disabled'] = nil;
		offstripButton.style['display'] = 'block';
       end
	  local h = offstripList:insert(tab:clone(), 0);
	  h.tab = tab;
          tab.style['visibility'] = 'hidden';
    end

	--将tab从下拉表中移除
	function moveFromOffstrip(tab)
		for i = 0,  offstripList.childcount - 1 do
			local offtab = offstripList[i];		
			if offtab.tab == tab then			
				offtab:destroy();
				break;
			end
		end
		if offstripList.childcount == 0 then			
			--offstripButton.attributes['disabled'] = '1';
			offstripButton.style['display'] = 'none';
		end
	end

	local  tabs_width = tabs.x + tabs.width;
	for i = 0, tabs.childcount - 1 do
		local tab = tabs[i];
		local  right_edge = tab.x + tab.width;		
		if right_edge > tabs_width and tab.style['visibility'] == 'visible' then
			moveToOffstrip(tab);			
		elseif right_edge <= tabs_width and tab.style['visibility'] == 'hidden' then			
			tab.style['visibility'] = 'visible';
			moveFromOffstrip(tab);
		end
	end
	self:update();
end


function TabStrip:addTab(label, filename)
	local  tabs = self:select('widget.tabs');
	local  tab =  tabs:find_element(string.format("option[value2='%s']", filename));
	local  created = false;
	if tab == nil then 
		tab = tabs:insert(self:create_element('option', label), 0);
		tab.attributes['value2'] = filename;
		tab.attributes['title'] = filename;
		created = true;
	end

	self:switchTab(tab);
	self:on_size();
	return tab, created; 
end


function TabStrip:switchTab(tab)
    local  prevCurrent = self:select("widget.tabs>option:current");
    if prevCurrent then
      if  prevCurrent == tab  then return; end
      prevCurrent.state[':current'] = false;
    end

	if tab == nil  then
		self:select("widget.ctl.close").attributes['disabled'] = true;
		return;
	end
    tab.state[':current'] = true;
    self:select("widget.ctl.close").attributes['disabled'] = nil;
	self:onTabSwitch(tab, prevCurrent); -- notify observers.
end

function TabStrip:setCurrent(fn)
	local  tabs = self:select('widget.tabs');
	local  tab =  tabs:find_element(string.format("option[value2='%s']", fn));
	if tab == nil  then return end;
	self:switchTab(tab);
end
function TabStrip:closeTab(tab)
	local tabs = self:select("widget.tabs");
	local delindex = -1;
	for i = 0, tabs.childcount - 1 do
		if tab == tabs[i] then
			self:onTabDelete(tab);
			delindex = i;
			tab:destroy();
			break;
		end
    end

	tabs:update();
	if tabs.childcount > 0 then		
		delindex = math.min(tabs.childcount - 1, delindex);
		self:switchTab(tabs[delindex]);
		self:on_size();	
	else
		self:switchTab(nil);
		self:update();
	end
end

function TabStrip:changeLable(label)
	local  tab = self:select("widget.tabs>option:current");
	if tab == nil then return end;
	tab.html = label;
	tab.attributes['title'] = label;
end

function TabStrip:on_controlevent(evt)
	if evt.type == Event.BUTTON_PRESS and evt.target.tag == "option" then
		self:switchTab(evt.target);  
		return true; 
	end

	if  evt.type == Event.MENU_ITEM_CLICK then
		local menuItem = evt.target;
		local tab      = menuItem.tab;
		
		tab.style['visibility'] = 'visible';
		tab.parent:insert(tab,0); -- move it as a first one		
		tab.parent:update();
		menuItem:destroy(); 
		self:on_size();
		self:switchTab(tab);
		return true;
	end

	if evt.type == Event.BUTTON_CLICK and evt.target == self:select("widget.close") then			
		local ctab = self:select("widget.tabs>option:current");
		self:closeTab( ctab ); 
		return true;
	end
    return false;
end

