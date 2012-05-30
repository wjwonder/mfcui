--scintilla控件功能扩展
--封装scintilla控件功能

local ex = {};
luaex_scintilla	= ex;	--全局访问table

--初始化
function ex:init()
	self:SendMsg(2037, 936);
	self:set_font('宋体', 11);
	self:set_tab_width(4);
--	self:show_line_number(1, 4);
	
end
--设置文本
--txt 需要设置的文本内容
--return 无
function ex:set_text(txt) 
	self:SendMsg(SCI_SETTEXT or 2181,  0, txt or '');
end

--插入文本
--pos 需要插入的位置 -1是当前光标处
--txt 需要插入的文本
--return 无
function ex:insert_text(pos, txt)
	self:SendMsg(SCI_INSERTTEXT or 2003, pos, txt);
end

--显示提示信息
function ex:show_tip(pos, tip)
	self:SendMsg(SCI_CALLTIPSHOW or 2200, pos, tip);
end

--高亮提示
function ex:highlight_tip(hStart, hEnd) 
	self:SendMsg(SCI_CALLTIPSETHLT or 2204, hStart, hEnd);
end

function ex:hide_tip()
	self:SendMsg(SCI_CALLTIPCANCEL or 2201);
end
--设置提示前景色
--back 背景色 ，fore 前景色 highlight高亮色
function ex:set_tip_color(fore, back, highlight)
	if back then
		self:SendMsg(SCI_CALLTIPSETBACK or 2205, back);
	end
	if fore then
		self:SendMsg(SCI_CALLTIPSETFORE or 2206, fore);
	end
	if highlight then
		self:SendMsg(SCI_CALLTIPSETFOREHLT or 2207, highlight);
	end

end

--设置

--显示自动完成选项
--entered 已经输入的字符数
--list 自动完成选项，多个选项之间用空格分隔
function ex:autocshow(entered, list) 
	self:SendMsg(SCI_AUTOCSHOW or 2100, entered or 0, list);
end

function ex:isautocshow()
	return self:SendMsg(SCI_AUTOCACTIVE or 2102) ~= 0;
end
--设置字体
--fontname 字体名称
--stylenumber 特定类型字体，不指定为默认字体
function ex:set_font(fontname, size, bold, italic,  stylenumber) 
	stylenumber = stylenumber or (STYLE_DEFAULT or 32);
	if fontname then
		self:SendMsg(SCI_STYLESETFONT or 2056, stylenumber, fontname);
	end

	if size then
		self:SendMsg(SCI_STYLESETSIZE or 2055, stylenumber, size);
	end

	if bold then bold = 1; else bold = 0; end
	self:SendMsg(SCI_STYLESETBOLD or 2053, stylenumber, bold);

	if italic then italic = 1; else italic = 0; end
	self:SendMsg(SCI_STYLESETITALIC or 2054, stylenumber, italic);
end

--语法加亮
--set 语法加亮设置
--内容
--[[
{	
	language = 'cpp' or 'lua';	--语言类型
	keywords = {'if then', ''} or string;		--语言关键字,支持多组关键字

	color = {
		[key] = color;
	};
}
]]
local COLOR_CHG = 
{
	['default'] = 0;	--默认
	['comment'] = 1;	--注释
	['commentline'] = 2;	--行注释
	['commentdoc']  = 3;
	['number']  = 4;	--数字
	['keyword'] = 5;	--关键字
	['string']  = 6;	--字符串
	['character'] = 7;	--字符
	['operator']  = 10;	--操作符
}
function ex:syntax_light(set)
	self:SendMsg(SCI_SETLEXERLANGUAGE or 4006, 0, set.language);
	if type(set.keywords) == 'table' then
		for k, v in ipairs(set.keywords) do
			self:SendMsg(SCI_SETKEYWORDS or 4005, k - 1, v);	
		end		
	else
		self:SendMsg(SCI_SETKEYWORDS or 4005, 0, set.keywords);	
	end

	--设置颜色
	for k, v in pairs(set.color or {}) do
		local kn = COLOR_CHG[k] or tonumber(k);
		if kn  then
			self:SendMsg(SCI_STYLESETFORE or 2051, kn, v);
		end
	end
end

--设置tab宽度
function ex:set_tab_width(width)
	self:SendMsg(SCI_SETTABWIDTH or 2036 ,width);
end

--显示行号
--margin 序号0-4，决定行号显示的顺序
--width  行号所在的宽度，数字代表行号的字节数
function ex:show_line_number(margin, width)
	self:SendMsg(SCI_SETMARGINTYPEN or 2240, margin or 0,  SC_MARGIN_NUMBER or 1);
	self:SendMsg(SCI_SETMARGINMASKN or 2244, margin or 0, bit.rol(1, margin));
	local pixwidth = self:SendMsg(SCI_TEXTWIDTH or 2276, STYLE_LINENUMBER or 33, '_' .. string.rep('9', width));
	self:SendMsg(SCI_SETMARGINWIDTHN or 2242, margin or 0, pixwidth);
end

--设置margin中显示标记
--margin 序号
--width  宽度
--iconidex 图标样式
--canclick 是否能响应点击消息 0-不能 1-能
function ex:set_margin_symbol(margin, width, iconindex, canclick)
	self:SendMsg(SCI_SETMARGINTYPEN  or 2240,  margin, SC_MARGIN_SYMBOL or 0);	
	self:SendMsg(SCI_SETMARGINWIDTHN or 2242, margin, width);
	self:SendMsg(SCI_MARKERDEFINE or 2040, margin, iconindex);
	self:SendMsg(SCI_SETMARGINMASKN or 2244, margin, bit.rol(1, margin));
	self:SendMsg(SCI_SETMARGINSENSITIVEN or 2246, margin, canclick);
end

--设置margin鼠标形状
--cursor 取值 2-arrow 4-wait 7-反向箭头
function ex:set_margin_cursor(margin, cursor)
	self:SendMsg(SCI_SETMARGINCURSORN or 2248, margin, cursor); 
end

--用xpm显示marker
function ex:set_marker_xpm(markernumber, xpmdata)
	self:SendMsg(SCI_MARKERDEFINEPIXMAP or 2049, markernumber, xpmdata);
end
--设置标记显示颜色
function ex:margin_color(markernumber, forecolor, backcolor)
	if forecolor then
		self:SendMsg(SCI_MARKERSETFORE or 2041, markernumber, forecolor);
	end

	if backcolor then
		self:SendMsg(SCI_MARKERSETBACK or 2042, markernumber, backcolor);
	end
end

--设置第line行标记位
function ex:marker_add(line, markernumber)
	self:SendMsg(SCI_MARKERADD or 2043, line, markernumber);
end

--删除第line行标记位
function ex:marker_delete(line, markernumber)
	self:SendMsg(SCI_MARKERDELETE or 2044, line, markernumber);
end

--判断某一行line指定的标志markernumber是否被选中
function ex:marker_ischecked(line, markernumber)
	local value = self:SendMsg(SCI_MARKERGET or 2046, line);
	local mask  = bit.rol(1, markernumber);
	return bit.band(value, mask) > 0;
end

--改变状态，状态转换
function ex:marker_change(line, markernumber)	
	if self:marker_ischecked(line, markernumber) then
		self:marker_delete(line, markernumber);
		return false;
	else
		self:marker_add(line, markernumber);
		return true;
	end
end

--清楚所有指定的标志位
function ex:marker_clearall(markernumber)
	self:SendMsg(SCI_MARKERDELETEALL or 2045, markernumber);
end

--获得在markernumber上指定标志的所有行，返回一个table
function ex:marker_getall(markernumber)
	local t = {};
	local mask = bit.rol(1, markernumber);
	local bg = 0;
	while(true) do
		local line = self:SendMsg(SCI_MARKERNEXT or 2047, bg, mask);
		if line < 0 then
			break;
		end
		table.insert(t, line);
		bg = line + 1;
	end
	return t;
end

--获得pos位置所在的单词
function ex:get_word(pos, bWordCharacters)
	local wb = math.min(pos, self:SendMsg(SCI_WORDSTARTPOSITION or 2266, pos, bWordCharacters));
	local we = math.min(pos, self:SendMsg(SCI_WORDENDPOSITION or 2267,   pos, bWordCharacters));

	--print('get_word', pos, wb, we);
	return self:GetTextRange(wb, we), wb, we;
end

--获得当前位置
function ex:get_current_pos() 
	return self:SendMsg(SCI_GETCURRENTPOS or 2008);
end

--设置自动完成最大行数
function ex:set_autoc_maxheight(m)
	return self:SendMsg(SCI_AUTOCSETMAXHEIGHT or 2210, m);
end

function ex:set_autoc_maxwidth(m)
	return self:SendMsg(SCI_AUTOCSETMAXWIDTH or 2208, m);
end

--根据pos获得行
function ex:get_line_from_pos(pos)
	return self:SendMsg(SCI_LINEFROMPOSITION or 2166, pos);
end

--根据行获得行情的起始位置
function ex:get_pos_from_line(line) 
	return self:SendMsg(SCI_POSITIONFROMLINE or 2167, line);
end

--获得汗结束位置
function ex:get_endpos_from_line(line)
	return self:SendMsg(SCI_GETLINEENDPOSITION or 2136, line);
end
--获得当前选中的文本
function ex:get_sel_text()
	local b = self:SendMsg(SCI_GETSELECTIONSTART or 2143);
	local e = self:SendMsg(SCI_GETSELECTIONEND or 2145);

	return self:GetTextRange(b, e);
end

--设置鼠标提示时间
function ex:set_mouse_dwell_time(millisec)
	self:SendMsg(SCI_SETMOUSEDWELLTIME or 2264, millisec);
end

--跳到指定航
function ex:goto_line(line)
	self:SendMsg(SCI_GOTOLINE or 2024, line);
end

--选中文本
function ex:select_text(posstart, posend) 
	self:SendMsg(SCI_SETSELECTIONSTART or 2142, posstart);
	self:SendMsg(SCI_SETSELECTIONEND or 2144, posend);
end

--选中某行
function ex:select_line(line)
	local linestart = self:get_pos_from_line(line);
	local lineend   = self:get_endpos_from_line(line);

	self:select_text(linestart, lineend);
end

--设置可见的第一行
function ex:set_first_visible_line(line) 
	self:SendMsg(SCI_SETFIRSTVISIBLELINE or 2613, line);
end

--获得屏幕上的显示的行数
function ex:get_screen_lines()
	return self:SendMsg(SCI_LINESONSCREEN or 2370);
end

--让第line居中显示
function ex:set_center_line(line) 
	local n = self:get_screen_lines();
	local fn = line - math.floor(n/2);
	self:set_first_visible_line(fn);
end

--注册通知消息
--code 通知消息类型
--回调函数
function ex:register_notify(code, func)
	if code == nil or type(func) ~= 'function' then
		return;
	end

	self.msg_map = self.msg_map or {};
	self.msg_map[code] = func;
	self:RegisterNotify(code);
end

--消息通知
function ex:onNotify(msg)
	local code = msg.code;
	local fn   = self.msg_map[code];
	if fn then
		fn(self, msg);
	end
end
