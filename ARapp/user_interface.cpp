//----------------- Includes ------------------------------------------

#include <GL/glut.h>
#include <AR/video.h>
#include <AR/gsub_lite.h>


//----------------- Extern Global Variables --------------------------------

extern int state;	// extern definition of the variable
extern ARGL_CONTEXT_SETTINGS_REF gArglSettings; // extern definition of the variable
extern long	gCallCountMarkerDetect; // extern definition of the variable

//********************* Output functions ******************************

void Quit(void) //Quite function
{
	arglCleanup(gArglSettings);
	arVideoCapStop();
	arVideoClose();
	exit(0);
}

void debugReportMode(void) //Debug mode function
{
	if (arFittingMode == AR_FITTING_TO_INPUT) {
		fprintf(stderr, "FittingMode (Z): INPUT IMAGE\n");
	}
	else {
		fprintf(stderr, "FittingMode (Z): COMPENSATED IMAGE\n");
	}

	if (arImageProcMode == AR_IMAGE_PROC_IN_FULL) {
		fprintf(stderr, "ProcMode (X)   : FULL IMAGE\n");
	}
	else {
		fprintf(stderr, "ProcMode (X)   : HALF IMAGE\n");
	}

	if (arglDrawModeGet(gArglSettings) == AR_DRAW_BY_GL_DRAW_PIXELS) {
		fprintf(stderr, "DrawMode (C)   : GL_DRAW_PIXELS\n");
	}
	else if (arglTexmapModeGet(gArglSettings) == AR_DRAW_TEXTURE_FULL_IMAGE) {
		fprintf(stderr, "DrawMode (C)   : TEXTURE MAPPING (FULL RESOLUTION)\n");
	}
	else {
		fprintf(stderr, "DrawMode (C)   : TEXTURE MAPPING (HALF RESOLUTION)\n");
	}

	if (arTemplateMatchingMode == AR_TEMPLATE_MATCHING_COLOR) {
		fprintf(stderr, "TemplateMatchingMode (M)   : Color Template\n");
	}
	else {
		fprintf(stderr, "TemplateMatchingMode (M)   : BW Template\n");
	}

	if (arMatchingPCAMode == AR_MATCHING_WITHOUT_PCA) {
		fprintf(stderr, "MatchingPCAMode (P)   : Without PCA\n");
	}
	else {
		fprintf(stderr, "MatchingPCAMode (P)   : With PCA\n");
	}
}



//********************** Input functions ********************************


void onMouseButton(int button, int state_mouse, int x, int y) //--------------- Mouse handler ------------

{

	if (state_mouse == GLUT_DOWN)			// test if mouse button pressed

	{
		// set the color to pure red for the left button


		if (button == GLUT_LEFT_BUTTON) // check mouse state
			if (state > 0)
				state--;				// last state

		if (button == GLUT_MIDDLE_BUTTON)
			Quit();		// quit app

		if (button == GLUT_RIGHT_BUTTON)
		{
			if (state < 15)
				state++;				// next state
		}
	}
}


void Keyboard(unsigned char key, int x, int y) // Keyboard handler
{
	int mode;
	switch (key) {
	case 0x1B:						// Quit.
	case 'Q':
	case 'q':
		
		Quit();
		break;
	case ' ':
		//	gDrawRotate = !gDrawRotate;
		break;
	
		
	case 'C':	
	case 'c':
		mode = arglDrawModeGet(gArglSettings);
		if (mode == AR_DRAW_BY_GL_DRAW_PIXELS) {
			arglDrawModeSet(gArglSettings, AR_DRAW_BY_TEXTURE_MAPPING);
			arglTexmapModeSet(gArglSettings, AR_DRAW_TEXTURE_FULL_IMAGE);
		}
		else {
			mode = arglTexmapModeGet(gArglSettings);
			if (mode == AR_DRAW_TEXTURE_FULL_IMAGE)	arglTexmapModeSet(gArglSettings, AR_DRAW_TEXTURE_HALF_IMAGE);
			else arglDrawModeSet(gArglSettings, AR_DRAW_BY_GL_DRAW_PIXELS);
		}
		fprintf(stderr, "*** Camera - %f (frame/sec)\n", (double)gCallCountMarkerDetect / arUtilTimer());
		gCallCountMarkerDetect = 0;
		arUtilTimerReset();
		debugReportMode();
		break;

	
	case 'D':
	case 'd':
		arDebug = !arDebug;
		break;
	case '?':
	case '/':
		printf("Keys:\n");
		printf(" q or [esc]    Quit demo.\n");
		printf(" w or s	       to switch between steps.\n");
		printf(" c             Change arglDrawMode and arglTexmapMode.\n");
		printf(" d             Activate / deactivate debug mode.\n");
		printf(" ? or /        Show this help.\n");
		printf("\nAdditionally, the ARVideo library supplied the following help text:\n");
		arVideoDispOption();
		break;
	case 'W':
	case 'w':	// zum nächsten Anleitungsstate springen
		if (state < 15)
			state++;
		break;
	case 'S':
	case 's': 	// zum vorherigen Anleitungsstate springen
		if (state > 0)
			state--;
		break;
	default:
		break;
	}
}



