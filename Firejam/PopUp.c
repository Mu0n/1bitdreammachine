#ifndef POPUP_C
#define POPUP_C

#include "PopUp.h"

PopUpPtr CreatePopUp(WindowPtr window, Rect *bounds, MenuHandle menu, short style)
{
	PopUpPtr popup;
	
	popup = (PopUpPtr)NewPtr(sizeof(PopUpRec));
	if (popup == nil)
		return nil;
	
	//InsertMenu(menu, -1L);
	
	popup->menu = menu;
	popup->window = window;
	popup->item = 1;
	popup->bounds = *bounds;
	popup->style = style;
	
	
	
	InsertMenu(popup->menu, -1L);
	
	
	return popup;
}

void DisposePopUp(PopUpPtr popup)
{
	if (popup == nil)
		return;
		
	DisposPtr((Ptr)popup);
}

void DrawPopUp(PopUpPtr popup)
{
	GrafPtr prevPort;
	
	GetPort(&prevPort);
	SetPort(popup->window);
	
	// draw border
	FrameRect(&popup->bounds);
	
	// draw shadow
	MoveTo(popup->bounds.left + 1, popup->bounds.bottom);
	LineTo(popup->bounds.right, popup->bounds.bottom);
	LineTo(popup->bounds.right, popup->bounds.top + 1);
	
	// draw text and triangle
	_DrawPopUpItem(popup);
	
	SetPort(prevPort);
}

void _DrawPopUpItem(PopUpPtr popup)
{
	Str255 menuItem;
	Rect bgRect;
	RgnHandle prevClip;
	
	short count;
	
	PolyHandle triangle;
	Point pt1, pt2, pt3;
	
	if (popup->style & kPopUpStyleText)
	{
		// determine text
		GetItem(popup->menu, popup->item, menuItem);
		
		// fill bg with white
		bgRect = popup->bounds;
		InsetRect(&bgRect, 1, 1);
		FillRect(&bgRect, white);
		
		// draw item text
		MoveTo(
			popup->bounds.left + 5, // left margin
			popup->bounds.bottom - 5 // bottom margin
		);
		TextFont(0);
		DrawString(menuItem);
	}
	
	if (popup->style & kPopUpStyleCaret)
	{
		// draw disclosure triangle
		pt1.h = popup->bounds.right - 17;
		pt1.v = popup->bounds.top + 6;
		
		pt2.h = popup->bounds.right - 5;
		pt2.v = pt1.v;
		
		pt3.h = popup->bounds.right - 11;
		pt3.v = popup->bounds.top + 13;
		
		triangle = OpenPoly();
		MoveTo(pt1.h, pt1.v);
		LineTo(pt2.h, pt2.v);
		LineTo(pt3.h, pt3.v);
		LineTo(pt1.h, pt1.v);
		ClosePoly();
		
		FillPoly(triangle, black);
		KillPoly(triangle);
	}
}

Boolean HandlePopUpEvent(PopUpPtr popup, EventRecord *eventPtr)
{
	WindowPtr window;
	long thePart;
	long menuChoice;
	
	if (eventPtr->what != mouseDown)
		return false;
	
	thePart = FindWindow(eventPtr->where, &window);
	
	if (!(window == popup->window && thePart == inContent))
		return false;
	
	return _HandlePopUpMouseDownInContent(popup, eventPtr);
}

Boolean _HandlePopUpMouseDownInContent(PopUpPtr popup, EventRecord *eventPtr)
{
	Point eventPoint, popUpPoint;
	short choice;
	Rect invertBounds;
	GrafPtr prevPort;
	
	eventPoint = eventPtr->where;
	GlobalToLocal(&eventPoint);
	
	if (PtInRect(eventPoint, &popup->bounds))
	{
		GetPort(&prevPort);
		SetPort(popup->window);
	
		invertBounds = popup->bounds;
		InsetRect(&invertBounds, 1, 1);
	
		popUpPoint.v = popup->bounds.top + 1;
		popUpPoint.h = popup->bounds.left + 1;
		LocalToGlobal(&popUpPoint);
		
		InvertRect(&invertBounds);
		
		if (popup->style & kPopUpStyleCheck)
			CheckItem(popup->menu, popup->item, true);
		
		choice = PopUpMenuSelect(
			popup->menu,
			popUpPoint.v,
			popUpPoint.h,
			popup->item
		);
		
		InvertRect(&invertBounds);
		
		if (popup->style & kPopUpStyleCheck)
			CheckItem(popup->menu, popup->item, false);
		
		SetPort(prevPort);
		
		if (LoWord(choice) > 0)
		{
			SelectPopUpItem(popup, LoWord(choice));
			return true;
		}
	}
	
	return false;
}

void SelectPopUpItem(PopUpPtr popup, short item)
{
	GrafPtr prevPort;
	
	GetPort(&prevPort);
	SetPort(popup->window);
	
	popup->item = item;
	_DrawPopUpItem(popup);
	
	SetPort(prevPort);
}

#endif