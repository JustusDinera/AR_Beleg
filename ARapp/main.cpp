/* This File includes the Main function and the setup calls für arToolKit and OpenGL
*  main.c
*
*/
/*
*
* Copyright (c) 2001-2006 Philip Lamb (PRL) phil@eden.net.nz. All rights reserved.
* This file is part of ARToolKit.
*
* ARToolKit is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* ARToolKit is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ARToolKit; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/


// ============================================================================
//	Includes
// ============================================================================
//---- standard lips -------------

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>							// malloc(), free()
#include <math.h>
#include "ar_tracker.h"
#include "ar_content.h"
#include "finite_state_machine.h"
#include "user_interface.h"

// ----- GL lips ------------------
#include <GL/glut.h>

//------- ARToolkit lips ----------

#include <AR/gsub_lite.h>

//--------- own lips -------------


//#include <finite_state_machine.h>
//#include <ar_content.h>
//#include <user_interface.h>

// ============================================================================
//	Constants
// ============================================================================

#define VIEW_SCALEFACTOR		0.025		// 1.0 ARToolKit unit becomes 0.025 of my OpenGL units.
#define VIEW_DISTANCE_MIN		0.1			// Objects closer to the camera than this will not be displayed.
#define VIEW_DISTANCE_MAX		100.0		// Objects further away from the camera than this will not be displayed.

// ============================================================================
//	Global variables
// ============================================================================

// Setup the size of the openGL Window
static int prefWidth = 800;					// Fullscreen mode width.
static int prefHeight = 600;				// Fullscreen mode height.
// Setup puffer depth and refresh rate
static int prefDepth = 32;					// Fullscreen mode bit depth.
static int prefRefresh = 0;					// Fullscreen mode refresh rate. Set to 0 to use default rate.

ARGL_CONTEXT_SETTINGS_REF gArglSettings = NULL; // Open GL init settings

//---------------- External global variables -------------------------

extern ARUint8		*gARTImage;				// Pointer contains the video image
extern ARParam		gARTCparam;				// Camera parameters

extern double		gPatt_trans[3][4];		// Stores the transformation of the detected markers, Per-marker, but we are using only 1 marker. 
extern int			gPatt_found;			// Per-marker, but we are using only 1 marker

//---- external function declaration -------------------
void ar_tracker(void);
int setupMarker(const char *patt_name, int *patt_id);
void init_cam(void);
void init_marker(void);

// ============================================================================
//	Functions
// ============================================================================


//	This function is called on events when the visibility of the
//	GLUT window changes (including when it first becomes visible).

static void Visibility(int visible)
{
	if (visible == GLUT_VISIBLE) {
		glutIdleFunc(ar_tracker);
	}
	else {
		glutIdleFunc(NULL);
	}
}


//	This function is called when the
//	GLUT window is resized.
static void Reshape(int w, int h)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Call through to anyone else who needs to know about window sizing here.
}


// This function is the display handler of this program and called when the window needs redrawing.

static void Display(void)
{
	GLdouble p[16];		// Projection matrix
	GLdouble m[16];		// Modelling matrix

	// Select correct buffer for this context.
	glDrawBuffer(GL_BACK);									// Select the framepuffer (back right and back left color buffers)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear before selected buffers for new frame (GL_COLOR_BUFFER_BIT enables the color wiriting,  GL_DEPTH_BUFFER_BIT Indicates the depth buffer).

	arglDispImage(gARTImage, &gARTCparam, 1.0, gArglSettings);	// zoom = 1.0. drwaing a ARTolkit image to a openGL Context
	
	// Test if the ar tracker has found the marker
	if (gPatt_found) 
	
		{
		// Projection transformation.
		arglCameraFrustumRH(&gARTCparam, VIEW_DISTANCE_MIN, VIEW_DISTANCE_MAX, p);	//Set the viewing transformation between arToolkit and open GL, defining the view distance
		glMatrixMode(GL_PROJECTION);		// set projections matrix
		glLoadMatrixd(p);					// load matrix p
		glMatrixMode(GL_MODELVIEW);			

		// Viewing transformation.
		glLoadIdentity();
		
		// ARToolKit supplied distance in millimetres, but I want OpenGL to work in my units.
		arglCameraViewRH(gPatt_trans, m, VIEW_SCALEFACTOR);
		glLoadMatrixd(m);

		//******** insert ar_content below ************

		glutWireCone(2.0, 2.0, 20, 20); // Zeichnet einen Drahtkörper im Zentrum des AR-Markers



 
		} 

	// Any 2D overlays go here.
	glutSwapBuffers();
}


//++++++++++++++++++ Main function ++++++++++++++++++++++++++++++++++++++

int main(int argc, char** argv)
{

	//********** Cam and ar_tracker inits **************
	
	init_cam();   // Initialisation of the cam
	init_marker(); // Initialisation of ar markers

	//********** Open GL inits **************

	// Initialise OpenGL
	glutInit(&argc, argv);		

	// Setup color, depth
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// Definition of the window size initial bevore creating the window (on top defined parameters)
	glutInitWindowSize(prefWidth, prefHeight);		
	
	// Create output open GL window
	glutCreateWindow(argv[0]);						


	// We initialise the OpenGL part of our application
	// We use the ARgsub_lite library to manage the interaction between the ARToolKit video capture and tracking, and OpenGL
	
	if ((gArglSettings = arglSetupForCurrentContext()) == NULL) 
		{
			fprintf(stderr, "main(): arglSetupForCurrentContext() returned error.\n");
			exit(-1);
		}

	// We also print out some debugging information in the user's console (or command line window)
	// showing the startup parameters for the application

//	Folgender Report nur mit funktion user Interface
//	debugReportMode();		
	
	glEnable(GL_DEPTH_TEST);
	
	// We reset ARToolKit's timer (which can be used to keep track of frame rates).
	arUtilTimerReset();

	// ******************register all openGL handlers (used in polling by glut)

	// Register the display function in open GL for updating handler, Diplay controls the sub loop
	glutDisplayFunc(Display);	
	// Register the window reshape
	glutReshapeFunc(Reshape);	
	// Register the visibility handler
	glutVisibilityFunc(Visibility);	
	// Register the keyboard input function	






	
	//open GL main loop, controls the updates
	glutMainLoop();		
	//               -------->--------
	//               |	main 		|	
	//				 |	loop		|
	//				 --------<-------	

	return (0);
}
