//----------------- includes

#include <GL/glut.h>

//----------- Global variables

GLfloat xmove_1, ymove_1, zmove_1 = 0;		//Pfeilmotion Translatef-Parameter für state ==1
GLfloat xmove_2, ymove_2, zmove_2 = 0;		//Pfeilmotion Translatef-Parameter für state ==2

GLfloat crossmotion = 0;						//Drehparameter des Kreuzes

extern int state;	// extern definition of the variable

//----------- Drawing --------------------------

static float gDrawRotateAngle = 0;			// For use in drawing.
static int	 gDrawRotate = FALSE;


void DrawArrow(static short direction)		// Rotating arrow
{
		glLoadIdentity;
		glPushMatrix();						// Save world coordinate system.
		
		if (direction == 1) //moving arrow in
		{
			
			// Place base of cube on marker surface.
			glTranslatef(0.0, 0.0, 0.0);	// set to marker origin
			glRotatef(90.0, 1.0, 1.0, 1.0); // rotate arrow

			//moving the arrow
			if (xmove_1 > 0.5)
				xmove_1 = 0;
			glTranslatef(xmove_1, 0.0, 0.0);	// move in the xmove_1 specifed range

			//Select arrow color
			glDisable(GL_LIGHTING);				// Just use colours.
			glColor3f(1.0, 0.0, 0.0);
			//Draw the arrow quader
			glBegin(GL_POLYGON);				// Quader
			glVertex3f(0.0, 0.0, 0.0);			// x, z, y
			glVertex3f(0.0, 0.0, 0.25);
			glVertex3f(1.5, 0.0, 0.25);
			glVertex3f(1.5, 0.0, 0.0);
			glEnd();
			//draw the arrow triangle
			glBegin(GL_TRIANGLES);				// Triangle
			glVertex3f(1.5, 0.0, 0.375);
			glVertex3f(2.0, 0.0, 0.125);
			glVertex3f(1.5, 0.0, -0.125);
			glEnd();
			glPopMatrix();						// Restore world coordinate system.
			
		}
		
		if (direction == 2) //moving arrow out
		{
			glTranslatef(0.75, 0.0, -0.75);		// set base of the arrow (x, z , -y)
			glRotatef(270.0, -1.0, -1.0, -1.0); // rotate arrow

			//move the arrow
			if (xmove_2 > 0.5)					// an dieser Stelle ist es überflüssig 2 mal move zu generieren, nur Abtrennungshalber existieren zwei
				xmove_2 = 0;
			glTranslatef(xmove_2, 0.0, 0.0);	// move in the xmove_2 specifed range

			//Select arrow color
			glDisable(GL_LIGHTING);				// Just use colours.
			glColor3f(0.0, 1.0, 0.0);			// Green
			//Draw the arrow quader
			glBegin(GL_POLYGON);				//Quader
			glVertex3f(0.0, 0.0, 0.0);			//x, z, y
			glVertex3f(0.0, 0.0, 0.25);
			glVertex3f(1.5, 0.0, 0.25);
			glVertex3f(1.5, 0.0, 0.0);
			glEnd();
			//draw the arrow triangle
			glBegin(GL_TRIANGLES);				//Dreieck
			glVertex3f(0.0, 0.0, 0.375);
			glVertex3f(-0.5, 0.0, 0.125);
			glVertex3f(0.0, 0.0, -0.125);
			glEnd();
			glPopMatrix();						// Restore world coordinate system.
		}
	
}


void DrawTeapot(void)				//Das zu "bearbeitende" Objekt, Teekanne 
{
	GLfloat   mat_ambient[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat   mat_flash[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat   mat_flash_shiny[] = { 50.0 };
	GLfloat   light_position[] = { 100.0, -200.0, 200.0, 0.0 };
	GLfloat   ambi[] = { 0.1, 0.1, 0.1, 0.1 };
	GLfloat   lightZeroColor[] = { 0.9, 0.9, 0.9, 0.1 };

	// set the material 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 1.0, 0.0);			// set a new position raltive to the detected marker centre x,-y,z

	glutSolidTeapot(1);						// build a teapot

	glDisable(GL_LIGHTING);
}


void DrawCheck()					// for state 4: Draw check
{
					
		glLineWidth(7.5);
		glColor3f(0.0, 0.8, 0.4);			// Green
		glBegin(GL_LINES);
	
		glVertex3f(1, 1, 1.2);
		glVertex3f(-0.5, -1, 1.2);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(-0.5, -1, 1.2);
		glVertex3f(-1, -0.25, 1.2);
		glEnd();
	
}


void DrawCross()					// 
{
		glRotatef(crossmotion, 0.0, 1.0, 0.0); // Rotate about z axis.
		glLineWidth(7.5);
		glColor3f(1.0, 0.0, 0.0);			// Rot
		glBegin(GL_LINES);					// front cross
		glVertex3f(1.2, 1.9, 0.8);
		glVertex3f(-1.2, 0, 1.9);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(1.2, 0, 1.9);
		glVertex3f(-1.2, 1.9, 0.8);
		glEnd();
		glBegin(GL_LINES);					// back cross
		glVertex3f(1.2, 1.9, -0.8);
		glVertex3f(-1.2, 0, -1.9);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(1.2, 0, -1.9);
		glVertex3f(-1.2, 1.9, -0.8);
		glEnd();
}

//---- Updating functions in idl() loop --------------

void ArrowMotion(float timeDelta)			// updating the moving arrow parameter by idle()
{
	if (state == 1)
		xmove_1 = xmove_1 + 0.5f*timeDelta;
	if (state == 2)
		xmove_2 = xmove_2 + 0.5f*timeDelta;
}

void CrossMotion(float timeDelta)			// updating the crossmotion bei idle()
{
	crossmotion += timeDelta * 45.0f;		// Rotate Kreuz at 45 degrees per second.
	if (crossmotion > 360.0f)
		crossmotion -= 360.0f;
}


void drawText(const char *text, int length, int x, int y) // funtion to draw text

{

	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(x, y);

	for (int i = 0; i < length; i++) // loop to put out text

	{

		// using the bitmap function to output text
		//------ supported fonts ---------
		//	GLUT_BITMAP_8_BY_13
		//  GLUT_BITMAP_9_BY_15
		//	GLUT_BITMAP_TIMES_ROMAN_10
		//  GLUT_BITMAP_TIMES_ROMAN_24 
		//	GLUT_BITMAP_HELVETICA_10,
		//  GLUT_BITMAP_HELVETICA_12, 
		//  GLUT_BITMAP_HELVETICA_18

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);

	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);

}
