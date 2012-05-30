--允许从pkg包中加载页面和资源
module('ui.pkg', package.seeall)

local zip = require('zip');

local open_pkg = zip.open;
--分离URL
local function parse_url(url) 
	local _, _, protocol, pkg, path, param = string.find(url, '([%w]+)://([^/]+)/([^?]+)(.*)');
	protocol = string.lower(protocol or '');
	return protocol, pkg, path, param;
end


--从pkg包中获得文件内容
local function get_file_from_pkg(pkg, path) 
	local p = nil;
	local data  = nil;
	while(1) do
		p = open_pkg(mfc.workdir() .. '\\' .. pkg);
		if p == nil then break;	end;
		
		local f = p:open(path);
		if f == nil then break; end;

		data = f:read('*a');
		f:close();
		break;
	end
	if p  then p:close(); p=nil; end

	return data;
end

function LoadData(url)
	if string.find(url, '%%') then
		url = htmlayout.url_unescape(url);
	end

	local protocol, pkg, path = parse_url(url);
	if pkg == nil or path == nil then return end;


	if protocol == 'ftp' then	--读取pkg中的文件内容
		return get_file_from_pkg(pkg, path);

	elseif protocol == 'file' then	--读取本地文件
		local f = io.open(pkg ..'/' .. path, 'rb');
		if f  then
			local data = f:read('*a');
			f:close();
			return data;
		end
	end
	
end