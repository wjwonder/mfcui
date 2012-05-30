--界面sys模块
module('ui.sys', package.seeall);

--全局错误处理函数
function _G.errormsg(msg)
	print(debug.traceback(string.format('%s 脚本错误: %s', os.date('%Y%m%d %H:%M:%S'),msg)));
end

