/****************************//*        EVENTS            *//* By Brian Greenstone      *//****************************//***************//* EXTERNALS   *//***************/#include <ToolUtils.h>#include <QD3D.h>#include "myglobals.h"#include "myevents.h"#include "mymenus.h"#include "misc.h"#include "qd3d_support.h"#include "process.h"extern	WindowPtr				gModelWindow;extern	QD3DSetupOutputType		gModelViewInfo;/****************************//*    PROTOTYPES            *//****************************/static void DoHighLevelEvent(void);static Boolean IsDAWindow(WindowPtr whichWindow);static OSErr  hasGotRequiredParams(AppleEvent *appEvent);static void	HandleMouseDown(void);/****************************//*    CONSTANTS             *//****************************//**********************//*     VARIABLES      *//**********************/EventRecord	gTheEvent;/******************** HANDLE EVENTS ************************/void HandleEvents(void){char	theChar;GrafPtr	oldPort;			WaitNextEvent(everyEvent,&gTheEvent, 0, 0);					switch (gTheEvent.what)	{		case	nullEvent:				DoModelWindowNullEvent();				break;						case	mouseDown:				HandleMouseDown();				break;						case	keyDown:		case	autoKey:				theChar	= gTheEvent.message & charCodeMask;				if	((gTheEvent.modifiers & cmdKey) != 0)					HandleMenuChoice(MenuKey(theChar));				break;						case	updateEvt:				if (!IsDAWindow((WindowPtr)gTheEvent.message))				{					GetPort (&oldPort);					SetPort ((WindowPtr)gTheEvent.message);					BeginUpdate((WindowPtr)gTheEvent.message);					DrawControls((WindowPtr)gTheEvent.message);										if ((WindowPtr)gTheEvent.message == gModelWindow)						DrawModelWindow();																EndUpdate((WindowPtr)gTheEvent.message);					SetPort(oldPort); 				}				else 				{					BeginUpdate((WindowPtr)gTheEvent.message);					EndUpdate((WindowPtr)gTheEvent.message);				}				break;						case	kHighLevelEvent:				DoHighLevelEvent();				break;	}}/**************** DO HIGH LEVEL EVENT *****************/static void DoHighLevelEvent(void){OSErr	myErr;	myErr = AEProcessAppleEvent(&gTheEvent);}/************** HANDLE MOUSE DOWN *******************/static void HandleMouseDown(void){	WindowPtr		whichWindow;	short 			thePart;	long			menuChoice, windSize;		thePart	=	FindWindow(gTheEvent.where, &whichWindow);	switch(thePart)	{		case	inMenuBar:				menuChoice = MenuSelect(gTheEvent.where);				HandleMenuChoice(menuChoice);				break;						case	inSysWindow:				SystemClick(&gTheEvent, whichWindow);				break;						case	inDrag:				DragWindow(whichWindow,gTheEvent.where, &qd.screenBits.bounds);				break;						case	inGoAway:				DisposeWindow (whichWindow);				break;						case	inContent:				SelectWindow(whichWindow);				break;						case	inGrow:				windSize = GrowWindow(whichWindow, gTheEvent.where,									&(**GetGrayRgn()).rgnBBox);				if (windSize != 0)				{					SetPort(whichWindow);					EraseRect(&whichWindow->portRect);					SizeWindow(whichWindow,LoWord(windSize),								HiWord(windSize), NORMAL_UPDATES);					InvalRect(&whichWindow->portRect);										if (whichWindow == gModelWindow)			// see if change Skeleton window					{						QD3D_ChangeDrawSize(&gModelViewInfo);					}				} 				break;	}}/**************** IS DA WINDOW ***********************/static Boolean IsDAWindow(WindowPtr whichWindow){	if	(whichWindow == nil)		return (false);	else		return (((WindowPeek)whichWindow)->windowKind < 0);}/*********************** MY APPLE EVENT: QUIT APPLICATION *************************/pascal OSErr MyAE_QuitApplication(AppleEvent *theAppleEvent, AppleEvent *reply,							SInt32 handlerRefcon){	CleanQuit();	return(noErr);}