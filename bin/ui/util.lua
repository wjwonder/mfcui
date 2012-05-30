module('ui.util', package.seeall);

--将参数转换成table列表
--如果参数是table，那么直接返回，否则将
--参数放入table的第一个数据中
function get_param_list(param)
	local t = nil;
	if type(param) == 'table' then
		t = param;
	else
		t = {};
		table.insert(t, param);
	end
	return t;
end
