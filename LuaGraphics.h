#ifndef _LUA_GRAPHICS_H
#define _LUA_GRAPHICS_H

//导出graphics对象
#include <htmlayout_graphin.h>
#include <Luna.h>

#define OFFSET_LINE_WIDTH 0.5

#define PARAM_FLOAT_1 PAD(L, 1)+OFFSET_LINE_WIDTH
#define PARAM_FLOAT_2 PARAM_FLOAT_1, PAD(L, 2)+OFFSET_LINE_WIDTH
#define PARAM_FLOAT_3 PARAM_FLOAT_2, PAD(L, 3)+OFFSET_LINE_WIDTH
#define PARAM_FLOAT_4 PARAM_FLOAT_3, PAD(L, 4)+OFFSET_LINE_WIDTH
#define PARAM_FLOAT_5 PARAM_FLOAT_4, PAD(L, 5)+OFFSET_LINE_WIDTH
#define PARAM_FLOAT_6 PARAM_FLOAT_5, PAD(L, 6)+OFFSET_LINE_WIDTH
#define PARAM_FLOAT_7 PARAM_FLOAT_6, PAD(L, 7)+OFFSET_LINE_WIDTH
#define PARAM_FLOAT_8 PARAM_FLOAT_7, PAD(L, 8)+OFFSET_LINE_WIDTH

class CLuaGraphics
{
public:
	static const char className[];					//类名
	static Lunar<CLuaGraphics>::RegType methods[];	//需要导出的函数

	typedef  htmlayout::graphics baseclass;
	CLuaGraphics(lua_State *L);	
	~CLuaGraphics();
protected:
	CLuaGraphics():m_pSurface(NULL), m_pGraphics(NULL)
	{}

	htmlayout::image*		m_pSurface;	//绘图表面
	htmlayout::graphics*    m_pGraphics;//绘图上下文

protected:
	htmlayout::graphics*   gApi()
	{
		assert(m_pSurface);
		assert(m_pGraphics);
		return m_pGraphics;
	}

	void empty()
	{
		if(m_pGraphics)
			delete m_pGraphics;
		if(m_pSurface)
			delete m_pSurface;

		m_pGraphics = NULL;
		m_pSurface  = NULL;
	}
public:
	int get_width(lua_State* L)
	{
		if(m_pSurface == NULL)
			return 0;
		lua_pushnumber(L, m_pSurface->width());
		return 1;
	}

	int get_height(lua_State* L)
	{
		if(m_pSurface == NULL)
			return 0;
		lua_pushnumber(L, m_pSurface->height());
		return 1;
	}
	//返回surface句柄
	//h = g:get_handle();
	int get_handle(lua_State* L);

	//
	int lineWidth(lua_State* L)
	{
		gApi()->line_width(PAD(L, 1));
		return 0;
	}

	int lineCap(lua_State* L)
	{
		gApi()->line_cap((LINE_CAP_TYPE)PAI(L, 1));
		return 0;
	}

	int lineJoin(lua_State* L)
	{
		gApi()->line_join((LINE_JOIN_TYPE)PAI(L, 1));
		return 0;
	}

	int lineLinearGradient(lua_State* L)
	{
		//gApi()->line_linear_gradient()
		return 0;
	}
	int lineColor(lua_State* L)
	{
		aux::a2w a(PAS(L, 1));
		gApi()->line_color(htmlayout::color::parse(aux::wchars(a, a.length())));
		return 0;
	}

	int fillColor(lua_State* L)
	{
		aux::a2w a(PAS(L, 1));
		gApi()->fill_color(htmlayout::color::parse(aux::wchars(a, a.length())));
		return 0;
	}
	int no_line(lua_State* L);

	int moveTo(lua_State* L);
	
	//g:line_to(x, y, isrelative);
	int lineTo(lua_State* L);

	//g:line(x1:float, y1:float, x2:float, y2:float)
	int line(lua_State* L);

	int clear(lua_State* L);

	//将当前内容贴到DC上
	//g:blit(hdc, x, y)
	int blit(lua_State* L);

	///三角形
	///g:triangle(x1:float, y1:float, x2:float, y2:float, x3:float, y3:float)
	int triangle(lua_State* L);

	//画矩形
	//g:rectangle( x:float, y:float, w:float, h:float ) 
	//         or( x:float, y:float, w:float, h:float, r:float ) 
	//         or( x:float, y:float, w:float, h:float, rx:float, ry:float ) 
	//         or( x:float, y:float, w:float, h:float, rx_top:float, ry_top:float, rx_bottom:float, ry_bottom:float )
	int rectangle(lua_State* L);

	//画椭圆形
	//g:ellipse( x1:float, y1:float, r:float ) : 
	//         or( x1:float, y1:float, rx:float, ry:float )
	int ellipse(lua_State* L);

	//画弧形
	//g:arc( x1:float, y1:float, rx:float, ry:float, startAngle:float, endAngle:float)
	int arc(lua_State* L);


	int worldToScreen(lua_State* L)
	{
		DIM x = PAD(L, 1);
		DIM y = PAD(L, 2);
		if(lua_isnoneornil(L, 2))
		{
			gApi()->world_to_screen(x);
			lua_pushnumber(L, x);
			return 1;
		}

		gApi()->world_to_screen(x, y);
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		return 2;
	}

	int scale(lua_State* L)
	{
		gApi()->scale(PAD(L, 1), PAD(L, 2));
		return 0;
	}


	int translate(lua_State* L)
	{
		gApi()->translate(PAD(L, 1), PAD(L, 2));
		return 0;
	}

	int beginPath(lua_State* L)
	{
		gApi()->open_path();
		return 0;
	}

	int closePath(lua_State* L)
	{
		gApi()->close_path();
		return 0;
	}

	int drawPath(lua_State* L)
	{
		gApi()->draw_path((DRAW_PATH_MODE)PAI(L, 1));
		return 0;
	}

	int loadImageFromBytes(lua_State* L)
	{
		empty();
		m_pSurface = htmlayout::image::load(aux::bytes((byte *)PAS(L, 1), PASL(L, 1)));
		//if(m_pSurface)
		//	m_pGraphics = new htmlayout::graphics(m_pSurface);
		lua_pushboolean(L, m_pSurface != NULL);
		return 1;
	}

	int drawImage(lua_State* L)
	{
		htmlayout::image* pImage = (htmlayout::image*)PAI(L, 1);
		int nParams = lua_gettop(L);
		lua_remove(L, 1);
		if(nParams == 3)
			gApi()->draw_image(pImage, PARAM_FLOAT_2);
		else if(nParams == 5)
			gApi()->draw_image(pImage, PARAM_FLOAT_4,0,0,pImage->width(), pImage->height());
		else if(nParams == 9)
			gApi()->draw_image(pImage, PARAM_FLOAT_8);
		return 0;
	}

	int blitImage(lua_State* L)
	{
		htmlayout::image* pImage = (htmlayout::image*)PAI(L, 1);
		int nParams = lua_gettop(L);
		lua_remove(L, 1);
		if(nParams == 3)
			gApi()->blit_image(pImage, PAI(L, 2), PAI(L, 3));
		else if(nParams == 7)
			gApi()->blit_image(pImage, PARAM_FLOAT_2, PAI(L, 3), PAI(L, 4), PAI(L, 5), PAI(L, 6));
		return 0;
	}

	int blendImage(lua_State* L)
	{
		htmlayout::image* pImage = (htmlayout::image*)PAI(L, 1);
		int nParams = lua_gettop(L);
		lua_remove(L, 1);
		if(nParams == 4)
			gApi()->blend_image(pImage, PAD(L, 1), PAD(L, 2), PAI(L, 3));
		else if(nParams == 8)
		{
			lua_insert(L, 3);
			gApi()->blend_image(pImage, PARAM_FLOAT_2, PAI(L, 3), PAI(L, 4), PAI(L, 5), PAI(L, 6), PAI(L, 7));
		}

		return 0;
	}

	int noFill(lua_State* L)
	{
		gApi()->no_fill();
		return 0;
	}

	int noLine(lua_State* L)
	{
		gApi()->no_line();
		return 0;
	}

	int text(lua_State* L)
	{
		aux::a2w a(PAS(L, 3));
		gApi()->text(PAD(L, 1), PAD(L, 2), aux::wchars(a, a.length()));
		return 0;
	}
	
	int font(lua_State* L)
	{
		gApi()->font(PAS(L, 1), PAD(L, 2), lua_toboolean(L, 3), lua_toboolean(L, 4), PAD(L, 5));
		return 0;
	}

	int textAlignment(lua_State* L)
	{
		gApi()->text_alignment((TEXT_ALIGNMENT)PAI(L, 1), (TEXT_ALIGNMENT)PAI(L, 2));
		return 0;
	}

	int textWidth(lua_State* L)
	{
		aux::a2w a(PAS(L, 1));
		lua_pushnumber(L, gApi()->text_width(aux::wchars(a, a.length())));
		return 1;
	}

	int save(lua_State* L)
	{
		gApi()->state_save();
		return 0;
	}

	int restore(lua_State* L)
	{
		gApi()->state_restore();
		return 0;
	}
	int skew(lua_State* L)
	{
		gApi()->skew(PAD(L, 1), PAD(L, 2));
		return 0;
	}

	int rotate(lua_State* L)
	{
		int nParams  = lua_gettop(L);
		if(nParams  == 1)
			gApi()->rotate(PAD(L, 1));
		else if(nParams == 3)
			gApi()->rotate(PAD(L, 1), PAD(L, 2), PAD(L, 3));
		return 0;
	}

	int star(lua_State* L)
	{
		gApi()->star(PARAM_FLOAT_4, PAD(L, 5), PAI(L, 6));
		return 0;
	}


	POS*  GetPos(lua_State* L, int idx, int& nNum)
	{
		if(!lua_istable(L, idx))
			return NULL;

		int nLen = lua_objlen(L, idx);
		POS* pXY = new POS[2 * nLen];

		nNum = 0;
		int nIndex = 0;
		lua_pushnil(L);  /* first key */
		while (lua_next(L, idx) != 0) {   
			lua_rawgeti(L, -1, 1);
			pXY[nIndex ++] = PAD(L, -1) + OFFSET_LINE_WIDTH;
			lua_pop(L, 1);

			lua_rawgeti(L, -1, 2);
			pXY[nIndex ++] = PAD(L, -1) + OFFSET_LINE_WIDTH;
			lua_pop(L, 1);

			++ nNum;
			lua_pop(L, 1);  /* removes `value'; keeps `key' for next iteration */
			if(nIndex >= 2 * nLen)
				break;
		}
		lua_pop(L, 1);
		return pXY;
	}

	int polygon(lua_State* L)
	{
		int nNum = 0;
		POS* pXY = GetPos(L, 1, nNum);
		if(pXY == NULL)
			return 0;

		gApi()->polygon(pXY, nNum);
		delete[] pXY;
		return 0;
	}


	int polyline(lua_State* L)
	{
		int nNum = 0;
		POS* pXY = GetPos(L, 1, nNum);
		if(pXY == NULL)
			return 0;

		gApi()->polyline(pXY, nNum);
		delete[] pXY;
		return 0;
	}

	int bezierCurveTo(lua_State* L)
	{
		gApi()->bezier_curve_to(PARAM_FLOAT_6, PAI(L, 7));
		return 0;
	}

	int hlineTo(lua_State* L)
	{
		gApi()->hline_to(PARAM_FLOAT_1, lua_toboolean(L, 2));
		return 0;
	}

	int vlineTo(lua_State* L)
	{
		gApi()->vline_to(PARAM_FLOAT_1, lua_toboolean(L, 2));
		return 0;
	}

	int arcTo(lua_State* L)
	{
		gApi()->arc_to(PARAM_FLOAT_2, PAD(L, 3), PAD(L, 4) + OFFSET_LINE_WIDTH, PAD(L, 5)+ OFFSET_LINE_WIDTH, lua_toboolean(L, 6), lua_toboolean(L, 7), lua_toboolean(L, 8));
		return 0;
	}

	int ellipseTo(lua_State* L)
	{
		gApi()->ellipse_to(PARAM_FLOAT_4, lua_toboolean(L, 5));
		return 0;
	}

	int quadraticCurveTo(lua_State* L)
	{
		gApi()->quadratic_curve_to(PARAM_FLOAT_4, lua_toboolean(L, 5));
		return 0;
	}
};
#endif