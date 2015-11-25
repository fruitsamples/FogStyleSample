/****************************//*   	PROCESS.C     		*//* By Brian Greenstone      *//****************************//****************************//*    EXTERNALS             *//****************************/#include <QD3D.h>#include <QD3DGeometry.h>#include <QD3DGroup.h>#include <QD3DMath.h>#include <QD3DView.h>#include <QD3DGroup.h>#include <QD3DTransform.h>#include <QD3DStorage.h>#include "myglobals.h"#include "misc.h"#include "process.h"#include "qd3d_support.h"extern	float				gFramesPerSecond;/****************************//*    PROTOTYPES            *//****************************/static void InitModelWindow(void);static void BuildTestModel(void);static void CreateMyFogStyle(void);/****************************//*    CONSTANTS             *//****************************/#define	MODEL_WIND_ID			400#define	MY_HITHER 				10#define MY_YON					120/*********************//*    VARIABLES      *//*********************/WindowPtr				gModelWindow = nil;QD3DSetupOutputType		gModelViewInfo;TQ3Object				gModelGroup = nil;TQ3GroupObject			gBaseGroup = nil;TQ3AttributeSet		gTextureAttr;TQ3Matrix4x4	gTransformMatrix;TQ3Object		gTransformObject = nil;TQ3FogStyleData	gFogData;TQ3StyleObject	gFogStyleObject = nil;TQ3ColorARGB	gClearColor = {1,1,1,1};#pragma mark ========INITIALIZATION STUFF=========/******************** INIT TEST ************************/void InitTest(void){			/* INIT THE WINDOW */				InitModelWindow();			/* CREATE THE TEST MODEL */				BuildTestModel();			/* CREATE DISPLAY GROUP */				gBaseGroup = Q3DisplayGroup_New();										// make display group 		Q3Matrix4x4_SetIdentity(&gTransformMatrix);	gTransformObject = Q3MatrixTransform_New(&gTransformMatrix);			// make transform object		Q3Group_AddObject(gBaseGroup, gTransformObject);						// add transform to group			Q3Group_AddObject(gBaseGroup, gFogStyleObject);	// add fog to it	Q3Group_AddObject(gBaseGroup, gModelGroup);								// add geometry to group 			/* DRAW THE WINDOW */							DrawModelWindow();}/************** INIT MODEL WINDOW *******************/static void InitModelWindow(void){QD3DSetupInputType	viewDef;TQ3Point3D			cameraFrom = { 0,0, 100 };TQ3Point3D			cameraTo = { 0, 0, 0 };TQ3Vector3D			cameraUp = { 0.0, 1.0, 0 };TQ3ColorRGB			ambientColor = { 1.0, 1.0, 1.0 };TQ3ColorRGB			fillColor = { 0.0, 1.0, 1.0 };TQ3Vector3D			fillDirection1 = {1, -.3, -1 };TQ3Vector3D			fillDirection2 = { -1, -.1, -.2 };			/* CREATE THE WINDOW */				gModelWindow = GetNewCWindow(MODEL_WIND_ID, nil,MOVE_TO_FRONT);	if (gModelWindow == nil)		DoFatalAlert("\pWhere did the GameWindow window go?");						SetPort((GrafPtr)gModelWindow);			/***********************/			/* SET QD3D PARAMETERS */			/***********************/	viewDef.view.displayWindow 		= gModelWindow;	viewDef.view.rendererType 		= kQ3RendererTypeInteractive;	viewDef.view.clearColor 		= gClearColor;	viewDef.view.paneClip.left 		= 0;	viewDef.view.paneClip.right 	= 0;	viewDef.view.paneClip.top 		= 0;	viewDef.view.paneClip.bottom 	= 0; 	viewDef.styles.interpolation 	= kQ3InterpolationStyleVertex; 	viewDef.styles.backfacing 		= kQ3BackfacingStyleBoth;	viewDef.styles.fill				= kQ3FillStyleFilled; 	viewDef.styles.illuminationType	= kQ3IlluminationTypeLambert; 	viewDef.camera.from 			= cameraFrom;	viewDef.camera.to 				= cameraTo;	viewDef.camera.up 				= cameraUp;	viewDef.camera.hither 			= MY_HITHER;	viewDef.camera.yon 				= MY_YON;	viewDef.camera.fov 				= 1.0;	viewDef.lights.ambientBrightness = .1;	viewDef.lights.ambientColor 	= ambientColor;	viewDef.lights.numFillLights 	= 1;	viewDef.lights.fillDirection[0] = fillDirection1;	viewDef.lights.fillDirection[1] = fillDirection2;	viewDef.lights.fillColor[0] 	= fillColor;	viewDef.lights.fillColor[1] 	= fillColor;	viewDef.lights.fillBrightness[0] = 1.1;	viewDef.lights.fillBrightness[1] = 0.7;	QD3D_SetupWindow(&viewDef, &gModelViewInfo);						/* CREATE FOG STYLE OBJECT */				CreateMyFogStyle();	}/******************* CREATE MY FOG STYLE OBJECT *****************/static void CreateMyFogStyle(void){			/* SET MY PARAMETERS */	gFogData.state 		= kQ3On;						// fog is on	gFogData.mode 		= kQ3FogModeLinear;				// fog is linear	gFogData.fogStart 	= 60;							// start @ front	gFogData.fogEnd		= MY_YON;						// end in back	gFogData.density 	= 1.0;							// goes totally foggy at back	gFogData.color		= gClearColor;					// fog color is same as clear color			/* MAKE NEW OBJECT */				gFogStyleObject = Q3FogStyle_New(&gFogData);	if (gFogStyleObject == nil)		DoFatalAlert("\pCreateMyFogStyle: Q3FogStyle_New failed!");}/********************* BUILD TEST MODEL **************************/static void BuildTestModel(void){TQ3TriMeshData					myTriMeshData;TQ3TriMeshAttributeData			vertexAttribs[3],faceAttribs;TQ3Point3D points[3] = {	0,35,0,	-35,-35,0,	35,-35,0};TQ3Vector3D	vertexNormals[3] = {	0,0,1,	0,0,1,	0,0,1};TQ3Vector3D	faceNormals[1] ={	0,0,1};TQ3Param2D uvArray[3] ={	.5,1,	0,0,	1,0};TQ3TriMeshTriangleData	triangles[1] ={	0,1,2};		TQ3SurfaceShaderObject	texture;			/* FIRST LOAD THE TEXTUREMAP */						texture = QD3D_GetTextureMap(128,nil);			gTextureAttr = Q3AttributeSet_New();	Q3AttributeSet_Add(gTextureAttr, kQ3AttributeTypeSurfaceShader, &texture);					/* BUILD MAIN TRIMESH DATA STRUCTURE */	myTriMeshData.triMeshAttributeSet = gTextureAttr;	myTriMeshData.numTriangles = 1;	myTriMeshData.triangles = &triangles[0];	myTriMeshData.numTriangleAttributeTypes = 1;	myTriMeshData.triangleAttributeTypes = &faceAttribs;	myTriMeshData.numEdges = 0;	myTriMeshData.edges = nil;	myTriMeshData.numEdgeAttributeTypes = 0;	myTriMeshData.edgeAttributeTypes = nil;	myTriMeshData.numPoints = 3;	myTriMeshData.points = &points[0];	myTriMeshData.numVertexAttributeTypes = 1; 	myTriMeshData.vertexAttributeTypes = &vertexAttribs[0];			/* CALCULATE BOUNDING BOX */	Q3BoundingBox_SetFromPoints3D(&myTriMeshData.bBox, &points[0],	3, sizeof(TQ3Point3D));			/* CREATE FACE ATTRIBUTES */ 	faceAttribs.attributeType = kQ3AttributeTypeNormal;	faceAttribs.data = &faceNormals[0];	faceAttribs.attributeUseArray = nil;			/* CREATE VERTEX ATTRIBUTES */ 	vertexAttribs[0].attributeType = kQ3AttributeTypeSurfaceUV;	vertexAttribs[0].data = &uvArray[0];	vertexAttribs[0].attributeUseArray = nil;		/* MAKE THE TRIMESH GEOMETRY OBJECT */	gModelGroup = Q3TriMesh_New(&myTriMeshData);	if (gModelGroup == nil)		DoFatalAlert("\pQ3TriMesh_New failed!");}#pragma mark =========UPDATING============/*************** DO MODEL WINDOW NULL EVENT **********************/void DoModelWindowNullEvent(void){TQ3Matrix4x4	m;			/* ROTATE THE GEOMETRY */				QD3D_CalcFramesPerSecond();	Q3Matrix4x4_SetRotate_XYZ(&m,0.4/gFramesPerSecond,0.5/gFramesPerSecond,0);	Q3Matrix4x4_Multiply(&m,&gTransformMatrix,&gTransformMatrix);				/* UPDATE THE MATRIX */				Q3MatrixTransform_Set(gTransformObject,&gTransformMatrix);			/* REDRAW IT */					DrawModelWindow();}/******************* DRAW MODEL WINDOW *********************///// Calls the support function QD3D_DrawScene.// It passes the view info and a pointer to the game draw callback.//void DrawModelWindow(void){	QD3D_DrawScene(&gModelViewInfo,(void *)SubmitFogAndGeometry);}/*************** SUBMIT FOG AND GEOMETRY ******************/void SubmitFogAndGeometry(QD3DSetupOutputType *viewInfo){//	Q3Style_Submit(gFogStyleObject,viewInfo->viewObject);		// submit the fog style//	Q3FogStyle_Submit(&gFogData,viewInfo->viewObject);			// submit fog in immediate mode			Q3Object_Submit(gBaseGroup,viewInfo->viewObject);			// submit the geometry}