// ============================================================================
//	Includes
// ============================================================================
//---- standard lips -------------

#ifndef __main__
#define __main__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>							// malloc(), free()
#include <math.h>

#include "user_interface.h"

// ----- GL lips ------------------
#include <GL/freeglut.h>

//------- ARToolkit lips ----------
#include <AR/gsub_lite.h>

//------- Model lips ----------
#include "Models.h"

//------- AR lips ----------
#include "ar_tracker.h"
#include "ar_content.h"
#include "finite_state_machine.h"


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


// the global Assimp scene 
MODEL knife("../Models/knife.stl");
MODEL board("../Models/board.stl");
MODEL pot("../Models/pot.stl");
MODEL potWater("../Models/potWater.stl");
MODEL lid("../Models/lid.stl");
MODEL fish("../Models/fish.stl");
MODEL fishCut("../Models/fishCut.stl");
MODEL carrot("../Models/carrot.stl");
MODEL carrotCut("../Models/carrotCut.stl");
MODEL meat("../Models/meat.stl");
MODEL meatCut("../Models/meatCut.stl");

#endif // !__main__
