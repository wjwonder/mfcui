require('mfc');

mfc.register('exctrl.dll');--加载扩展控件
mfc.init();	--初始化

local fn = arg[1] or  string.format('file://%s/demo.htm', mfc.workdir());
local w = mfc.NewWindow(fn, true);	--启动模态窗口

