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
 * File: RadioButton.cpp
 * Description: ERadioButton --- Radio buttons within their parent only one be on
 * 
 * --------------------------------------------------------------------------*/

#include <etk/support/ClassInfo.h>
#include <etk/interface/Window.h>

#include "RadioButton.h"


ERadioButton::ERadioButton(ERect frame, const char *name, const char *label,
			   EMessage *message, euint32 resizeMode, euint32 flags)
	: EControl(frame, name, label, message, resizeMode, flags)
{
}


ERadioButton::~ERadioButton()
{
}


void
ERadioButton::SetLabel(const char *label)
{
	EControl::SetLabel(label);
	Invalidate();
}


void
ERadioButton::WindowActivated(bool state)
{
	Invalidate();
}


void
ERadioButton::Draw(ERect updateRect)
{
	if(Window() == NULL) return;

	EFont font;
	GetFont(&font);
	e_font_height fontHeight;
	font.GetHeight(&fontHeight);
	float sHeight = fontHeight.ascent + fontHeight.descent;

	ERect rect = Frame().OffsetToSelf(E_ORIGIN);
	rect.InsetBy(5, (rect.Height() - sHeight) / 2);
	if(rect.IsValid() == false) return;

	if((IsFocus() || IsFocusChanging()) && IsEnabled() && Label() != NULL)
	{
		EPoint penLocation;
		penLocation.Set(rect.left + rect.Height() + 5, rect.Center().y + sHeight / 2 + 1);

		SetHighColor((IsFocus() && Window()->IsActivate()) ? e_ui_color(E_NAVIGATION_BASE_COLOR) : ViewColor());
		StrokeLine(penLocation, penLocation + EPoint(font.StringWidth(Label()), 0));
	}

	if(IsFocusChanging()) return;

	e_rgb_color shineColor = e_ui_color(E_SHINE_COLOR);
	e_rgb_color shadowColor = e_ui_color(E_SHADOW_COLOR);

	if(!IsEnabled())
	{
		shineColor.disable(ViewColor());
		shadowColor.disable(ViewColor());
	}

	rect.right = rect.left + rect.Height();
	SetHighColor(shineColor.mix_copy(0, 0, 0, 5));
	FillEllipse(rect);
	SetHighColor(shineColor);
	StrokeArc(rect, 225, 180);
	SetHighColor(shadowColor);
	StrokeArc(rect, 45, 180);

	if(Value() == E_CONTROL_ON)
	{
		SetHighColor(shadowColor.mix_copy(255, 255, 255, 50));
		ERect r = rect.InsetByCopy(3, 3);
		FillEllipse(r);
	}

	if(Label() != NULL)
	{
		EPoint penLocation;
		penLocation.x = rect.right + 5;
		penLocation.y = rect.Center().y - sHeight / 2.f;
		penLocation.y += fontHeight.ascent + 1;

		SetHighColor(IsEnabled() ? e_ui_color(E_PANEL_TEXT_COLOR) : e_ui_color(E_SHINE_COLOR).disable(ViewColor()));
		SetLowColor(ViewColor());

		DrawString(Label(), penLocation);
		if(!IsEnabled())
		{
			SetHighColor(e_ui_color(E_SHADOW_COLOR).disable(ViewColor()));
			DrawString(Label(), penLocation - EPoint(1, 1));
		}
	}
}

void
ERadioButton::MouseDown(EPoint where)
{
	if(!IsEnabled() || !QueryCurrentMouse(true, E_PRIMARY_MOUSE_BUTTON)) return;

#if 0
	e_font_height fontHeight;
	GetFontHeight(&fontHeight);
	float sHeight = fontHeight.ascent + fontHeight.descent;

	ERect rect = Frame().OffsetToSelf(E_ORIGIN);
	rect.InsetBy(5, (rect.Height() - sHeight) / 2);
	if(rect.IsValid() == false) return;
	rect.right = rect.left + rect.Height();
	if(rect.Contains(where) == false) return;
#endif

	if((Flags() & E_NAVIGABLE) && !IsFocus()) MakeFocus();

//	SetValue((Value() == E_CONTROL_ON) ? E_CONTROL_OFF : E_CONTROL_ON);

	if(Value() == E_CONTROL_ON) return;
	SetValue(E_CONTROL_ON);

	Invoke();
}


void
ERadioButton::KeyDown(const char *bytes, eint32 numBytes)
{
	if(!IsEnabled() || !IsFocus() || numBytes != 1) return;
	if(!(bytes[0] == E_ENTER || bytes[0] == E_SPACE)) return;

//	SetValue((Value() == E_CONTROL_ON) ? E_CONTROL_OFF : E_CONTROL_ON);

	if(Value() == E_CONTROL_ON) return;
	SetValue(E_CONTROL_ON);

	Invoke();
}


void
ERadioButton::SetFont(const EFont *font, euint8 mask)
{
	EControl::SetFont(font, mask);
	Invalidate();
}


void
ERadioButton::GetPreferredSize(float *width, float *height)
{
	if(width == NULL && height == NULL) return;

	EFont font;
	GetFont(&font);
	e_font_height fontHeight;
	font.GetHeight(&fontHeight);
	float sHeight = fontHeight.ascent + fontHeight.descent;

	if(width) *width = font.StringWidth(Label()) + sHeight + 20;
	if(height) *height = sHeight + 4;
}


void
ERadioButton::SetValue(eint32 value)
{
	if(value != Value())
	{
		EControl::SetValueNoUpdate(value);
		Invalidate();

		if(value == E_CONTROL_ON)
		{
			for(EView *sibling = NextSibling(); sibling != NULL; sibling = sibling->NextSibling())
			{
				ERadioButton *rbtn = e_cast_as(sibling, ERadioButton);
				if(rbtn != NULL) rbtn->SetValue(E_CONTROL_OFF);
			}
			for(EView *sibling = PreviousSibling(); sibling != NULL; sibling = sibling->PreviousSibling())
			{
				ERadioButton *rbtn = e_cast_as(sibling, ERadioButton);
				if(rbtn != NULL) rbtn->SetValue(E_CONTROL_OFF);
			}
		}
	}
}

