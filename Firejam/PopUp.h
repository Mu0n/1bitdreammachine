#ifndef __POPUP_H__
#define __POPUP_H__

#define kPopUpStyleText    0x01 // draw current item text
#define kPopUpStyleCaret   0x02 // draw disclosure triangle
#define kPopUpStyleCheck   0x04 // check current item in menu
#define kPopUpStyleDefault (kPopUpStyleText | kPopUpStyleCaret | kPopUpStyleCheck)

typedef struct PopUpRec
{
	WindowPtr window;
	MenuHandle menu;
	Rect bounds;
	short item;
	short style;
} PopUpRec, *PopUpPtr, **PopUpHandle;

// Returns a newly-allocated pointer to a PopUpRec
PopUpPtr CreatePopUp(WindowPtr window, Rect *bounds, MenuHandle menu, short style);

// Frees memory
void DisposePopUp(PopUpPtr popup);

// Paints the control frame, text, and other elements. Must be called
// during the window update cycle, between BeginUpdate() and EndUpdate()
void DrawPopUp(PopUpPtr popup);

// Hook to handle mouse-down events that trigger the menu. Must be called
// in the main event loop. Returns true if the event was handled.
Boolean HandlePopUpEvent(PopUpPtr popup, EventRecord *eventPtr);

// Changes the currently-selected item index and repaints the control
void SelectPopUpItem(PopUpPtr popup, short item);

// Library use only
void _DrawPopUpItem(PopUpPtr popup);
Boolean _HandlePopUpMouseDownInContent(PopUpPtr popup, EventRecord *eventPtr);
#endif