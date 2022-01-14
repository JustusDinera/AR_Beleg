//--------------- Includes ------------------------------

#include <string>

#include <GL/glut.h>
#include <ar_content.h>


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
			DrawTeapot();					//Draw teapot in this ste
			
			
			//output text
	
			std::string text;		// definition textstring
			text = "Willkommen, viel Spass bei diesem Versuch! :";			// text content
			glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
			drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)

		}
	
//*********************** state 1 *****************************

		if (state == 1)						// Deckel öffnen
		{
			
			//output ar content
			
			
			//output text
			std::string text;		// definition textstring
			text = "Schritt 1: Oeffnen Sie den Deckel der Kanne!! :";			// text content
			glColor3f(1, 0, 0);		// text color e.g. (1,0,0 red)
			drawText(text.data(), text.size(), 10, 10); //textstring, textlength (from xstring), position x,y (origin lower left 0,0)

		}
	
//*********************** state 2 *****************************		
		
		if (state == 2)						// Wasser und Tee einfüllen
		{
			
			//output ar content
			

			//output text
		
		
		}




//************************** further states **************

//             .
//             .
//             Further states ...........
//             .

}