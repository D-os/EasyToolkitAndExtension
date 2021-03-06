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
 * File: etk-pixmap.cpp
 *
 * --------------------------------------------------------------------------*/

#include "etk-win32gdi.h"

#include <etk/support/Autolock.h>
#include <etk/support/ClassInfo.h>


EWin32GraphicsDrawable::EWin32GraphicsDrawable(EWin32GraphicsEngine *win32Engine, euint32 w, euint32 h)
	: EGraphicsDrawable(), win32Pixmap(NULL), win32HDC(NULL), win32Pen(NULL), win32Brush(NULL),
	  fRequestAsyncWin(NULL), WM_ETK_MESSAGE(0)
{
	if(w == E_MAXUINT32 || h == E_MAXUINT32)
	{
		ETK_DEBUG("[GRAPHICS]: %s --- Either width or height is so large.", __PRETTY_FUNCTION__);
		return;
	}

	if(win32Engine == NULL) return;

	do {
		EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
		if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK ||
		   win32Engine->WM_ETK_MESSAGE == 0 || win32Engine->win32RequestAsyncWin == NULL)
		{
			ETK_WARNING("[GRAPHICS]: %s --- Invalid engine or unable to duplicate handle.", __PRETTY_FUNCTION__);
			return;
		}

		WM_ETK_MESSAGE = win32Engine->WM_ETK_MESSAGE;
		fRequestAsyncWin = win32Engine->win32RequestAsyncWin;
	} while(false);

	e_rgb_color whiteColor = {255, 255, 255, 255};
	EGraphicsDrawable::SetBackgroundColor(whiteColor);

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_CREATE_PIXMAP;
	callback.pixmap = this;
	callback.w = w;
	callback.h = h;
	callback.bkColor = whiteColor;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_PIXMAP, (LPARAM)&callback) == (LRESULT)TRUE);

	if(!successed || win32Pixmap == NULL || win32HDC == NULL)
	{
		ETK_DEBUG("[GRAPHICS]: %s --- Create pixmap failed: \"%s\".",
			  __PRETTY_FUNCTION__, successed ? "win32Pixmap = NULL || win32HDC = NULL" : "SendMessageA failed");
	}
}


LRESULT _etk_create_pixmap(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_CREATE_PIXMAP || callback->pixmap == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	int w = (int)callback->w + 1;
	int h = (int)callback->h + 1;

	HDC hdc = CreateCompatibleDC(win32Engine->win32ScreenHDC);
	if(hdc == NULL) return FALSE;

	if((callback->pixmap->win32Pixmap = CreateCompatibleBitmap(win32Engine->win32ScreenHDC, w, h)) == NULL ||
	   DeleteObject(SelectObject(hdc, callback->pixmap->win32Pixmap)) == 0)
	{
		if(callback->pixmap->win32Pixmap)
		{
			DeleteObject(callback->pixmap->win32Pixmap);
			callback->pixmap->win32Pixmap = NULL;
		}
		DeleteDC(hdc);
		return FALSE;
	}

	callback->pixmap->win32HDC = hdc;

	HBRUSH hbrush = CreateSolidBrush(RGB(callback->bkColor.red, callback->bkColor.green, callback->bkColor.blue));
	HRGN hrgn = CreateRectRgn(0, 0, w, h);
	if(hbrush != NULL && hrgn != NULL) FillRgn(hdc, hrgn, hbrush);
	if(hbrush != NULL) DeleteObject(hbrush);
	if(hrgn != NULL) DeleteObject(hrgn);

	return TRUE;
}


EWin32GraphicsDrawable::~EWin32GraphicsDrawable()
{
	if(fRequestAsyncWin != NULL)
	{
		etk_win32_gdi_callback_t callback;
		callback.command = WM_ETK_MESSAGE_DESTROY_PIXMAP;
		callback.pixmap = this;

		bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
					       WM_ETK_MESSAGE_PIXMAP, (LPARAM)&callback) == (LRESULT)TRUE);

		if(!successed || win32Pixmap != NULL || win32HDC != NULL || win32Pen != NULL || win32Brush != NULL)
			ETK_ERROR("[GRAPHICS]: %s --- Unable to destroy pixmap.", __PRETTY_FUNCTION__);
	}
}


LRESULT _etk_destroy_pixmap(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_DESTROY_PIXMAP || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	if(callback->pixmap->win32Pen != NULL) DeleteObject(callback->pixmap->win32Pen);
	if(callback->pixmap->win32Brush != NULL) DeleteObject(callback->pixmap->win32Brush);

	callback->pixmap->win32Pen = NULL;
	callback->pixmap->win32Brush = NULL;

	DeleteObject(callback->pixmap->win32Pixmap);
	callback->pixmap->win32Pixmap = NULL;

	DeleteDC(callback->pixmap->win32HDC);
	callback->pixmap->win32HDC = NULL;

	return TRUE;
}


e_status_t
EWin32GraphicsDrawable::SetBackgroundColor(e_rgb_color bkColor)
{
	if(fRequestAsyncWin == NULL) return E_ERROR;

	EGraphicsDrawable::SetBackgroundColor(bkColor);

	// TODO: clear content

	return E_OK;
}


e_status_t
EWin32GraphicsDrawable::ResizeTo(euint32 w, euint32 h)
{
	if(fRequestAsyncWin == NULL) return E_ERROR;

	if(w == E_MAXUINT32 || h == E_MAXUINT32)
	{
		ETK_DEBUG("[GRAPHICS]: %s --- Either width or height is so large.", __PRETTY_FUNCTION__);
		return E_ERROR;
	}

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_RESIZE_PIXMAP;
	callback.pixmap = this;
	callback.w = w;
	callback.h = h;
	callback.bkColor = BackgroundColor();

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_PIXMAP, (LPARAM)&callback) == (LRESULT)TRUE);

	if(successed && win32Pixmap != NULL) return E_OK;

	ETK_DEBUG("[GRAPHICS]: %s --- Unable to resize pixmap.", __PRETTY_FUNCTION__);

	return E_ERROR;
}


LRESULT _etk_resize_pixmap(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_RESIZE_PIXMAP || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	int w = (int)callback->w + 1;
	int h = (int)callback->h + 1;

	HBITMAP newPixmap = CreateCompatibleBitmap(win32Engine->win32ScreenHDC, w, h);
	if(newPixmap == NULL) return FALSE;

	HDC hdc = CreateCompatibleDC(win32Engine->win32ScreenHDC);
	if(hdc != NULL)
	{
		HGDIOBJ oldHbm = SelectObject(hdc, newPixmap);

		HBRUSH hbrush = CreateSolidBrush(RGB(callback->bkColor.red, callback->bkColor.green, callback->bkColor.blue));
		HRGN hrgn = CreateRectRgn(0, 0, w, h);
		if(hbrush != NULL && hrgn != NULL) FillRgn(hdc, hrgn, hbrush);
		if(hbrush != NULL) DeleteObject(hbrush);
		if(hrgn != NULL) DeleteObject(hrgn);

		BitBlt(hdc, 0, 0, w, h, callback->pixmap->win32HDC, 0, 0, SRCCOPY);

		SelectObject(hdc, oldHbm);
		DeleteDC(hdc);
	}

	if(SelectObject(callback->pixmap->win32HDC, newPixmap) == 0)
	{
		DeleteObject(newPixmap);
		return FALSE;
	}

	DeleteObject(callback->pixmap->win32Pixmap);
	callback->pixmap->win32Pixmap = newPixmap;

	return TRUE;
}


e_status_t
EWin32GraphicsDrawable::CopyTo(EGraphicsContext *dc,
			       EGraphicsDrawable *dstDrawable,
			       eint32 x, eint32 y, euint32 w, euint32 h,
			       eint32 dstX, eint32 dstY, euint32 dstW, euint32 dstH)
{
	if(fRequestAsyncWin == NULL || dc == NULL || dstDrawable == NULL) return E_ERROR;

	if(dc->DrawingMode() != E_OP_COPY)
	{
		// TODO
		ETK_DEBUG("[GRAPHICS]: %s --- FIXME: unsupported drawing mode.", __PRETTY_FUNCTION__);
		return E_ERROR;
	}

	if(w == E_MAXUINT32 || h == E_MAXUINT32 || dstW == E_MAXUINT32 || dstH == E_MAXUINT32)
	{
		ETK_DEBUG("[GRAPHICS]: %s --- Either width or height is so large.", __PRETTY_FUNCTION__);
		return E_ERROR;
	}

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_DRAW_PIXMAP;
	callback.pixmap = this;
	callback.dc = dc;
	callback.dstDrawable = dstDrawable;
	callback.x = x;
	callback.y = y;
	callback.w = w;
	callback.h = h;

	callback.wx = dstX;
	callback.wy = dstY;
	callback.ww = dstW;
	callback.wh = dstH;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_PIXMAP, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}


LRESULT _etk_draw_pixmap(EWin32GraphicsEngine *win32Engine, etk_win32_gdi_callback_t *callback)
{
	if(win32Engine == NULL || callback == NULL ||
	   callback->command != WM_ETK_MESSAGE_DRAW_PIXMAP || callback->pixmap == NULL ||
	   callback->pixmap->win32Pixmap == NULL || callback->pixmap->win32HDC == NULL ||
	   callback->dc == NULL || callback->dstDrawable == NULL) return FALSE;

	EAutolock <EWin32GraphicsEngine> autolock(win32Engine);
	if(autolock.IsLocked() == false || win32Engine->InitCheck() != E_OK) return FALSE;

	EWin32GraphicsWindow *win = e_cast_as(callback->dstDrawable, EWin32GraphicsWindow);
	EWin32GraphicsDrawable *pix = e_cast_as(callback->dstDrawable, EWin32GraphicsDrawable);
	if(win == NULL && pix == NULL)
	{
		ETK_WARNING("[GRAPHICS]: %s --- Invalid drawable.", __PRETTY_FUNCTION__);
		return FALSE;
	}

	if(win != NULL)
	{
		if(win->win32Window == NULL) return FALSE;

#if 0
		eint8 otherThread = (GetCurrentThreadId() == win32Engine->win32ThreadID ? 0 : 1);
		if(otherThread == 1)
			otherThread = (AttachThreadInput(win32Engine->win32ThreadID, GetCurrentThreadId(), TRUE) == 0 ? 2 : 1);
		if(otherThread > 1) return FALSE;
#endif

		HDC hdcWin = GetDC(win->win32Window);

		HRGN hrgn = win32Engine->ConvertRegion(callback->dc->Clipping());
		SelectClipRgn(hdcWin, hrgn);

		if(callback->w == callback->ww && callback->h == callback->wh)
		{
			BitBlt(hdcWin, callback->wx, callback->wy, (int)callback->ww + 1, (int)callback->wh + 1,
			       callback->pixmap->win32HDC, callback->x, callback->y, SRCCOPY);
		}
		else
		{
			SetStretchBltMode(hdcWin, COLORONCOLOR);
			StretchBlt(hdcWin, callback->wx, callback->wy, (int)callback->ww + 1, (int)callback->wh + 1,
				   callback->pixmap->win32HDC,
				   callback->x, callback->y, (int)callback->w + 1, (int)callback->h + 1, SRCCOPY);
		}

		if(hrgn)
		{
			SelectClipRgn(hdcWin, NULL);
			DeleteObject(hrgn);
		}

		ReleaseDC(win->win32Window, hdcWin);

#if 0
		if(otherThread == 1) AttachThreadInput(win32Engine->win32ThreadID, GetCurrentThreadId(), FALSE);
#endif
	}
	else
	{
		if(pix->win32HDC == NULL) return FALSE;

		HRGN hrgn = win32Engine->ConvertRegion(callback->dc->Clipping());
		SelectClipRgn(pix->win32HDC, hrgn);

		if(callback->w == callback->ww && callback->h == callback->wh)
		{
			BitBlt(pix->win32HDC, callback->wx, callback->wy, (int)callback->ww + 1, (int)callback->wh + 1,
			       callback->pixmap->win32HDC, callback->x, callback->y, SRCCOPY);
		}
		else
		{
			SetStretchBltMode(pix->win32HDC, COLORONCOLOR);
			StretchBlt(pix->win32HDC, callback->wx, callback->wy, (int)callback->ww + 1, (int)callback->wh + 1,
				   callback->pixmap->win32HDC,
				   callback->x, callback->y, (int)callback->w + 1, (int)callback->h + 1, SRCCOPY);
		}

		if(hrgn)
		{
			SelectClipRgn(pix->win32HDC, NULL);
			DeleteObject(hrgn);
		}
	}

	return TRUE;
}


e_status_t
EWin32GraphicsDrawable::DrawPixmap(EGraphicsContext *dc, const EPixmap *pix,
				   eint32 x, eint32 y, euint32 w, euint32 h,
				   eint32 dstX, eint32 dstY, euint32 dstW, euint32 dstH)
{
	if(fRequestAsyncWin == NULL) return E_ERROR;

	if(w == E_MAXUINT32 || h == E_MAXUINT32 || dstW == E_MAXUINT32 || dstH == E_MAXUINT32)
	{
		ETK_DEBUG("[GRAPHICS]: %s --- Either width or height is so large.", __PRETTY_FUNCTION__);
		return E_ERROR;
	}

	etk_win32_gdi_callback_t callback;
	callback.command = WM_ETK_MESSAGE_DRAW_EPIXMAP;
	callback.dc = dc;
	callback.dstDrawable = this;
	callback.x = x;
	callback.y = y;
	callback.w = w;
	callback.h = h;

	callback.wx = dstX;
	callback.wy = dstY;
	callback.ww = dstW;
	callback.wh = dstH;

	callback.data = (const void*)pix;

	bool successed = (SendMessageA(fRequestAsyncWin, WM_ETK_MESSAGE,
				       WM_ETK_MESSAGE_PIXMAP, (LPARAM)&callback) == (LRESULT)TRUE);
	return(successed ? E_OK : E_ERROR);
}

