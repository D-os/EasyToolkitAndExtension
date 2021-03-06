/* --------------------------------------------------------------------------
 *
 * ETK++ --- The Easy Toolkit for C++ programing
 * Copyright (C) 2004-2006, Anthony Lee, All Rights Reserved
 *
 * ETK++ library is a freeware; it may be used and distributed according to
 * the terms of The MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * File: etk-drawing.cpp
 *
 * --------------------------------------------------------------------------*/

#include <math.h>
#ifndef M_PI
	#define M_PI	3.14159265358979323846
#endif // M_PI

#include "etk-win32gdi.h"

#include <etk/support/ClassInfo.h>
#include <etk/support/Autolock.h>
#include <etk/render/Pixmap.h>


e_status_t
EWin32GraphicsDrawable::StrokePoint(EGraphicsContext *dc,
				    eint32 x, eint32 y)
{
	if(fRequestAsyncWin == NULL || dc == NULL) return E_ERROR;

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_POINT;
	callback.pixmap = this;
	callback.dc = dc;
	callback.x = x;
	callback.y = y;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::StrokePoints(EGraphicsContext *dc,
				     const eint32 *pts, eint32 count)
{
	if(fRequestAsyncWin == NULL || dc == NULL || pts == NULL || count <= 0) return E_ERROR;

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_POINTS;
	callback.pixmap = this;
	callback.dc = dc;
	callback.pts = pts;
	callback.ptsCount = count;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::StrokePoints_Colors(EGraphicsContext *dc,
					    const EList *ptsArrayLists, eint32 arrayCount, const e_rgb_color *highColors)
{
	if(fRequestAsyncWin == NULL || dc == NULL || ptsArrayLists == NULL || arrayCount <= 0) return E_ERROR;

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_POINTS_COLOR;
	callback.pixmap = this;
	callback.dc = dc;
	callback.ptsArrayLists = ptsArrayLists;
	callback.ptsArrayCount = arrayCount;
	callback.ptsColors = highColors;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::StrokePoints_Alphas(EGraphicsContext *dc,
					    const eint32 *pts, const euint8 *alpha, eint32 count)
{
	if(fRequestAsyncWin == NULL || dc == NULL || pts == NULL || alpha == NULL || count <= 0) return E_ERROR;

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_POINTS_ALPHA;
	callback.pixmap = this;
	callback.dc = dc;
	callback.pts = pts;
	callback.ptsAlpha = alpha;
	callback.ptsCount = count;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::StrokeLine(EGraphicsContext *dc,
				   eint32 x0, eint32 y0, eint32 x1, eint32 y1)
{
	if(x0 == x1 && y0 == y1) return StrokePoint(dc, x0, y0);

	if(fRequestAsyncWin == NULL || dc == NULL) return E_ERROR;

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_LINE;
	callback.pixmap = this;
	callback.dc = dc;
	callback.x = x0;
	callback.y = y0;
	callback.wx = x1;
	callback.wy = y1;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::StrokeRect(EGraphicsContext *dc,
				   eint32 x, eint32 y, euint32 w, euint32 h)
{
	if(fRequestAsyncWin == NULL || dc == NULL) return E_ERROR;

	if(w == 0 && h == 0)
		return StrokePoint(dc, x, y);
	else if(w == 0 || h == 0)
		return StrokeLine(dc, x, y, x + (eint32)w, y + (eint32)h);

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_RECT;
	callback.pixmap = this;
	callback.dc = dc;
	callback.x = x;
	callback.y = y;
	callback.w = w;
	callback.h = h;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::StrokeRects(EGraphicsContext *dc,
				    const eint32 *rects, eint32 count)
{
	if(fRequestAsyncWin == NULL || dc == NULL || rects == NULL || count <= 0) return E_ERROR;

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_RECTS;
	callback.pixmap = this;
	callback.dc = dc;
	callback.pts = rects;
	callback.ptsCount = count;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::FillRect(EGraphicsContext *dc,
				 eint32 x, eint32 y, euint32 w, euint32 h)
{
	if(fRequestAsyncWin == NULL || dc == NULL) return E_ERROR;

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_FILL_RECT;
	callback.pixmap = this;
	callback.dc = dc;
	callback.x = x;
	callback.y = y;
	callback.w = w;
	callback.h = h;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::FillRects(EGraphicsContext *dc,
				  const eint32 *rects, eint32 count)
{
	if(fRequestAsyncWin == NULL || dc == NULL || rects == NULL || count <= 0) return E_ERROR;

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_FILL_RECTS;
	callback.pixmap = this;
	callback.dc = dc;
	callback.pts = rects;
	callback.ptsCount = count;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::FillRegion(EGraphicsContext *dc,
				   const ERegion &region)
{
	if(fRequestAsyncWin == NULL || dc == NULL || region.CountRects() <= 0) return E_ERROR;

	if(region.CountRects() == 1)
	{
		ERect r = region.RectAt(0).FloorSelf();
		return FillRect(dc, (eint32)r.left, (eint32)r.top, (euint32)r.Width(), (euint32)r.Height());
	}

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_FILL_REGION;
	callback.pixmap = this;
	callback.dc = dc;
	callback.region = &region;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::StrokeRoundRect(EGraphicsContext *dc,
					eint32 x, eint32 y, euint32 w, euint32 h, euint32 xRadius, euint32 yRadius)
{
	if(fRequestAsyncWin == NULL || dc == NULL) return E_ERROR;

	bool xRadiusLarge = (2 * xRadius >= w ? true : false);
	bool yRadiusLarge = (2 * yRadius >= h ? true : false);

	if(xRadius == 0 || yRadius == 0) return StrokeRect(dc, x, y, w, h);
	else if(xRadiusLarge && yRadiusLarge) return StrokeArc(dc, x, y, w, h, 0, 360);

	if(w == 0 && h == 0)
		return StrokePoint(dc, x, y);
	else if(w == 0 || h == 0)
		return StrokeLine(dc, x, y, x + (eint32)w, y + (eint32)h);

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_ROUND_RECT;
	callback.pixmap = this;
	callback.dc = dc;
	callback.x = x;
	callback.y = y;
	callback.w = w;
	callback.h = h;
	callback.ww = 2 * xRadius;
	callback.wh = 2 * yRadius;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::FillRoundRect(EGraphicsContext *dc,
				      eint32 x, eint32 y, euint32 w, euint32 h, euint32 xRadius, euint32 yRadius)
{
	if(fRequestAsyncWin == NULL || dc == NULL) return E_ERROR;

	bool xRadiusLarge = (2 * xRadius >= w ? true : false);
	bool yRadiusLarge = (2 * yRadius >= h ? true : false);

	if(xRadius == 0 || yRadius == 0) return FillRect(dc, x, y, w, h);
	else if(xRadiusLarge && yRadiusLarge) return FillArc(dc, x, y, w, h, 0, 360);

	if(w == 0 || h == 0) return FillRect(dc, x, y, w, h);

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_FILL_ROUND_RECT;
	callback.pixmap = this;
	callback.dc = dc;
	callback.x = x;
	callback.y = y;
	callback.w = w;
	callback.h = h;
	callback.ww = 2 * xRadius;
	callback.wh = 2 * yRadius;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::StrokeArc(EGraphicsContext *dc,
				  eint32 x, eint32 y, euint32 w, euint32 h, float startAngle, float endAngle)
{
	if(fRequestAsyncWin == NULL || dc == NULL) return E_ERROR;

	if(w == 0 && h == 0)
		return StrokePoint(dc, x, y);
	else if(w == 0 || h == 0)
		return StrokeLine(dc, x, y, x + (eint32)w, y + (eint32)h);

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_ARC;
	callback.pixmap = this;
	callback.dc = dc;
	callback.x = x;
	callback.y = y;
	callback.w = w;
	callback.h = h;
	callback.startAngle = startAngle;
	callback.endAngle = endAngle;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::FillArc(EGraphicsContext *dc,
				eint32 x, eint32 y, euint32 w, euint32 h, float startAngle, float endAngle)
{
	if(fRequestAsyncWin == NULL || dc == NULL) return E_ERROR;

	if(w == 0 || h == 0) return FillRect(dc, x, y, w, h);

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_FILL_ARC;
	callback.pixmap = this;
	callback.dc = dc;
	callback.x = x;
	callback.y = y;
	callback.w = w;
	callback.h = h;
	callback.startAngle = startAngle;
	callback.endAngle = endAngle;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::StrokePolygon(EGraphicsContext *dc,
				      const eint32 *pts, eint32 count, bool closed)
{
	if(fRequestAsyncWin == NULL || dc == NULL || pts == NULL || count <= 0) return E_ERROR;

	if(count == 1)
		return StrokePoint(dc, pts[0], pts[1]);
	else if(count == 2)
		return StrokeLine(dc, pts[0], pts[1], pts[2], pts[3]);

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_STROKE_POLYGON;
	callback.pixmap = this;
	callback.dc = dc;
	callback.pts = pts;
	callback.ptsCount = count;
	callback.polyClosed = closed;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


e_status_t
EWin32GraphicsDrawable::FillPolygon(EGraphicsContext *dc,
				    const eint32 *pts, eint32 count)
{
	if(fRequestAsyncWin == NULL || dc == NULL || pts == NULL || count <= 0) return E_ERROR;

	if(count == 1) return FillRect(dc, pts[0], pts[1], 0, 0);

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_FILL_POLYGON;
	callback.pixmap = this;
	callback.dc = dc;
	callback.pts = pts;
	callback.ptsCount = count;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_DRAWING, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


HRGN
EWin32GraphicsEngine::ConvertRegion(const ERegion *region)
{
	HRGN hrgn = NULL;

	for(eint32 i = 0; i < (region ? region->CountRects() : 0); i++)
	{
		ERect r = region->RectAt(i).FloorCopy();
		if(i == 0) {hrgn = CreateRectRgn((int)r.left, (int)r.top, (int)r.right + 1, (int)r.bottom + 1); continue;}
		else if(hrgn == NULL) break;

		HRGN hrgn1 = CreateRectRgn((int)r.left, (int)r.top, (int)r.right + 1, (int)r.bottom + 1);
		if(hrgn1 == NULL) {DeleteObject(hrgn); hrgn = NULL; break;}

		CombineRgn(hrgn, hrgn, hrgn1, RGN_OR);
		DeleteObject(hrgn1);
	}

	return hrgn;
}


bool
EWin32GraphicsEngine::PrepareContext(EWin32GraphicsDrawable *pixmap, EGraphicsContext *dc,
				     bool hollowBrush, bool setPenSize)
{
	if(pixmap == NULL || pixmap->win32HDC == NULL || dc == NULL || dc->Clipping()->CountRects() <= 0) return false;

	SetTextColor(pixmap->win32HDC, RGB(dc->HighColor().red, dc->HighColor().green, dc->HighColor().blue));
	SetBkMode(pixmap->win32HDC, TRANSPARENT);

	LOGBRUSH plb;
	HBITMAP hbm = NULL;
	HGDIOBJ oldHbm = NULL;
	HDC hdc = NULL;
	plb.lbHatch = 0;
	if(dc->Pattern() == E_SOLID_HIGH || dc->Pattern() == E_SOLID_LOW)
	{
		plb.lbStyle = BS_SOLID;
		if(dc->Pattern() == E_SOLID_HIGH)
			plb.lbColor = RGB(dc->HighColor().red, dc->HighColor().green, dc->HighColor().blue);
		else
			plb.lbColor = RGB(dc->LowColor().red, dc->LowColor().green, dc->LowColor().blue);
	}
	else
	{
		plb.lbStyle = BS_PATTERN;
		hdc = CreateCompatibleDC(win32ScreenHDC);
		hbm = CreateCompatibleBitmap(win32ScreenHDC, 8, 8);
		if(hbm != NULL)
		{
			oldHbm = SelectObject(hdc, hbm);
			COLORREF wHighColor = RGB(dc->HighColor().red, dc->HighColor().green, dc->HighColor().blue);
			COLORREF wLowColor = RGB(dc->LowColor().red, dc->LowColor().green, dc->LowColor().blue);
			for(int i = 0; i < 8; i++)
				for(int j = 0; j < 8; j++)
					SetPixel(hdc, j, i,
						 ((dc->Pattern().data[i] & (1 << (7 - j))) ? wHighColor : wLowColor));
		}
		else
		{
			ETK_DEBUG("[GRAPHICS]: %s --- CreateCompatibleBitmap failed.", __PRETTY_FUNCTION__);
		}
		plb.lbHatch = (LONG)hbm;
	}

	HPEN newPen = ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER,
				   (setPenSize ? (int)(dc->PenSize() <= 1 ? 0 : dc->PenSize()) : 0), &plb, 0, NULL);
	HBRUSH newBrush = NULL;
	if(hollowBrush) plb.lbStyle = BS_NULL;
	newBrush = CreateBrushIndirect(&plb);

	if(hbm != NULL)
	{
		if(hdc && oldHbm) SelectObject(hdc, oldHbm);
		DeleteObject(hbm);
	}
	if(hdc != NULL) DeleteDC(hdc);

	if(newPen == NULL || newBrush == NULL)
	{
		if(newPen != NULL) DeleteObject(newPen);
		if(newBrush != NULL) DeleteObject(newBrush);
		return false;
	}

	HPEN oldPen = (HPEN)SelectObject(pixmap->win32HDC, newPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(pixmap->win32HDC, newBrush);

	if(oldPen != pixmap->win32Pen && oldPen != NULL) DeleteObject(oldPen);
	if(oldBrush != pixmap->win32Brush && oldBrush != NULL) DeleteObject(oldBrush);

	if(pixmap->win32Pen != NULL) DeleteObject(pixmap->win32Pen);
	if(pixmap->win32Brush != NULL) DeleteObject(pixmap->win32Brush);

	pixmap->win32Pen = newPen;
	pixmap->win32Brush = newBrush;

	HRGN hrgn = ConvertRegion(dc->Clipping());
	if(hrgn == NULL) return false;

	SelectClipRgn(pixmap->win32HDC, hrgn);
	DeleteObject(hrgn);

	int fnDrawMode;
	switch(dc->DrawingMode())
	{
		case E_OP_COPY: fnDrawMode = R2_COPYPEN; break;
		case E_OP_XOR: fnDrawMode = R2_XORPEN; break;
//		case E_OP_INVERT: fnDrawMode = R2_NOT; break;
//		case E_OP_ERASE: fnDrawMode = R2_BLACK; break;
//		case E_OP_ADD: fnDrawMode = R2_MERGEPEN; break;
//		case E_OP_SUBTRACT: fnDrawMode = R2_NOTMERGEPEN; break;
//		case E_OP_SELECT: fnDrawMode = R2_MASKPEN; break;
//		case E_OP_BLEND: break;
//		case E_OP_MIN: break;
//		case E_OP_MAX: break;
//		case E_OP_ALPHA: break; /* TODO */
		default:
			ETK_WARNING("[GRAPHICS]: %s --- DrawingMode %u not support!", __PRETTY_FUNCTION__, (unsigned int)dc->DrawingMode());
			fnDrawMode = R2_COPYPEN;
			break;
	}
	SetROP2(pixmap->win32HDC, fnDrawMode);

	return true;
}


static bool _etk_dc_query_high_color(const e_pattern &pattern, eint32 x, eint32 y)
{
	if(pattern == E_SOLID_HIGH) return true;
	else if(pattern == E_SOLID_LOW) return false;

	x %= 8;
	y %= 8;

	euint8 pat = pattern.data[y];
	if(pat & (1 << (7 - x))) return true;

	return false;
}


LRESULT _etk_stroke_point(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_POINT || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

	LRESULT retVal = FALSE;

	if(callback->dc->PenSize() <= 1)
	{
		COLORREF color;
		if(_etk_dc_query_high_color(callback->dc->Pattern(), callback->x, callback->y))
			color = RGB(callback->dc->HighColor().red, callback->dc->HighColor().green, callback->dc->HighColor().blue);
		else
			color = RGB(callback->dc->LowColor().red, callback->dc->LowColor().green, callback->dc->LowColor().blue);

		retVal = (SetPixel(callback->pixmap->win32HDC, callback->x, callback->y, color) == 0 ? TRUE : FALSE);
	}
	else
	{
		int left = callback->x - (int)((callback->dc->PenSize() - 1) / 2);
		int top = callback->y - (int)((callback->dc->PenSize() - 1) / 2);
		int right = left + (int)callback->dc->PenSize() + 1;
		int bottom = top + (int)callback->dc->PenSize() + 1;

		if(callback->dc->IsSquarePointStyle())
			retVal = (Rectangle(callback->pixmap->win32HDC, left, top, right, bottom) == 0 ? FALSE : TRUE);
		else
			retVal = (Ellipse(callback->pixmap->win32HDC, left, top, right, bottom) == 0 ? FALSE : TRUE);
	}

	return retVal;
}


LRESULT _etk_stroke_points(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_POINTS || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL ||
	   callback->pts == NULL || callback->ptsCount <= 0) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

	const eint32 *pts = callback->pts;
	if(callback->dc->PenSize() <= 1)
	{
		COLORREF wHighColor = RGB(callback->dc->HighColor().red, callback->dc->HighColor().green, callback->dc->HighColor().blue);
		COLORREF wLowColor = RGB(callback->dc->LowColor().red, callback->dc->LowColor().green, callback->dc->LowColor().blue);

		for(eint32 i = 0; i < callback->ptsCount; i++)
		{
			eint32 x = *pts++;
			eint32 y = *pts++;
			SetPixel(callback->pixmap->win32HDC, x, y,
				 (_etk_dc_query_high_color(callback->dc->Pattern(), x, y) ? wHighColor : wLowColor));
		}
	}
	else
	{
		for(eint32 i = 0; i < callback->ptsCount; i++)
		{
			eint32 x = *pts++;
			eint32 y = *pts++;

			int left = x - (int)((callback->dc->PenSize() - 1) / 2);
			int top = y - (int)((callback->dc->PenSize() - 1) / 2);
			int right = left + (int)callback->dc->PenSize() + 1;
			int bottom = top + (int)callback->dc->PenSize() + 1;

			if(callback->dc->IsSquarePointStyle())
				Rectangle(callback->pixmap->win32HDC, left, top, right, bottom);
			else
				Ellipse(callback->pixmap->win32HDC, left, top, right, bottom);
		}
	}

	return TRUE;
}


static bool _etk_pixmap_change_brush_high_color(EWin32GraphicsEngine *win32Engine, EWin32GraphicsDrawable *pixmap,
						EGraphicsContext *dc, e_rgb_color high_color)
{
	LOGBRUSH plb;
	plb.lbHatch = 0;
	HBITMAP hbm = NULL;
	HGDIOBJ oldHbm = NULL;
	HDC hdc = NULL;
	if(dc->Pattern() == E_SOLID_HIGH || dc->Pattern() == E_SOLID_LOW)
	{
		plb.lbStyle = BS_SOLID;
		if(dc->Pattern() == E_SOLID_HIGH)
			plb.lbColor = RGB(high_color.red, high_color.green, high_color.blue);
		else
			plb.lbColor = RGB(dc->LowColor().red, dc->LowColor().green, dc->LowColor().blue);
	}
	else
	{
		plb.lbStyle = BS_PATTERN;
		hdc = CreateCompatibleDC(win32Engine->win32ScreenHDC);
		hbm = CreateCompatibleBitmap(win32Engine->win32ScreenHDC, 8, 8);
		if(hbm != NULL)
		{
			oldHbm = SelectObject(hdc, hbm);
			COLORREF wHighColor = RGB(high_color.red, high_color.green, high_color.blue);
			COLORREF wLowColor = RGB(dc->LowColor().red, dc->LowColor().green, dc->LowColor().blue);
			for(int i = 0; i < 8; i++)
				for(int j = 0; j < 8; j++)
					SetPixel(hdc, j, i,
						 ((dc->Pattern().data[i] & (1 << (7 - j))) ? wHighColor : wLowColor));
		}
		else
		{
			ETK_DEBUG("[GRAPHICS]: %s --- CreateCompatibleBitmap failed.", __PRETTY_FUNCTION__);
		}
		plb.lbHatch = (LONG)hbm;
	}

	HBRUSH newBrush = CreateBrushIndirect(&plb);

	if(hbm != NULL)
	{
		if(hdc && oldHbm) SelectObject(hdc, oldHbm);
		DeleteObject(hbm);
	}
	if(hdc != NULL) DeleteDC(hdc);

	if(newBrush == NULL) return false;

	HBRUSH oldBrush = (HBRUSH)SelectObject(pixmap->win32HDC, newBrush);

	if(oldBrush != pixmap->win32Brush && oldBrush != NULL) DeleteObject(oldBrush);

	if(pixmap->win32Brush != NULL) DeleteObject(pixmap->win32Brush);

	pixmap->win32Brush = newBrush;

	return true;
}


LRESULT _etk_stroke_points_color(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_POINTS_COLOR || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL ||
	   callback->ptsArrayLists == NULL || callback->ptsArrayCount <= 0) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

	const EList *ptsArrayLists = callback->ptsArrayLists;
	const e_rgb_color *high_colors = callback->ptsColors;

	e_rgb_color oldColor = callback->dc->HighColor();

	for(eint32 k = 0; k < callback->ptsArrayCount; k++, ptsArrayLists++)
	{
		if(ptsArrayLists == NULL) break;

		e_rgb_color color = (high_colors == NULL ? callback->dc->HighColor() : *high_colors++);

		eint32 count = ptsArrayLists->CountItems();
		if(count <= 0) continue;

		if(callback->dc->PenSize() <= 1)
		{
			COLORREF wHighColor = RGB(color.red, color.green, color.blue);
			COLORREF wLowColor = RGB(callback->dc->LowColor().red,
						 callback->dc->LowColor().green,
						 callback->dc->LowColor().blue);

			for(eint32 i = 0; i < count; i++)
			{
				const eint32 *pt = (const eint32*)ptsArrayLists->ItemAt(i);
				if(pt == NULL) continue;

				eint32 x = *pt++;
				eint32 y = *pt++;

				SetPixel(callback->pixmap->win32HDC, x, y,
					 (_etk_dc_query_high_color(callback->dc->Pattern(), x, y) ? wHighColor : wLowColor));
			}
		}
		else
		{
			for(eint32 i = 0; i < count; i++)
			{
				const eint32 *pt = (const eint32*)ptsArrayLists->ItemAt(i);
				if(pt == NULL) continue;

				eint32 x = *pt++;
				eint32 y = *pt++;

				if(oldColor != color)
				{
					_etk_pixmap_change_brush_high_color(win32Engine, callback->pixmap, callback->dc, color);
					oldColor = color;
				}

				int left = x - (int)((callback->dc->PenSize() - 1) / 2);
				int top = y - (int)((callback->dc->PenSize() - 1) / 2);
				int right = left + (int)callback->dc->PenSize() + 1;
				int bottom = top + (int)callback->dc->PenSize() + 1;

				if(callback->dc->IsSquarePointStyle())
					Rectangle(callback->pixmap->win32HDC, left, top, right, bottom);
				else
					Ellipse(callback->pixmap->win32HDC, left, top, right, bottom);
			}
		}
	}

	return TRUE;
}


LRESULT _etk_stroke_points_alpha(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_POINTS_ALPHA || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL ||
	   callback->pts == NULL || callback->ptsAlpha == NULL || callback->ptsCount <= 0) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

	if(callback->dc->PenSize() <= 1)
	{
		const eint32 *pts = callback->pts;
		const euint8 *alphas = callback->ptsAlpha;

		for(eint32 i = 0; i < callback->ptsCount; i++)
		{
			eint32 x = *pts++;
			eint32 y = *pts++;
			euint8 alpha = *alphas++;

			COLORREF wcolor = GetPixel(callback->pixmap->win32HDC, (int)x, (int)y);
			e_rgb_color color;
			color.set_to(GetRValue(wcolor), GetGValue(wcolor), GetBValue(wcolor), 255);
			color.mix(callback->dc->HighColor().red,
				  callback->dc->HighColor().green,
				  callback->dc->HighColor().blue,
				  alpha);

			wcolor = RGB(color.red, color.green, color.green);
			SetPixel(callback->pixmap->win32HDC, x, y, wcolor);
		}

		return TRUE;
	}
	else
	{
		ETK_WARNING("[GRAPHICS]: %s --- stroke large point not support yet.", __PRETTY_FUNCTION__);
	}

	// TODO
	return FALSE;
}


LRESULT _etk_stroke_line(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_LINE || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, true, true) == false) return FALSE;

	POINT pt;
	pt.x = callback->wx; pt.y = callback->wy;

	MoveToEx(callback->pixmap->win32HDC, callback->x, callback->y, NULL);
	LineTo(callback->pixmap->win32HDC, pt.x, pt.y);

	if(callback->dc->PenSize() <= 1)
	{
		COLORREF wcolor;
		if(_etk_dc_query_high_color(callback->dc->Pattern(), pt.x, pt.y))
			wcolor = RGB(callback->dc->HighColor().red, callback->dc->HighColor().green, callback->dc->HighColor().blue);
		else
			wcolor = RGB(callback->dc->LowColor().red, callback->dc->LowColor().green, callback->dc->LowColor().blue);

		SetPixel(callback->pixmap->win32HDC, pt.x, pt.y, wcolor);
	}
	else
	{
		// TODO: draw end point --- wide line
	}

	return TRUE;
}


LRESULT _etk_stroke_rect(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_RECT || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, true, true) == false) return FALSE;

	if(Rectangle(callback->pixmap->win32HDC, callback->x, callback->y,
		     callback->x + (int)callback->w + 1, callback->y + (int)callback->h + 1) == 0) return FALSE;

	return TRUE;
}


LRESULT _etk_stroke_rects(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_RECTS || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->dc == NULL ||
	   callback->pts == NULL || callback->pixmap->win32HDC == NULL || callback->ptsCount <= 0) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, true, true) == false) return FALSE;

	const eint32 *pts = callback->pts;
	for(eint32 i = 0; i < callback->ptsCount; i++)
	{
		eint32 x = *pts++; eint32 y = *pts++; euint32 w = (euint32)(*pts++); euint32 h = (euint32)(*pts++);
		Rectangle(callback->pixmap->win32HDC, x, y, x + (int)w + 1, y + (int)h + 1);
	}

	return TRUE;
}


LRESULT _etk_fill_rect(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_FILL_RECT || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

	if(Rectangle(callback->pixmap->win32HDC, callback->x, callback->y,
		     callback->x + (int)callback->w + 1, callback->y + (int)callback->h + 1) == 0) return FALSE;

	return TRUE;
}


LRESULT _etk_fill_rects(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_FILL_RECTS || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL ||
	   callback->pts == NULL || callback->ptsCount <= 0) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

	const eint32 *pts = callback->pts;
	for(eint32 i = 0; i < callback->ptsCount; i++)
	{
		eint32 x = *pts++; eint32 y = *pts++; euint32 w = (euint32)(*pts++); euint32 h = (euint32)(*pts++);
		Rectangle(callback->pixmap->win32HDC, x, y, x + (int)w + 1, y + (int)h + 1);
	}

	return TRUE;
}


LRESULT _etk_fill_region(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_FILL_REGION || callback->region == NULL || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

	HRGN hrgn = NULL;
	for(eint32 i = 0; i < callback->region->CountRects(); i++)
	{
		ERect r = callback->region->RectAt(i).FloorSelf();
		if(i == 0) {hrgn = CreateRectRgn((int)r.left, (int)r.top, (int)r.right + 1, (int)r.bottom + 1); continue;}
		else if(hrgn == NULL) break;

		HRGN hrgn1 = CreateRectRgn((int)r.left, (int)r.top, (int)r.right + 1, (int)r.bottom + 1);
		if(hrgn1 == NULL) {DeleteObject(hrgn); hrgn = NULL; break;}

		CombineRgn(hrgn, hrgn, hrgn1, RGN_OR);
		DeleteObject(hrgn1);
	}
	if(hrgn == NULL)
	{
		ETK_DEBUG("[GRAPHICS]: %s --- Unable to create win32 region.", __PRETTY_FUNCTION__);
		return FALSE;
	}

	BOOL status = FillRgn(callback->pixmap->win32HDC, hrgn, callback->pixmap->win32Brush);

	DeleteObject(hrgn);

	return (LRESULT)status;
}


LRESULT _etk_stroke_round_rect(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_ROUND_RECT || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, true, true) == false) return FALSE;

	if(RoundRect(callback->pixmap->win32HDC, callback->x, callback->y,
		     callback->x + (int)callback->w + 1, callback->y + (int)callback->h + 1,
		     (int)callback->ww, (int)callback->wh) == 0) return FALSE;

	return TRUE;
}


LRESULT _etk_fill_round_rect(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_FILL_ROUND_RECT || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

	if(RoundRect(callback->pixmap->win32HDC, callback->x, callback->y,
		     callback->x + (int)callback->w + 1, callback->y + (int)callback->h + 1,
		     (int)callback->ww, (int)callback->wh) == 0) return FALSE;

	return TRUE;
}


LRESULT _etk_stroke_arc(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_ARC || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL) return FALSE;

	float start_angle = callback->startAngle;
	float end_angle = callback->endAngle;

	if(end_angle - start_angle >= 360 || end_angle - start_angle <= -360)
	{
		start_angle = 0; end_angle = 360;
	}
	else
	{
		start_angle = (float)fmod((double)start_angle, (double)360);
		end_angle = (float)fmod((double)end_angle, (double)360);
	}

	if(start_angle < 0) start_angle = 360.f + start_angle;
	if(end_angle < 0) end_angle = 360.f + end_angle;

	float x = (float)callback->x;
	float y = (float)callback->y;
	float w = (float)callback->w;
	float h = (float)callback->h;

	EPoint ptStart, ptEnd;
	if(start_angle == end_angle || end_angle - start_angle == 360)
	{
		ptStart = ptEnd = EPoint(x + w, y + h / 2.f);
	}
	else
	{
		float xRadius = w / 2.f;
		float yRadius = h / 2.f;
		float xCenter = x + xRadius;
		float yCenter = y + yRadius;
		ptStart.x = xCenter + xRadius * (float)cos(M_PI * (double)start_angle / 180.f);
		ptStart.y = yCenter - yRadius * (float)sin(M_PI * (double)start_angle / 180.f);
		ptEnd.x = xCenter + xRadius * (float)cos(M_PI * (double)end_angle / 180.f);
		ptEnd.y = yCenter - yRadius * (float)sin(M_PI * (double)end_angle / 180.f);
	}
	ptStart.FloorSelf();
	ptEnd.FloorSelf();

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, true, true) == false) return FALSE;

	BOOL status;
	if(ptStart == ptEnd)
	{
		status = Ellipse(callback->pixmap->win32HDC,
				 callback->x, callback->y, callback->x + (eint32)callback->w + 1, callback->y + (eint32)callback->h + 1);
	}
	else
	{
		status = Arc(callback->pixmap->win32HDC,
			     callback->x, callback->y, callback->x + (eint32)callback->w + 1, callback->y + (eint32)callback->h + 1,
			     (int)ptStart.x, (int)ptStart.y, (int)ptEnd.x, (int)ptEnd.y);
	}

	return (LRESULT)status;
}


LRESULT _etk_fill_arc(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_FILL_ARC || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL) return FALSE;

	float start_angle = callback->startAngle;
	float end_angle = callback->endAngle;

	if(end_angle - start_angle >= 360 || end_angle - start_angle <= -360)
	{
		start_angle = 0; end_angle = 360;
	}
	else
	{
		start_angle = (float)fmod((double)start_angle, (double)360);
		end_angle = (float)fmod((double)end_angle, (double)360);
	}

	if(start_angle < 0) start_angle = 360.f + start_angle;
	if(end_angle < 0) end_angle = 360.f + end_angle;

	float x = (float)callback->x;
	float y = (float)callback->y;
	float w = (float)callback->w;
	float h = (float)callback->h;

	EPoint ptStart, ptEnd;
	if(start_angle == end_angle || end_angle - start_angle == 360)
	{
		ptStart = ptEnd = EPoint(x + w, y + h / 2.f);
	}
	else
	{
		float xRadius = w / 2.f;
		float yRadius = h / 2.f;
		float xCenter = x + xRadius;
		float yCenter = y + yRadius;
		ptStart.x = xCenter + xRadius * (float)cos(M_PI * (double)start_angle / 180.f);
		ptStart.y = yCenter - yRadius * (float)sin(M_PI * (double)start_angle / 180.f);
		ptEnd.x = xCenter + xRadius * (float)cos(M_PI * (double)end_angle / 180.f);
		ptEnd.y = yCenter - yRadius * (float)sin(M_PI * (double)end_angle / 180.f);
	}
	ptStart.FloorSelf();
	ptEnd.FloorSelf();

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

	BOOL status;
	if(ptStart == ptEnd)
	{
		status = Ellipse(callback->pixmap->win32HDC,
				 callback->x, callback->y, callback->x + (eint32)callback->w + 1, callback->y + (eint32)callback->h + 1);
	}
	else
	{
		status = Pie(callback->pixmap->win32HDC,
			     callback->x, callback->y, callback->x + (eint32)callback->w + 1, callback->y + (eint32)callback->h + 1,
			     (int)ptStart.x, (int)ptStart.y, (int)ptEnd.x, (int)ptEnd.y);
	}

	return (LRESULT)status;
}


LRESULT _etk_stroke_polygon(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_STROKE_POLYGON || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL ||
	   callback->pts == NULL || callback->ptsCount < 3 || callback->ptsCount >= E_MAXINT) return FALSE;

	POINT *wPts = new POINT[callback->ptsCount + 1];
	if(wPts == NULL) return FALSE;

	const eint32 *pts = callback->pts;
	for(eint32 i = 0; i < callback->ptsCount; i++)
	{
		wPts[i].x = *pts++;
		wPts[i].y = *pts++;
	}

	int count = callback->ptsCount;
	bool poly_closed = callback->polyClosed;
	if((wPts[count - 1].x != wPts[0].x || wPts[count - 1].y != wPts[0].y) && poly_closed)
	{
		wPts[count].x = wPts[0].x;
		wPts[count].y = wPts[0].y;
		count++;
	}
	else if(wPts[count - 1].x == wPts[0].x && wPts[count - 1].y == wPts[0].y)
	{
		poly_closed = true;
	}

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, true, true) == false)
	{
		delete[] wPts;
		return FALSE;
	}

	BOOL status;
	if(poly_closed)
	{
		status = Polygon(callback->pixmap->win32HDC, wPts, count);
	}
	else
	{
		if((status = Polyline(callback->pixmap->win32HDC, wPts, count)) != 0)
		{
			if(callback->dc->PenSize() <= 1)
			{
				COLORREF wcolor;
				if(_etk_dc_query_high_color(callback->dc->Pattern(), wPts[count - 1].x, wPts[count - 1].y))
					wcolor = RGB(callback->dc->HighColor().red,
						     callback->dc->HighColor().green,
						     callback->dc->HighColor().blue);
				else
					wcolor = RGB(callback->dc->LowColor().red,
						     callback->dc->LowColor().green,
						     callback->dc->LowColor().blue);

				SetPixel(callback->pixmap->win32HDC, wPts[count - 1].x, wPts[count - 1].y, wcolor);
			}
			else
			{
				// TODO: draw end point --- wide line
			}
		}
	}

	delete[] wPts;

	return (LRESULT)status;
}


LRESULT _etk_fill_polygon(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_FILL_POLYGON || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL || callback->dc == NULL ||
	   callback->pts == NULL || callback->ptsCount < 2 || callback->ptsCount > E_MAXINT) return FALSE;

	if(callback->ptsCount == 2)
	{
		EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
		if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

		if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false) return FALSE;

		POINT pt;
		pt.x = callback->pts[2]; pt.y = callback->pts[3];

		MoveToEx(callback->pixmap->win32HDC, callback->pts[0], callback->pts[1], NULL);
		LineTo(callback->pixmap->win32HDC, pt.x, pt.y);

		COLORREF wcolor;
		if(_etk_dc_query_high_color(callback->dc->Pattern(), pt.x, pt.y))
			wcolor = RGB(callback->dc->HighColor().red, callback->dc->HighColor().green, callback->dc->HighColor().blue);
		else
			wcolor = RGB(callback->dc->LowColor().red, callback->dc->LowColor().green, callback->dc->LowColor().blue);

		SetPixel(callback->pixmap->win32HDC, pt.x, pt.y, wcolor);

		return TRUE;
	}

	POINT *wPts = new POINT[callback->ptsCount];
	if(wPts == NULL) return FALSE;

	const eint32 *pts = callback->pts;
	for(eint32 i = 0; i < callback->ptsCount; i++)
	{
		wPts[i].x = *pts++;
		wPts[i].y = *pts++;
	}

	int count = callback->ptsCount;
	if(wPts[count - 1].x == wPts[0].x && wPts[count - 1].y == wPts[0].y) count--;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(win32Engine->PrepareContext(callback->pixmap, callback->dc, false, false) == false)
	{
		delete[] wPts;
		return FALSE;
	}

	BOOL status = Polygon(callback->pixmap->win32HDC, wPts, count);

	delete[] wPts;

	return (LRESULT)status;
}


LRESULT _etk_draw_epixmap(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_DRAW_EPIXMAP || callback->data == NULL ||
	   callback->dc == NULL || callback->dstDrawable == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	EWin32GraphicsWindow *win = e_cast_as(callback->dstDrawable, EWin32GraphicsWindow);
	EWin32GraphicsDrawable *pix = e_cast_as(callback->dstDrawable, EWin32GraphicsDrawable);
	const EPixmap *epixmap = (const EPixmap *)callback->data;

	if(win == NULL && pix == NULL)
	{
		ETK_WARNING("[GRAPHICS]: %s --- Invalid drawable.", __PRETTY_FUNCTION__);
		return FALSE;
	}

	void* bits = (void*)epixmap->Bits();
	BITMAPINFO bitsInfo;

	bzero(&bitsInfo, sizeof(BITMAPINFO));
	bitsInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitsInfo.bmiHeader.biWidth = (LONG)epixmap->Bounds().Width() + 1;
	bitsInfo.bmiHeader.biHeight = -((LONG)epixmap->Bounds().Height() + 1);
	bitsInfo.bmiHeader.biPlanes = 1;

	switch(epixmap->ColorSpace())
	{
		case E_RGB24_BIG:
			if((bits = malloc((size_t)epixmap->BitsLength())) != NULL)
			{
				euint8 *tmp = (euint8*)bits;
				const euint8 *src = (const euint8*)epixmap->Bits();
				for(euint32 i = 0; i < epixmap->BitsLength(); i += 3, tmp += 3, src += 3)
				{
					tmp[0] = src[2];
					tmp[1] = src[1];
					tmp[2] = src[0];
				}
				bitsInfo.bmiHeader.biBitCount = 24;
				bitsInfo.bmiHeader.biSizeImage = (DWORD)epixmap->BitsLength();
			}
			break;

		case E_RGB24:
			bits = (void*)epixmap->Bits();
			bitsInfo.bmiHeader.biBitCount = 24;
			bitsInfo.bmiHeader.biSizeImage = (DWORD)epixmap->BitsLength();
			break;

		case E_RGB32:
		case E_RGBA32:
			bits = (void*)epixmap->Bits();
			bitsInfo.bmiHeader.biBitCount = 32;
			bitsInfo.bmiHeader.biCompression = BI_RGB;
			bitsInfo.bmiHeader.biSizeImage = 0;
			break;

		default:
			break;
	}

	if(bits == NULL)
	{
		ETK_WARNING("[GRAPHICS]: %s --- Unsupported color space (0x%x).", __PRETTY_FUNCTION__, epixmap->ColorSpace());
		return FALSE;
	}

	LRESULT retVal = FALSE;

	if(!(win == NULL || win->win32Window == NULL))
	{
		HDC hdcWin = GetDC(win->win32Window);

		HRGN hrgn = win32Engine->ConvertRegion(callback->dc->Clipping());
		if(hrgn != NULL)
		{
			SelectClipRgn(hdcWin, hrgn);
			SetStretchBltMode(hdcWin, COLORONCOLOR);
			StretchDIBits(hdcWin, callback->wx, callback->wy, (int)callback->ww + 1, (int)callback->wh + 1,
				      callback->x, callback->y, (int)callback->w + 1, (int)callback->h + 1,
				      bits, &bitsInfo, DIB_RGB_COLORS, SRCCOPY);
			SelectClipRgn(hdcWin, NULL);
			DeleteObject(hrgn);
		}

		ReleaseDC(win->win32Window, hdcWin);

		retVal = TRUE;
	}
	else if(!(pix == NULL || pix->win32HDC == NULL))
	{
		HRGN hrgn = win32Engine->ConvertRegion(callback->dc->Clipping());
		if(hrgn != NULL)
		{
			SelectClipRgn(pix->win32HDC, hrgn);
			SetStretchBltMode(pix->win32HDC, COLORONCOLOR);
			StretchDIBits(pix->win32HDC, callback->wx, callback->wy, (int)callback->ww + 1, (int)callback->wh + 1,
				      callback->x, callback->y, (int)callback->w + 1, (int)callback->h + 1,
				      bits, &bitsInfo, DIB_RGB_COLORS, SRCCOPY);
			SelectClipRgn(pix->win32HDC, NULL);
			DeleteObject(hrgn);
		}

		retVal = TRUE;
	}

	if(bits != (void*)epixmap->Bits()) free(bits);

	return retVal;
}

