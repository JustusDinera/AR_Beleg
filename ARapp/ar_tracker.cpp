//----------- Includes -----------------
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>							// malloc(), free()
#include <math.h>

// ----- GL lips ------------------

#include <GL/glut.h>

//------- ARToolkit lips ----------
#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>						// arParamDisp()
#include <AR/ar.h>
#include <AR/gsub_lite.h>
#include <AR/gsub.h>

//--------- own libs -------------

#include <ar_content.h>



//--------- Global variable -------

ARUint8		*gARTImage = NULL;						// Image acquisition
ARParam		gARTCparam;

// Marker detection.
int		gARTThreshhold = 100;			// Threshold to find the ar-marker in a binary image
long	gCallCountMarkerDetect = 0;		// Number of detected markers

double	gPatt_width = 80.0;				// Per-marker, but we are using only 1 marker.
double	gPatt_centre[2] = { 0.0, 0.0 }; // Per-marker, but we are using only 1 marker.
double	gPatt_trans[3][4];				// Stores the transformation of the detected markers, Per-marker, but we are using only 1 marker. 
int		gPatt_found = FALSE;			// Per-marker, but we are using only 1 marker.
int		gPatt_id;						// ID of the detected marker


//-------- definitions --------------


//---------- functions declaration

static int setupMarker(const char *patt_name, int *patt_id);
void init_cam(void);
int setupCamera(const char *cparam_name, char *vconf, ARParam *cparam); // Init ARCamera
void init_marker(void);

//------------- Setup AR-Camera

void init_cam(void)

{
	//path to the camera parameter data
	const char *cparam_name = "Data/camera_para.dat";    
	
	// Camera configuration.
	#ifdef _WIN32
		char			*vconf = "Data\\WDM_camera_flipV.xml";  // video capture library configuration string
	#else
		char			*vconf = "";
	#endif

	// ! Call cam setup, return error message ist cam ist not correct opened
	if (!setupCamera(cparam_name, vconf, &gARTCparam))  

		{	//Camera setup 
			fprintf(stderr, "main(): Unable to set up AR camera.\n");
			exit(-1);
		}
}

// setupCamera loads a file containing calibration parameters for a camera, opens a connection to the camera, sets some defaults(the binarization threshhold in this case) 
// and starts grabbing frames. It records its settings into 3 variables which are passed in as parameters. In our case, we will store these parameters in global variables.
int setupCamera(const char *cparam_name, char *vconf, ARParam *cparam)
{
	ARParam			wparam;
	int				xsize, ysize;	// Variables to store the video stream sizes

	// Open the video path.
	if (arVideoOpen(vconf) < 0) 
	
		{
			fprintf(stderr, "setupCamera(): Unable to open connection to camera.\n");
			return (FALSE);
		}

	// Find the size of the camera video stream.
	if (arVideoInqSize(&xsize, &ysize) < 0) return (FALSE);				
		
		fprintf(stdout, "Camera image size (x,y) = (%d,%d)\n", xsize, ysize);

	// Load the camera parameters, resize for the window and init.
	if (arParamLoad(cparam_name, 1, &wparam) < 0) 
	
		{
			fprintf(stderr, "setupCamera(): Error loading parameter file %s for camera.\n", cparam_name);
			return (FALSE);
		}
	
	// Once the camera parameters are loaded, we adjust them to match the actual video image size being supplied by the video library
	arParamChangeSize(&wparam, xsize, ysize, cparam);
	fprintf(stdout, "*** Camera Parameter ***\n");
	arParamDisp(cparam);
	arInitCparam(cparam);

	// Start video capturing
	if (arVideoCapStart() != 0) 
	
		{
			fprintf(stderr, "setupCamera(): Unable to begin camera data capture.\n");
			return (FALSE);
		}

	return (TRUE);
}

// ************************** Setup AR-Marker **************************

// Setup marker
static int setupMarker(const char *patt_name, int *patt_id)
{
	// load the maker pattern (only one marker)
	if ((*patt_id = arLoadPatt(patt_name)) < 0)

	{
		fprintf(stderr, "setupMarker(): pattern load error !!\n");
		return (FALSE);
	}

	return (TRUE);
}


// ---------- Init Marker type and path
void init_marker(void)
{
	//arSetPatternDetectionMode(arHandle, AR_MATRIX_CODE_DETECTION);
	//arSetMatrixCodeType(arHandle, AR_MATRIX_CODE_3x3_PARITY65);
	// Define the ar-marker and path
	const char *patt_name = "Data/patt.hiro";		

	// Call function setup marker
	if  (!setupMarker(patt_name, &gPatt_id))		

		{						//setup marker
			fprintf(stderr, "main(): Unable to set up AR marker.\n");
			exit(-1);
		}
}

//----------------------- main functions ----------------------------------

// Main loop to detect the markers
void ar_tracker(void)  
{
	static int ms_prev;
	int ms;
	float s_elapsed;
	ARUint8 *image;

	ARMarkerInfo    *marker_info;						// Pointer to array holding the details of detected markers
	int             marker_num;							// Count of number of markers detected
	int             j, k;							    

	// Find out how long since ar_tracker() last ran.
	ms = glutGet(GLUT_ELAPSED_TIME);
	s_elapsed = (float)(ms - ms_prev) * 0.001;
	if (s_elapsed < 0.01f) return;						// Don't update more often than 100 Hz.
	ms_prev = ms;

	// Update drawing.
	// übergibt die Zeitbasis von 1 Sekunde und Ruft die Motion-Aktualisierung auf
	//	DrawCubeUpdate(s_elapsed);


//************** Folgende zwei Zeilen müssen bei der Einbindung von "ar_content.h" wieder einkommentiert werden
	//ArrowMotion(s_elapsed);
	//CrossMotion(s_elapsed);


	// Get a new video-image and store to image
	if ((image = arVideoGetImage()) != NULL) 
	
		{	
			// Save the fetched image
			gARTImage = image;	
			// Invalidate any previous detected markers
			gPatt_found = FALSE; // After aquiring a new image, set the gPatt_found marker to false					
			// Increment ARToolKit FPS counter
			
			gCallCountMarkerDetect++; 

		//	Every time a new frame has been acquired, it needs to be searched for markers.This is accomplished
		//	by a call to the function arDetectMarker(), passing in the pointer to the new frame, and an ARHandle
		//	The ARHandle holds the ARToolKit marker detection settings and also stores the results of the marker detection
		if (arDetectMarker(gARTImage, gARTThreshhold, &marker_info, &marker_num) < 0)
			
			{
				exit(-1); // exit if no marker found in the image
			}

		// Check through the marker_info array for highest confidence
		// visible marker matching our preferred pattern.
		k = -1;
	
		for (j = 0; j < marker_num; j++) 
			{
				if (marker_info[j].id == gPatt_id) // Test of correct Pattern ID
			
					{
						if (k == -1) k = j; // First marker detected
						else if (marker_info[j].cf > marker_info[k].cf) k = j; // Higher confidence marker detected.
					}
			}

		// Test if k is modified and wie have found the correct Pattern!!
		if (k != -1)

			{
				// Get the transformation between the marker and the real camera into gPatt_trans.
				// we have found the marker containing the Hiro pattern, so the last task we ask ARToolKit "arGetTransMat(....)"
				// to perform on the marker is to retrieve its position and orientation(its "pose") relative to the camera
				arGetTransMat(&(marker_info[k]), gPatt_centre, gPatt_width, gPatt_trans);
				gPatt_found = TRUE;	// set the flag to TRUE after detecting the pattern
			}
		
		glutPostRedisplay();
	}
}
