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
	std::string text;		// definition textstring

	//*********************** state 0 *****************************
	if (state == 0)						//Initialschritt
	{
		//output ar content
		DrawBoardCarrotLeek();

		//output text
		text = "Willkommen zu dieser Kochanleitung!";			// text content
	}

	//*********************** state 0 *****************************
	if (state == 1)						//Initialschritt
	{
		//output ar content
		DrawBoardCarrotLeek();

		//output text
		
		text = "Zutaten: Karotte und Lauch";			// text content
	}

	//*********************** state 1 *****************************
	if (state == 2)						//Initialschritt
	{
		//output ar content
		DrawBoardFishMeat();

		//output text
		
		text = "Zutaten: Fish und Rindfleisch";			// text content
	}

	//*********************** state 2 *****************************
	if (state == 3)						//Initialschritt
	{
		//output ar content
		DrawWashFood();
		
			//output text
		
		text = "Waschen Sie das Gemuese";			// text content
	}

	//*********************** state 3 *****************************
	if (state == 4)						//Initialschritt
	{
		//output ar content
		DrawBoardCarrotCutKnife();

		//output text
		
		text = "Schneiden Sie die Karotte in Scheiben";			// text content

	}

	//*********************** state 4 *****************************
	if (state == 5)						//Initialschritt
	{
		//output ar content
		DrawBoardLeekCutKnife();

		//output text
		
		text = "Schneiden Sie den Lauch in Scheiben";			// text content
	}

	//*********************** state 5 *****************************
	if (state == 6)						//Initialschritt
	{
		//output ar content
		DrawBoardFishCutKnife();

		//output text
		
		text = "Schneiden Sie den Fisch in vier Scheiben";			// text content
	}

	//*********************** state 6 *****************************
	if (state == 7)						//Initialschritt
	{
		//output ar content
		DrawBoardMeatCutKnife();

		//output text
		
		text = "Schneiden Sie das Fleisch in Wuerfel";			// text content
	}

	//*********************** state 7 *****************************
	if (state == 8)						//Initialschritt
	{
		//output ar content
		DrawPotWaterInSink();

		//output text
		
		text = "Fuellen Sie den Topf mit Wasser";			// text content
	}

	//*********************** state 8 *****************************
	if (state == 9)						//Initialschritt
	{
		//output ar content
		DrawPotWaterOnStove();

		//output text
		
		text = "Stellen Sie den Topf auf den Herd";			// text content
	}

	//*********************** state 9 *****************************
	if (state == 10)						//Initialschritt
	{
		//output ar content
		DrawFoodInPot(state);

		//output text
		
		text = "Fuegen Sie die geschnittenen Karotten hinzu";			// text content
	}

	//*********************** state 10 *****************************
	if (state == 11)						//Initialschritt
	{
		//output ar content
		DrawFoodInPot(state);

		//output text
		
		text = "Fuegen Sie den geschnittenen Lauch hinzu";			// text content
	}

	//*********************** state 11 *****************************
	if (state == 12)						//Initialschritt
	{
		//output ar content
		DrawFoodInPot(state);

		//output text
		
		text = "Fuegen Sie den geschnittenen Fisch hinzu";			// text content
	}

	//*********************** state 12 *****************************
	if (state == 13)						//Initialschritt
	{
		//output ar content
		DrawFoodInPot(state);

		//output text
		
		text = "Fuegen Sie das gewuerfelte Fleisch hinzu";			// text content
	}

	//*********************** state 13 *****************************
	if (state == 14)						//Initialschritt
	{
		//output ar content
		DrawSoupDone();

		//output text
		
		text = "Lassen sie die den Topf 20 Minuten koecheln";			// text content
	}

	//*********************** state 14 *****************************
	if (state == 15)						//Initialschritt
	{
		//output ar content
		DrawServeFood();

		//output text
		
		text = "Die fertige Suppe in eine Schuessel schoepfen und servieren";			// text content
	}

	glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
	drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)
}