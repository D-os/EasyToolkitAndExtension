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
 * File: MenuField.h
 * Description: EMenuField --- display a labeled pop-up menu
 * 
 * --------------------------------------------------------------------------*/

#ifndef __ETK_MENU_FIELD_H__
#define __ETK_MENU_FIELD_H__

#include <etk/interface/MenuBar.h>

#ifdef __cplusplus /* Just for C++ */

class _IMPEXP_ETK EMenuField : public EView {
public:
	EMenuField(ERect frame,
		   const char *name,
		   const char *label,
		   EMenu *menu,
		   bool fixedSize = false,
		   euint32 resizeMode = E_FOLLOW_LEFT | E_FOLLOW_TOP,
		   euint32 flags = E_WILL_DRAW | E_NAVIGABLE | E_FRAME_EVENTS);
	virtual ~EMenuField();

	virtual void	SetLabel(const char *label);
	const char	*Label() const;

	virtual void	SetAlignment(e_alignment alignment);
	e_alignment	Alignment() const;

	virtual void	SetDivider(float divider);
	float		Divider() const;

	// SetMenu: the old menu will be destructed automatically.
	virtual bool	SetMenu(EMenu *menu);
	EMenu		*Menu() const;

	EMenuBar	*MenuBar() const;
	EMenuItem	*MenuItem() const;

	virtual void	Draw(ERect updateRect);
	virtual void	GetPreferredSize(float *width, float *height);
	virtual void	FrameMoved(EPoint new_position);
	virtual void	FrameResized(float new_width, float new_height);
	virtual void	WindowActivated(bool state);
	virtual void	MakeFocus(bool focusState = true);
	virtual void	SetFont(const EFont *font, euint8 mask = E_FONT_ALL);
	virtual void	MouseDown(EPoint where);
	virtual void	MouseUp(EPoint where);
	virtual void	MouseMoved(EPoint where, euint32 code, const EMessage *a_message);
	virtual void	KeyDown(const char *bytes, eint32 numBytes);
	virtual void	KeyUp(const char *bytes, eint32 numBytes);

protected:
	virtual void	ChildRemoving(EView *child);

private:
	bool fFixedSize;
	e_alignment fAlignment;
	float fDivider;

	char *fLabel;
	EMenuBar *fMenuBar;
	EMenu *fMenu;
};

#endif /* __cplusplus */

#endif /* __ETK_MENU_FIELD_H__ */

