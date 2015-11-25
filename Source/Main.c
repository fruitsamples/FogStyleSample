/****************************//*  FOG TEST                *//* By Brian Greenstone      *//****************************//****************************//*    EXTERNALS             *//****************************/#include <Gestalt.h>#include "myglobals.h"#include "qd3d_support.h"#include "mymenus.h"#include "myevents.h"#include "misc.h"#include "3dmf.h"#include "process.h"extern	QD3DSetupOutputType		gModelViewInfo;/****************************//*    PROTOTYPES            *//****************************/static	void ToolBoxInit(void);/*****************//* TOOLBOX INIT  *//*****************/static void ToolBoxInit(void){TQ3Status	myStatus;long response; 	MaxApplZone();	InitGraf(&qd.thePort);	FlushEvents ( everyEvent, REMOVE_ALL_EVENTS);	InitFonts();	InitWindows();	InitDialogs(nil);	InitCursor();	InitMenus();	TEInit();				/* SEE IF QD3D AVAILABLE */		if((void *)Q3Initialize == (void *)kUnresolvedCFragSymbolAddress)		DoFatalAlert("\pQuickDraw 3D version 1.6 or better is required to run this application!");	myStatus = Q3Initialize();	if ( myStatus == kQ3Failure )		DoFatalAlert("\pQ3Initialize returned failure.");				Gestalt(gestaltQD3D, &response);	if (response & (1<<gestaltQD3DPresent))	{			Gestalt(gestaltQD3DVersion,&response);		if (response < 0x10600)						// must be using 1.6 or better		{err:					DoFatalAlert("\pQuickDraw 3D version 1.6 or better is required to run this application!");		}	}	else		goto err;}/*****************//* TOOLBOX EXIT  *//*****************/static void ToolBoxExit(void){TQ3Status	myStatus;	myStatus = Q3Exit();	if ( myStatus == kQ3Failure )		DoFatalAlert("\pQ3Exit returned failure.");				}/************************************************************//******************** PROGRAM MAIN ENTRY  *******************//************************************************************/void main(void){	ToolBoxInit(); 		 			InitMenuBar();	InitTest();	while (true)		HandleEvents();	ToolBoxExit();}