/* PrepTools.h */#ifndef __PREPTOOLS__#define __PREPTOOLS__#define kBaseResID 128#define kMoveToFront  (WindowPtr)-1L#define kRandomUpperLimit 32768#define kScrollBarAdjust  (16-1)#define kScrollBarWidth   16#define kScreenWidth (qd.screenBits.bounds.right-qd.screenBits.bounds.left)#define kScreenHeight (qd.screenBits.bounds.bottom-qd.screenBits.bounds.top)#define kMacPlusHeight 342#define kMacPlusWidth  512#define kMenuBarHeight 20#define MAX(A,B) (A>B?A:B)#define MIN(A,B) (A>B?B:A)#define SQUARE(A) A*A#define ABS(A) (A<0?-A:A)/*------ Constants -------*//* Part 1: Used in menu bar hiding*/static short     gs_dyMBar   = 0;static RgnHandle gs_hrgnMBar = 0;/* Part 2: *//*------ End of Constants -------*/void InitMacStuff(void);void HideMyMenuBar(void);void ShowMyMenuBar(void);unsigned short rangeRandom( unsigned short min, unsigned short max );WindowPtr WholeScreen(Pattern thePat, Boolean wantMenus);WindowPtr MacPlusArea(Pattern borderPat, Pattern middlePat, Boolean wantMenus, WindowRecord *winRec);WindowPtr ForceMacPlusScreen(Pattern thePath, Boolean wantMenus);void CLS(Pattern thePat, Boolean wantMenus);short OldLoadFile(SFTypeList myTypes); // Must FSClose the short it returns when finished with the fileint GetScreenWidth(void);int GetScreenHeight(void);void PStringCat(Str255 a, Str255 b);void PStringCopy(Str255 a, Str255 b);#endif