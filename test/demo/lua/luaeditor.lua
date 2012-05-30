require('help');
--lua编辑器
local LE = {};
LuaEditor = LE;


function LE:init()
	--消息响应和注册
	local notify = 
	{
		[2001] = self.onCharAdded;
		[2016] = self.onDWellStart;
	};

	for k, v in pairs(notify) do
		self:register_notify(k, v);
	end

	--屏蔽掉不需要显示的快捷键
	local b = 'BEFGHKNOPQRSW';
	for i = 1, #b do
		self:SendMsg(SCI_ASSIGNCMDKEY or 2070, string.byte(b:sub(i, i)) + 2 * 65536, SCI_NULL or 2172);
	end

	self:set_tip_color(0, 0xE1FFFF, 0x0000FF);
	self:set_autoc_maxheight(15);
	self:set_autoc_maxwidth(64);
	self:set_mouse_dwell_time(300);


	self:syntax_light({
		language = 'lua',
		keywords = {
		'and break do else elseif end false for function if in local nil not or repeat return then true until while self',
		'table.insert table.remove string.sub table.concat'
		};
		color = {
			['default'] = 0x0;
			['keyword'] = 0xFF0000;
			['number']  = 0x0000FF;
			['string']  = 0xFF00FF;
			['character'] = 0xFF00FF;
			['comment'] = 0x008000;
			['commentline'] = 0x008000;
			[13] = 0xFF0000;
			[10] = 0x0000A0;
		};
	});
end

function LE:get_all_word(pos)
	local t = {};
	local curw = nil;
	while(true) do
		local xc, wb, we = self:get_word(pos, 1);
		curw = curw or xc;
		if xc == nil then
			break;
		end
		table.insert(t, 1, xc);
		local sep = self:GetTextRange(wb - 1, wb);
		if sep ~= '.' then
			break;
		end
		table.insert(t, 1, sep);
		pos = wb - 1;
	end
	return table.concat(t, ''), curw;
end


function LE:show_autoc_auto(allword, word, ch) 		
	local path = nil;
	if ch == '.' or string.find(allword, '%.') then
		path = allword;
		if ch ~= '.' then
			local _, _, p = string.find(allword, '^(.+)%.([^%.]+)$');
			path = p;
		end
	else
		path = '_root_';
		if word == nil or #word < 3 then
			return false;
		end
	end	
	if path == nil  then
		return false;
	end
	local text = help.GetAutoCList(path);
	if text == nil then
		return false;
	end
	
	self:autocshow(#word, text);
	return self:isautocshow();
end

--分离函数,返回一个table
function LE:sep_function(fstr) 
	local lastfunc = nil;
	local t = {};
	if fstr == nil then
		return t;
	end
	local rightend = 0;
	while(true) do
		local b, e, left, func, right = string.find(fstr, '^(.-)([%w:][%w.0-9_]*)%s*%(([^(]*)$');
		if func == nil then
			break;
		end

		local r, n = string.gsub(right, '%)', '');
		if n <= 0 and rightend <= 0 then
			lastfunc = lastfunc or func;
			table.insert(t, func);
			rightend = rightend - 1;
		else
			rightend = rightend + (n - 1);
		end
		fstr = left;
	end
	return t, lastfunc;
end

--显示上下文帮助
function LE:onShowContextHelp(t)
end


local function find_cur_string_param(func)
	func = string.gsub(func, '"[^"]*"', 'x');
	func = string.gsub(func, "'[^']*'", 'x');
	local n = 0;
	while(true) do
		func, n = string.gsub(func, '%([^%(%)]*%)', '');
		if n == nil or n <= 0 then
			break;
		end
	end

	local b, e, f = string.find(func, '%(([^%(]*)$');
	if f then
		local  _, n =  string.gsub(f, ',', ',');
		return n + 1;
	end
	return 0;
end

--查找函数定义func中第n个参数的位置
local function find_param_pos(func, n)
	local len = #func;
	local b = string.find(func, '%(.+%)')
	if b == nil then
		return nil;
	end
	local nindex = 1;
	local pbegin = nil;
	local pend = nil;
	for i = b+1, len do
		local c = string.sub(func, i, i);
		if c == ',' or c == ')' then
			nindex  = nindex + 1;
		elseif nindex == n then
			if pbegin == nil then
				pbegin = i;
			end 
		end

		if pbegin  and nindex > n then
			pend = i - 1;
			break;
		end


		if c == ')' then
			break; 
		end
	end

	if pbegin and pend  then
		local s = string.sub(func, pbegin, pend);
		local b, e = string.find(s, '([^%[%]]+)');
		if b  then
			return b + pbegin - 1, e + pbegin - 1;
		end
	end
	return pbegin, pend;
end

function LE:show_context_help(pos, allword, word)
	local line = self:get_line_from_pos(pos);
	local begin= self:get_pos_from_line(line);
	local linestr = self:GetTextRange(begin, pos);

	local f_list, lastfunc = self:sep_function(linestr);
	if word  and word ~= f_list[1] then
		table.insert(f_list, 1, word);
	end

	--print('lastfunc=', lastfunc, linestr);
	local t = help.GetHelp(f_list);
	self:onShowContextHelp(t);
	if lastfunc then
		local tip = help.GetHelp({lastfunc});
		if tip and tip[1] and tip[1].def then			
			self:show_tip(pos, tip[1].def .. '\n' .. (tip[1].help or ''));
			local curpos = find_cur_string_param(linestr);
			local show_hl = false;
			if curpos > 0 then
				local b, e = find_param_pos(tip[1].def, curpos);
				if b and e then
					self:highlight_tip(b - 1, e);
					show_hl = true;
				end
			end

			if not show_hl then
				self:highlight_tip(-1, -1);
			end
		else
			self:hide_tip();	
		end
	else
		self:hide_tip();
	end
end

--相应SCN_CHARADDED 2001消息
function LE:onCharAdded(msg)
	local ch = string.char(msg.ch);
	local pos = self:get_current_pos();
	local allword, word;
	if ch == '.' then	--查找全局函数 
		allword, word = self:get_all_word(pos - 1, 1);		
		self:show_autoc_auto(allword, '', ch);
		return;
	elseif ch == ':' then
		word = self:get_word(pos - 1, 1);
	elseif ch == '(' then
		allword, word = self:get_all_word(pos, 1);
	else
		allword, word = self:get_all_word(pos, 1);
		if word and #word >= 1 then
			if self:show_autoc_auto(allword, word, ch) then				
				return;
			end
		end
	end
	self:show_context_help(pos, allword, word);
end

function LE:onKey(msg)
	print(os.date(), string.rep('*', 24));
	table.foreach(msg, print);
	
end
function LE:onDWellStart(msg)
	if msg.pos > 0 then		
		self:show_tip(msg.pos, 'table.insert(table[,pos ,]value)');
	end
end

