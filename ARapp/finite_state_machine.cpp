//--------------- Includes ------------------------------
//#include "finite_state_machine.h"

#include <string>
#include <GL/glut.h>
#include "ar_content.h"
//-------------- Definitions --------------------------------
#define max_Schritt				5			// max. steps of the finite state machine
#define min_Schritt				0			// first step of the finite state machine
#define max_Zeichenkette		59			// max number of characters stored in the array maximale (arra width)


//---------------- Global Variable extern
int state;						// current step of the finite state machine variable used in external files

//--------------- Functions

void fsm()	// Finite state machine
{
	//*********************** state 0 *****************************
	if (state == 0)						//Initialschritt
	{
		//output ar content
		DrawBoardCarrotFishLeek();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)
	}

	//*********************** state 1 *****************************
	if (state == 1)						//Initialschritt
	{
		//output ar content
		DrawWashFood();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0
	}

	//*********************** state 2 *****************************
	if (state == 2)						//Initialschritt
	{
		//output ar content
		DrawBoardCarrotCutKnife();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)
	}

	//*********************** state 3 *****************************
	if (state == 3)						//Initialschritt
	{
		//output ar content
		DrawBoardLeekCutKnife();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)

	}

	//*********************** state 4 *****************************
	if (state == 4)						//Initialschritt
	{
		//output ar content
		DrawBoardFishCutKnife();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)
	}

	//*********************** state 5 *****************************
	if (state == 5)						//Initialschritt
	{
		//output ar content
		DrawPotWaterInSink();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)
	}

	//*********************** state 6 *****************************
	if (state == 6)						//Initialschritt
	{
		//output ar content
		DrawPotWaterOnStove();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)
	}

	//*********************** state 7 *****************************
	if (state == 7)						//Initialschritt
	{
		//output ar content
		DrawFoodInPot();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)
	}

	//*********************** state 8 *****************************
	if (state == 8)						//Initialschritt
	{
		//output ar content
		DrawSoupDone();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)
	}

	//*********************** state 9 *****************************
	if (state == 9)						//Initialschritt
	{
		//output ar content
		DrawServeFood();

		//output text
		std::string text;		// definition textstring
		text = "";			// text content
		glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
		drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)
	}
}