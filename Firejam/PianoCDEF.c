#pragma segment PianoCDEF

#include <Types.h>
#include <QuickDraw.h>
#include <Controls.h>

#define NB_ACTI_NOTES 88

typedef struct {
	Boolean noteOn[128];
	Pattern blackPat;
	Pattern whitePat;
	Pattern grayPat;
	Pattern actBlackPat;
	Pattern actWhitePat;
	Pattern actWhitePat2;
	Pattern actWhitePat3;
	Pattern actWhitePat4;
	RgnHandle noteRgn[88];
} PianoStates;

pascal long main(short, ControlHandle, short, long);

void DoDrawControl(ControlHandle, short);
void DoDrawOneKey(ControlHandle, short);
void initNoteRgn(ControlHandle);
long DoTestControl(ControlHandle c, long);
int  identifyNoteFromPoint(Point);
void getNoteRect(ControlHandle, short, Rect *);
short GetWhiteSlot(short);
short isWhite(short);

pascal long main(short varCode, ControlHandle c, short message, long param)
{
long result = 0;
int i;
Rect r;
Point pt;
Rect  kRect;
PianoStates **ps;

//for(i=0;i<message;i++) FlashMenuBar(0);
switch(message)
	{
	case drawCntl:
		DoDrawControl(c, (short)(param&0x0000FFFF));
		break;
	
	case testCntl:
		//result = DoTestControl(c, param);
		pt.v = (short)((param>>16) & 0x0000FFFF);
		pt.h = (short)(param & 0x0000FFFF);
		
		ps = (PianoStates**)(*c)->contrlData;
		for(i=0;i<NB_ACTI_NOTES;i++)
			{
			if(PtInRgn(pt,(*ps)->noteRgn[i]))
				{
				return (128+i); //a part code is returned	
				}
			}
		break;
		
	case calcCRgns:
		RectRgn((RgnHandle)param, &((**c).contrlRect));
		break;
	
	case initCntl:
		result = 0;
		ps = (PianoStates **) NewHandleClear(sizeof(PianoStates));
		(*c)->contrlData = (Handle)ps;
		
		for(i=0;i<8;i++) 
			{
			(*ps)->blackPat[i]=0xFF;
			(*ps)->whitePat[i]=0x00;
			}
		for(i=0;i<8;i+=2)
			{
			(*ps)->grayPat[i]=0xAA;
			(*ps)->grayPat[i+1]=0x55;
			
			(*ps)->actBlackPat[i]=0x77;
			(*ps)->actBlackPat[i+1]=0xDD;
			
			(*ps)->actWhitePat[i]=0x88;
			(*ps)->actWhitePat[i+1]=0x22;
			
			(*ps)->actWhitePat2[i]=0x44;
			(*ps)->actWhitePat2[i+1]=0x11;
			
			(*ps)->actWhitePat3[i]=0x22;
			(*ps)->actWhitePat3[i+1]=0x88;
			
			(*ps)->actWhitePat4[i]=0x11;
			(*ps)->actWhitePat4[i+1]=0x44;
			}
		for(i=0;i<NB_ACTI_NOTES;i++)
			{
			(*ps)->noteRgn[i] = NewRgn();
			}
		initNoteRgn(c);
		break;
	
	case dispCntl:
		break;
		
	case posCntl:
		break;
		
	case thumbCntl:
		break;
		
	case dragCntl:
		break;
		
	case autoTrack:
		break;
	
	case 100:
		DoDrawOneKey(c, (short)(param&0x0000FFFF));
		break;
	default:
		return result;
	}

return result;
}

short GetWhiteSlot(short noteIndex)
{
short octave = noteIndex / 12;
short note = noteIndex % 12;
short offset;

switch(note)
	{
	case 0: offset = 0; break;
	case 1: offset = 0; break;
	case 2: offset = 1; break;
	case 3: offset = 2; break;
	case 4: offset = 2; break;
	case 5: offset = 3; break;
	case 6: offset = 3; break;
	case 7: offset = 4; break;
	case 8: offset = 5; break;
	case 9: offset = 5; break;
	case 10: offset = 6; break;
	case 11: offset = 6; break;
	default: offset = 0; break;
	}
return (octave * 7) + offset;
}

short isWhite(short noteIndex)
{
short note = noteIndex % 12;
if(note == 0 || note == 2 || note == 3 || note == 5 || note == 7 || note == 8 || note == 10) return 1;
return 0;
}
void getNoteRect(ControlHandle c, short noteIndex, Rect *r)
{
Rect pRect = (*c)->contrlRect;
short slot = GetWhiteSlot(noteIndex);

short xLeft = pRect.left + slot  * 9;
short xRight = xLeft + 10;

if(isWhite(noteIndex))
	{
	r->top = pRect.top;
	r->bottom = pRect.bottom;
	r->left = xLeft;
	r->right = xRight;
	}
else {
	r->top = pRect.top;
	r->bottom = pRect.bottom-30;
	r->left = xLeft+4;
	r->right = xRight+4;
}

}

void DoDrawControl(ControlHandle c, short partCode)
{ 
Rect kRect; //k: note to draw
short i;
PianoStates **ps;

PenNormal();

ps = (PianoStates**)(*c)->contrlData;
HLock((Handle)ps);
//cRect =  ((**c).contrlRect);

if(partCode == 0) //full drawing requested
	{
	//white notes
	for(i=0; i<NB_ACTI_NOTES; i++)
		{
		if(isWhite(i))
			{
			getNoteRect(c, i, &kRect);
			EraseRect(&kRect);
			FrameRect(&kRect);
			}
		}
		
	//black notes
	for(i=0; i<NB_ACTI_NOTES; i++)
		{
		if(!isWhite(i))
			{
			getNoteRect(c, i, &kRect);
			InsetRect(&kRect,2,0);
			EraseRect(&kRect);
			FrameRect(&kRect);
			FillRect(&kRect,(*ps)->blackPat);
			}
		}
	}
	
HUnlock((Handle)ps);
}

void initNoteRgn(ControlHandle c)
{
Rect kRect, cRect, bRect; //k: note to draw c:whole control b:neighbor black key
short i;
RgnHandle r1 = NewRgn();
RgnHandle r2 = NewRgn();
PianoStates **ps;
long oldA5 = SetCurrentA5();

ps = (PianoStates**)(*c)->contrlData;
HLock((Handle)ps);
for(i=0;i<NB_ACTI_NOTES;i++)
{
	getNoteRect(c, i, &kRect);
	
	RectRgn((*ps)->noteRgn[i], &kRect);

	InsetRgn((*ps)->noteRgn[i],1,1);
		if(i > 0) //black note before, redraw that
		{
		if(isWhite(i-1) == 0)
		{
		getNoteRect(c,i-1,&bRect);
		InsetRect(&bRect,2,0);
		RectRgn(r2,&bRect);
		DiffRgn((*ps)->noteRgn[i],r2,(*ps)->noteRgn[i]);
		}
		}
	if(i < 109) //black note after, redraw that
		{
		if(isWhite(i+1) == 0)
		{
		getNoteRect(c,i+1,&bRect);
		InsetRect(&bRect,2,0);
		RectRgn(r2,&bRect);
		DiffRgn((*ps)->noteRgn[i],r2,(*ps)->noteRgn[i]);
		}
		}
}	

for(i=0;i<NB_ACTI_NOTES;i++) MoveHHi((Handle) (*ps)->noteRgn[i]);

HUnlock((Handle)ps);
SetA5(oldA5);


}
void DoDrawOneKey(ControlHandle c, short noteIndex)
{ 
Rect kRect, cRect, bRect; //k: note to draw c:whole control b:neighbor black key
short i;
RgnHandle r1 = NewRgn();
RgnHandle r2 = NewRgn();
PianoStates **ps;
short isHigh;
long oldA5 = SetCurrentA5();
short index;

PenNormal();
ps = (PianoStates**)(*c)->contrlData;
HLock((Handle)ps);
isHigh = (*ps)->noteOn[noteIndex];
index = noteIndex-21;
    
r1 = (*ps)->noteRgn[index];

if(isHigh) 
	{
	if(isWhite(index)) 
		{ //crazy scheme to align the phases of the patterns regardless of white note position
		switch((*r1)->rgnBBox.left % 4)
			{
			case 0:
				FillRgn(r1,(*ps)->actWhitePat);
				break;
			case 1:
				FillRgn(r1,(*ps)->actWhitePat2);
				break;
			case 2:
				FillRgn(r1,(*ps)->actWhitePat3);
				break;
			case 3:
				FillRgn(r1,(*ps)->actWhitePat4);
				break;
			}
		}
	else 
		{
		InsetRgn(r1,2,0);
		FillRgn(r1,(*ps)->grayPat);
		InsetRgn(r1,-2,0);
		}
	}
else 
	{
	if(isWhite(index)) FillRgn(r1,(*ps)->whitePat);
	else 
		{
		InsetRgn(r1,2,0);
		FillRgn(r1,(*ps)->blackPat);	
		InsetRgn(r1,-2,0);
		}
	}	
HUnlock((Handle)ps);
SetA5(oldA5);

}


long DoTestControl(ControlHandle c, long param)
{
int i;
Rect r;
Point pt;

pt.v = (short)(param>>16);
pt.h = (short)(param & 0xFFFF);

for(i=0; i<NB_ACTI_NOTES; i++) //white keys
{
	getNoteRect(c, i, &r);
	if(PtInRect(pt, &r))
		{
		return 128+i;
		}
	
}
return 0;
}

