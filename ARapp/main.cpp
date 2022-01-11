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
#include <GL/freeglut.h>

//------- ARToolkit lips ----------
#include <AR/gsub_lite.h>

//------- assimp lips ----------
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//--------- own lips -------------
//#include <finite_state_machine.h>
//#include <ar_content.h>
//#include <user_interface.h>

// ============================================================================
//	Global definitions
// ============================================================================
int loadasset(aiVector3D* scene_min, aiVector3D* scene_max, const C_STRUCT aiScene* scene, aiVector3D* scene_center);

// the global Assimp scene object class

class MODEL
{
public:
	MODEL(char* path_model);
	~MODEL();

	const C_STRUCT aiScene * scene;
	GLuint scene_list;
	C_STRUCT aiVector3D scene_min, scene_max, scene_center;
};
MODEL::MODEL(char* path_model)
{
	//scene = NULL;
	scene_list = 0;

	// Load the model file.
	scene = aiImportFile(path_model, aiProcessPreset_TargetRealtime_MaxQuality);

	if (0 != loadasset(&this->scene_min, &this->scene_max, this->scene, &this->scene_center)) {
		printf_s("Failed to load model. Please ensure that the specified file exists.");
	}
	
}

MODEL::~MODEL()
{
}



// ============================================================================
//	Constants
// ============================================================================

#define VIEW_SCALEFACTOR		0.025		// 1.0 ARToolKit unit becomes 0.025 of my OpenGL units.
#define VIEW_DISTANCE_MIN		0.1			// Objects closer to the camera than this will not be displayed.
#define VIEW_DISTANCE_MAX		100.0		// Objects further away from the camera than this will not be displayed.




// ============================================================================
//	Global variables
// ============================================================================

// the global Assimp scene 
MODEL knife("../Models/knife.stl");
MODEL fish("../Models/fish.obj");
MODEL pot("../Models/pot.stl");
MODEL lid("../Models/lid.stl");

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


#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

// set the bounds of the node of the model
void get_bounding_box_for_node(const C_STRUCT aiNode* nd,
	C_STRUCT aiVector3D* min,
	C_STRUCT aiVector3D* max,
	C_STRUCT aiMatrix4x4* trafo,
	const aiScene* scene
) {
	C_STRUCT aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const C_STRUCT aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			C_STRUCT aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n], min, max, trafo, scene);
	}
	*trafo = prev;
}

// set the bounds of the displayed area 
void get_bounding_box(C_STRUCT aiVector3D* min, C_STRUCT aiVector3D* max, const aiScene* C_STRUCT scene)
{
	C_STRUCT aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode, min, max, &trafo, scene);
}

// convert a color array to a float array
void color4_to_float4(const C_STRUCT aiColor4D* c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

// converte single value to a array of an color
void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

// set the surface of the models
void apply_material(const C_STRUCT aiMaterial* mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	C_STRUCT aiColor4D diffuse;
	C_STRUCT aiColor4D specular;
	C_STRUCT aiColor4D ambient;
	C_STRUCT aiColor4D emission;
	ai_real shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if (ret1 == AI_SUCCESS) {
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if (ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
		else
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 1.0f, 0.0f, 0.0f, 1.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}

// Render the imported models
void recursive_render(const C_STRUCT aiScene* sc, const C_STRUCT aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	C_STRUCT aiMatrix4x4 m = nd->mTransformation;

	/* update transform */
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	/* draw all meshes assigned to this node */
	for (; n < nd->mNumMeshes; ++n) {
		//const C_STRUCT aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		const C_STRUCT aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

		//color
		//aiColor3D color(1.0f, 0.0f, 0.0f);


		apply_material(sc->mMaterials[mesh->mMaterialIndex]);
		/*
		if (mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		}
		else {
			glEnable(GL_LIGHTING);
		}
		*/

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const C_STRUCT aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch (face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);
				for (i = 0; i < face->mNumIndices; i++) {
					int index = face->mIndices[i];
					if (mesh->mColors[0] != NULL)
						glColor4fv((GLfloat*)&mesh->mColors[0][index]);
					if (mesh->mNormals != NULL)
						glNormal3fv(&mesh->mNormals[index].x);
					glVertex3fv(&mesh->mVertices[index].x);
				}
			glEnd();
		}

	}

	/* draw all children */
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n]);
	}

	glPopMatrix();
}

// load the model to the scene
int loadasset(aiVector3D* scene_min, aiVector3D* scene_max, const C_STRUCT aiScene* scene, aiVector3D* scene_center)
{
	/* we are taking one of the postprocessing presets to avoid
	   spelling out 20+ single postprocessing flags here. */

	if (scene) {
		get_bounding_box(scene_min, scene_max, scene);
		scene_center->x = (scene_min->x + scene_max->x) / 2.0f;
		scene_center->y = (scene_min->y + scene_max->y) / 2.0f;
		scene_center->z = (scene_min->z + scene_max->z) / 2.0f;
		return 0;
	}
	return 1;
}


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

//void load_model(const aiScene* scene, aiVector3D* scene_max, aiVector3D* scene_min, aiVector3D* scene_center, GLuint scene_list) {
void load_model(MODEL model, ai_real x, ai_real y, ai_real z) {
	float tmp;
	tmp = model.scene_max.x - model.scene_min.x + x;
	tmp = aisgl_max(model.scene_max.y - model.scene_min.y + y, tmp);
	tmp = aisgl_max(model.scene_max.z - model.scene_min.z + z, tmp);
	tmp = 1.f / tmp;
	glScalef(tmp, tmp, tmp);

	/* center the model */
	glTranslatef(-model.scene_center.x, -model.scene_center.y, -model.scene_center.z);

	//color
	//aiColor3D color(1.0f, 0.0f, 0.0f);

	// if the display list has not been made yet, create a new one and fill it with scene contents 
	if (model.scene_list == 0) {
		model.scene_list = glGenLists(1);
		glNewList(model.scene_list, GL_COMPILE);
		// now begin at the root node of the imported data and traverse the scenegraph by multiplying subsequent local transforms together on GL's matrix stack. 
		recursive_render(model.scene, model.scene->mRootNode);
		glEndList();
	}
	glCallList(model.scene_list);
}

// This function is the display handler of this program and called when the window needs redrawing.

static void Display(void)
{
	GLdouble p[16];		// Projection matrix
	GLdouble m[16];		// Modelling matrix

	// Select correct buffer for this context.
	glDrawBuffer(GL_BACK); // Select the framepuffer (back right and back left color buffers)
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

		//glutWireCone(2.0, 2.0, 20, 20); // Zeichnet einen Drahtkörper im Zentrum des AR-Markers


		/* scale the whole asset to fit into our view frustum */

		load_model(knife,0,0,0);

		//load_model(fish,0.5,0.5,0);
		//load_model(pot, 0.5, 0.5, 0);
		//load_model(lid, 0.5, 0.5, 0);

	} 
		

	// Any 2D overlays go here.
	glutSwapBuffers();
}


//++++++++++++++++++ Main function ++++++++++++++++++++++++++++++++++++++

int main(int argc, char** argv)
{
	//char* model_file = "../models/knife.stl";

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
	
	// leave loop on close
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);


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
	glDisable(GL_LIGHTING);

	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

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


	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);



	// Transparend background after clear
	glClearColor(0, 0, 0, 1);



	
	//open GL main loop, controls the updates
	glutMainLoop();		
	//               -------->--------
	//               |	main 		|	
	//				 |	loop		|
	//				 --------<-------	

	// free space to avoid ressource leakage (assimp)
	aiReleaseImport(knife.scene);
	aiReleaseImport(fish.scene);

	return (0);
}
