#include "LuaGraphics.h"

const char CLuaGraphics::className[] = "LuaGraphics";
Lunar<CLuaGraphics>::RegType CLuaGraphics::methods[] =
{
	method(CLuaGraphics, get_width),
	method(CLuaGraphics, get_height),
	method(CLuaGraphics, no_line),	
	method(CLuaGraphics, lineTo),	
	method(CLuaGraphics, moveTo),	
	method(CLuaGraphics, line),	
	method(CLuaGraphics, lineCap),
	method(CLuaGraphics, lineWidth),	
	method(CLuaGraphics, get_handle),	
	method(CLuaGraphics, blit),	
	method(CLuaGraphics, clear),	
	method(CLuaGraphics, triangle),	
	method(CLuaGraphics, rectangle),	
	method(CLuaGraphics, ellipse),	
	method(CLuaGraphics, arc),	
	method(CLuaGraphics, lineColor),	
	method(CLuaGraphics, fillColor),	
	method(CLuaGraphics, worldToScreen),	
	method(CLuaGraphics, scale),	
	method(CLuaGraphics, rotate),	
	method(CLuaGraphics, translate),	
	method(CLuaGraphics, beginPath),
	method(CLuaGraphics, closePath),
	method(CLuaGraphics, drawPath),
	method(CLuaGraphics, lineJoin),
	method(CLuaGraphics, loadImageFromBytes),
	method(CLuaGraphics, drawImage),
	method(CLuaGraphics, blitImage),
	method(CLuaGraphics, blendImage),
	method(CLuaGraphics, noLine),
	method(CLuaGraphics, noFill),
	method(CLuaGraphics, font),
	method(CLuaGraphics, text),
	method(CLuaGraphics, textAlignment),
	method(CLuaGraphics, textWidth),
	method(CLuaGraphics, save),
	method(CLuaGraphics, restore),
	method(CLuaGraphics, skew),
	method(CLuaGraphics, star),
	method(CLuaGraphics, polygon),
	method(CLuaGraphics, polyline),
	method(CLuaGraphics, bezierCurveTo),
	method(CLuaGraphics, vlineTo),
	method(CLuaGraphics, hlineTo),
	method(CLuaGraphics, arcTo),
	method(CLuaGraphics, ellipseTo),
	method(CLuaGraphics, quadraticCurveTo),
	
	{0, 0}
};

CLuaGraphics::CLuaGraphics(lua_State* L)
:m_pSurface(NULL), m_pGraphics(NULL)
{
	int w = PAI(L, 1);
	int h = PAI(L, 2);
	
	w = w <= 0 ? 1 : w;
	h = h <= 0 ? 1 : h;

	m_pSurface = htmlayout::image::create(w, h);
	if(m_pSurface)
		m_pGraphics = new htmlayout::graphics(m_pSurface);

	

}

CLuaGraphics::~CLuaGraphics()
{
	empty();	
}

int CLuaGraphics::get_handle(lua_State* L)
{
	lua_pushnumber(L, (long)m_pSurface);
	return 1;
}

int CLuaGraphics::no_line(lua_State* L)
{
	gApi()->no_line();
	return 0;
}

int CLuaGraphics::moveTo(lua_State* L)
{
	gApi()->move_to(PARAM_FLOAT_2, lua_toboolean(L, 3));
	return 0;
}

int CLuaGraphics::lineTo(lua_State* L)
{	
	gApi()->line_to(PARAM_FLOAT_2, lua_toboolean(L, 3));
	return 0;
}

int CLuaGraphics::line(lua_State* L)
{
	gApi()->line(PARAM_FLOAT_4);
	return 0;
}

int CLuaGraphics::clear(lua_State* L)
{
	if(m_pSurface)
		m_pSurface->clear();
	return 0;
}

int CLuaGraphics::blit(lua_State* L)
{
	if(m_pSurface == NULL)
		return 0;
	HDC hdc = (HDC)PAI(L, 1);

	m_pSurface->blit(hdc,PAI(L, 2), PAI(L, 3), PAI(L, 4), PAI(L, 5), PAI(L, 6), PAI(L, 7));

	
	return 0;
}

int CLuaGraphics::triangle(lua_State* L)
{
	gApi()->triangle(PARAM_FLOAT_6);
	return 0;
}

int CLuaGraphics::rectangle(lua_State* L)
{
	if(lua_isnoneornil(L, 5))
	{
		gApi()->rectangle(PARAM_FLOAT_4);
	}
	else if(lua_isnoneornil(L, 6))
	{
		gApi()->rectangle(PARAM_FLOAT_5);
	}
	else if(lua_isnoneornil(L, 7))
	{
		gApi()->rectangle(PARAM_FLOAT_5,PAD(L, 5), PAD(L, 6), PAD(L, 6));
	}
	else 
	{
		gApi()->rectangle(PARAM_FLOAT_8);
	}
	return 0;
}

int CLuaGraphics::ellipse(lua_State* L)
{
	if(lua_isnoneornil(L, 4))
	{
		gApi()->ellipse(PARAM_FLOAT_3, PAD(L, 3));
	}
	else
	{
		gApi()->ellipse(PARAM_FLOAT_4);
	}
	return 0;
}

int CLuaGraphics::arc(lua_State* L)
{
	gApi()->arc(PARAM_FLOAT_4, PAD(L, 5), PAD(L, 6));
	return 0;
}
