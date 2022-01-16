#ifndef AR_CONTENT_H
#define AR_CONTENT_H


#include <GL/glut.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>							// malloc(), free()
#include <math.h>
#include "ar_tracker.h"
//#include "finite_state_machine.h"
#include "user_interface.h"

// ----- GL lips ------------------
#include <GL/freeglut.h>

//------- ARToolkit lips ----------
#include <AR/gsub_lite.h>

//------- assimp lips ----------
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// ============================================================================
//	Global declarations
// ============================================================================
// --------- global Class // the global Assimp scene object class -------------
class MODEL
{
public:
	MODEL(char* path_model);
	~MODEL();

	const C_STRUCT aiScene* scene;
	GLuint scene_list;
	C_STRUCT aiVector3D scene_min, scene_max, scene_center;
};

// --------- precompiler declarations

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

//----------- Global variables

GLfloat xmove_1, ymove_1, zmove_1 = 0;		//Pfeilmotion Translatef-Parameter f�r state ==1
GLfloat xmove_2, ymove_2, zmove_2 = 0;		//Pfeilmotion Translatef-Parameter f�r state ==2

GLfloat crossmotion = 0;						//Drehparameter des Kreuzes

// deinition of coordinates
enum coord { X, Y, Z };

//-------- the global Assimp scene -----------
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
MODEL leekGreen("../Models/leekGreen.stl");
MODEL leekWhite("../Models/leekWhite.stl");
MODEL leekGreenCut("../Models/leekGreenCut.stl");
MODEL leekWhiteCut("../Models/leekWhiteCut.stl");

MODEL sink("../Models/sink.stl");
MODEL sinkDoor("../Models/sinkDoor.stl");
MODEL sinkFaucet("../Models/sinkFaucet.stl");

MODEL stove("../Models/stove.stl");
MODEL stoveBlack("../Models/stoveBlack.stl");
MODEL stoveDoor("../Models/stoveDoor.stl");

MODEL table("../Models/table.obj");
MODEL bowlLower("../Models/bowlLower.stl");
MODEL bowlUpper("../Models/bowlUpper.stl");
MODEL bowlInner("../Models/bowlInner.stl");
MODEL spoon("../Models/spoon.stl");

extern int state;	// extern definition of the variable

//-------- For use in drawing --------------
static float gDrawRotateAngle = 0;			
static int	 gDrawRotate = FALSE;

//----------- Global functions ---------------

int loadasset(aiVector3D* scene_min, aiVector3D* scene_max, const C_STRUCT aiScene* scene, aiVector3D* scene_center);
void get_bounding_box_for_node(const C_STRUCT aiNode* nd,
	C_STRUCT aiVector3D* min,
	C_STRUCT aiVector3D* max,
	C_STRUCT aiMatrix4x4* trafo,
	const aiScene* scene);

	void get_bounding_box(C_STRUCT aiVector3D* min, C_STRUCT aiVector3D* max, const aiScene* C_STRUCT scene);

int loadasset(aiVector3D* scene_min, aiVector3D* scene_max, const C_STRUCT aiScene* scene, aiVector3D* scene_center);

void get_bounding_box_for_node(
	const C_STRUCT aiNode* nd,
	C_STRUCT aiVector3D* min,
	C_STRUCT aiVector3D* max,
	C_STRUCT aiMatrix4x4* trafo,
	const aiScene* scene);

void get_bounding_box(C_STRUCT aiVector3D* min, C_STRUCT aiVector3D* max, const aiScene* C_STRUCT scene);

void scale_center_model(MODEL model, ai_real x, ai_real y, ai_real z);

void color4_to_float4(const C_STRUCT aiColor4D* c, float f[4]);

void set_float4(float f[4], float a, float b, float c, float d);

void apply_material(const C_STRUCT aiMaterial* mtl);

void recursive_render(const C_STRUCT aiScene* sc, const C_STRUCT aiNode* nd);

void DrawArrow(static short direction);					// Funktionierender Pfeil, ausgerichtet, mit Motion

void ArrowMotion(float timeDelta);						// Updates the motion of the arrow

void DrawTeapot(void);									// Draws the Teapot

void DrawCheck(void);									//Draw Check

void DrawCross(void);									//Draw cross

void CrossMotion(float timeDelta);						// update the motion parameters of the cross

void drawText(const char *text, int length, int x, int y); // funtion to draw text

void DrawSink(void);

void DrawStove(void);

void DrawBoardFishCutKnife(void);

void DrawBoardCarrotCutKnife(void);

void DrawBoardLeekCutKnife(void);

void DrawBoardCarrotFishLeek(void);

void DrawPotWaterOnStove(void);

void DrawPotWaterInSink(void);

void DrawWashFood(void);

void DrawSoupDone(void);

void DrawFoodInStove(void);

void DrawServeFood(void);


// ============================================================================
//	Global definitions
// ============================================================================

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
	// free space to avoid ressource leakage (assimp)
	//aiReleaseImport(this->scene);
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

void scale_center_model(MODEL model, ai_real x, ai_real y, ai_real z) {
	float tmp;
	tmp = model.scene_max.x - model.scene_min.x;
	tmp = aisgl_max(model.scene_max.y - model.scene_min.y, tmp);
	tmp = aisgl_max(model.scene_max.z - model.scene_min.z, tmp);
	tmp = 1.5f / tmp;
	glScalef(tmp, tmp, tmp);

	// center the model 
	glTranslatef(-model.scene_center.x + x, -model.scene_center.y + y, -model.scene_center.z + z);
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
				//Auskommentiert; Farbe mit glcolor3f(); vor aufruf dieser Funktion setzen
				//if (mesh->mColors[0] != NULL)
					//glColor4fv((GLfloat*)&mesh->mColors[0][index]);
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

//----------- Drawing --------------------------

//Draw model and scale to scene; model - scale - translate - color
void draw(
	MODEL* model,
	GLfloat scaleX,
	GLfloat scaleY,
	GLfloat scaleZ,
	GLfloat transX,
	GLfloat transY,
	GLfloat transZ,
	GLfloat colorR,
	GLfloat colorG,
	GLfloat colorB)
{
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	glTranslatef(transX, transY, transZ);
	scale_center_model(*model, 1, 1, 1);
	glScalef(scaleX, scaleY, scaleZ);
	glColor3f(colorR, colorG, colorB);
	recursive_render(model->scene, model->scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.
}

GLfloat upDownMovement(GLfloat min, GLfloat max, GLfloat stepSize, int* move, GLfloat* oldValue) {
	static int state = 0; // 0 = up; 1 = down
	static GLfloat prevValue = 0;
	static unsigned int prevTime = 0;
	unsigned int msTime;

	msTime = glutGet(GLUT_ELAPSED_TIME);

	if (prevTime == 0)
	{
		prevTime = msTime;
	}

	if (msTime - prevTime > 20)
	{
		prevValue += stepSize;
		prevTime = msTime;
	}

	if (max - min < prevValue)
	{
		if (state)
			state = 0;
		else
			state = 1;

		prevValue = 0;
	}

	if (state)
	{
		return (max - prevValue);
	}
	else
	{
		return (min + prevValue);
	}
}

GLfloat transMovment(GLfloat min, GLfloat max, GLfloat stepSize, int* move, GLfloat* oldValue) {
	//static int state = 0; // 0 = up; 1 = down
	//static GLfloat prevValue = 0;
	static unsigned int prevTime = 0;
	unsigned int msTime;
	GLfloat absoluteValue = max - min;

	msTime = glutGet(GLUT_ELAPSED_TIME);

	if (prevTime == 0)
	{
		prevTime = msTime;
	}

	if (msTime - prevTime > 20)
	{
		*oldValue += stepSize;
		prevTime = msTime;

	}

	if (absoluteValue < 0)
	{
		absoluteValue = absoluteValue * -1;
	}

	if (absoluteValue < *oldValue)
	{
		if (*move)
			*move = 0;
		else
			*move = 1;

		*oldValue = 0;
	}

	if (*move)
	{
		return (max - *oldValue);
	}
	else
	{
		return (min + *oldValue);
	}
}

//****** models and scenes ******//

void DrawSink(void)
{
	// ***Sink***
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 1.0);
	recursive_render(sink.scene, sink.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 0.0, 0.0);
	recursive_render(sinkDoor.scene, sinkDoor.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 0.0);
	recursive_render(sinkFaucet.scene, sinkFaucet.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.
}

void DrawStove(void)
{
	// ***stove***
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 1.0);
	recursive_render(stove.scene, stove.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	glScalef(0.03, 0.03, 0.03);
	glColor3f(0.0, 0.0, 0.0);
	recursive_render(stoveBlack.scene, stoveBlack.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 0.0);
	recursive_render(stoveDoor.scene, stoveDoor.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.
}

void DrawBoardFishCutKnife(void)
{
	// ***board, cut fish, knife***
	//board
	draw(&board, 2, 2, 2, 0, 0, 0, 0.482, 0.192, 0.058);

	//fish
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(fishCut, 1.0, 1.0, 1.0);
	glScalef(1.0, 1.0, 1.0);
	glTranslatef(-1.0, -1.25, -0.85);
	glColor3f(0.929, 0.568, 0.129);
	recursive_render(fishCut.scene, fishCut.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	//knife
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(knife, 1.0, 1.0, 1.0);
	glTranslatef(-0.8, -80.0, 100.0);
	glColor3f(0.349, 0.529, 0.486);
	recursive_render(knife.scene, knife.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.
}

void DrawBoardCarrotCutKnife(void)
{
	static int moveDirection = 0;
	static GLfloat lastValue = 0;
	// ***board, cut carrot, knife***

	draw(&board, 2, 2, 2, 0, 0, 0, 0.482, 0.192, 0.058);

	//carrot
	draw(&carrotCut, 1.5, 1, 1, 0, -0.2, 0.1, 0.929, 0.568, 0.129);

	//knife
	draw(&knife, 1.2, 1.2, 1.2, -0.1, -0.5, upDownMovement(0.2, 0.6, 0.01, &moveDirection, &lastValue), 0.831, 0.847, 0.945);
}

void DrawBoardLeekCutKnife(void)
{
	static int moveDirection = 0;
	static GLfloat lastValue = 0;
	// ***board, cut carrot, knife***
	draw(&board, 2, 2, 2, 0, 0, 0, 0.482, 0.192, 0.058);

	//leek
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//scale_center_model(fish, 1.0, 1.0, 1.0);
	glTranslatef(-0.3, 0.0, 0.2);
	glScalef(0.025, 0.015, 0.015);
	glColor3f(0, 0.564, 0.352);
	recursive_render(leekGreenCut.scene, leekGreenCut.scene->mRootNode);	//render Model
	glColor3f(0.901, 0.917, 0.905);
	recursive_render(leekWhiteCut.scene, leekWhiteCut.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	//knife
	draw(&knife, 1.2, 1.2, 1.2, -0.1, -0.5, upDownMovement(0.2, 0.6, 0.01, &moveDirection, &lastValue), 0.831, 0.847, 0.945);
}

void DrawBoardCarrotFishLeek(void)
{
	static int moveDirection = 0;
	static GLfloat lastValue = 0;
	// ***load board, carrot, fish***
	//chopping board
	draw(&board, 2, 2, 2, 0.0, 0.0, 0.0, 0.482, 0.192, 0.058);

	//carrot
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(carrot, 1.0, 1.0, 1.0);
	glScalef(1.5, 1.0, 1.0);
	glTranslatef(0.0, -35.0, 10.0);
	glColor3f(0.929, 0.568, 0.129);
	recursive_render(carrot.scene, carrot.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	//leek
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//scale_center_model(fish, 1.0, 1.0, 1.0);
	glTranslatef(-0.1, -0.2, 0.2);
	glScalef(0.025, 0.015, 0.015);
	glColor3f(0, 0.564, 0.352);
	recursive_render(leekGreen.scene, leekGreen.scene->mRootNode);	//render Model
	glColor3f(0.901, 0.917, 0.905);
	recursive_render(leekWhite.scene, leekWhite.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	//fish
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(fish, 1.0, 1.0, 1.0);
	glScalef(1.5, 1.0, 1.0);
	glTranslatef(-0.6, -0.6, -0.85);
	glColor3f(0.349, 0.529, 0.486);
	recursive_render(fish.scene, fish.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.
}

void DrawPotWaterOnStove(void)
{
	// Draw stove
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 1.0);
	recursive_render(stove.scene, stove.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glColor3f(0.0, 0.0, 0.0);
	recursive_render(stoveBlack.scene, stoveBlack.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 0.0);
	recursive_render(stoveDoor.scene, stoveDoor.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	//Draw pot with water on stove
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(pot, 1.0, 1.0, 1.0);
	glTranslatef(45, -10.0, 372.0);
	glScalef(0.65, 0.65, 0.65);
	glColor3f(0.662, 0.662, 0.662);
	recursive_render(pot.scene, pot.scene->mRootNode);	//render Model

	glColor3f(0.447, 0.807, 0.952);
	recursive_render(potWater.scene, potWater.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

}

void DrawPotWaterInSink(void)
{
	// Draw sink
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 1.0);
	recursive_render(sink.scene, sink.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 0.0, 0.0);
	recursive_render(sinkDoor.scene, sinkDoor.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 0.0);
	recursive_render(sinkFaucet.scene, sinkFaucet.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	//Draw pot with water on stove
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(pot, 1.0, 1.0, 1.0);
	glTranslatef(-65.0, 10.0, 220.0);
	glScalef(0.8, 0.8, 0.8);
	glColor3f(0.662, 0.662, 0.662);
	recursive_render(pot.scene, pot.scene->mRootNode);	//render Model

	glColor3f(0.447, 0.807, 0.952);
	recursive_render(potWater.scene, potWater.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

}

void DrawWashFood(void)
{
	DrawSink();

	//chopping board
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(board, 1.0, 1.0, 1.0);
	glTranslatef(110.0, 0.0, 340.0);
	glScalef(1.0, 1.0, 1.0);
	glColor3f(0.482, 0.192, 0.058);
	recursive_render(board.scene, board.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	//carrot
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(carrot, 1.0, 1.0, 1.0);
	glScalef(1.0, 0.6, 1.0);
	glTranslatef(-10.0, -10.0, 100.0);
	glRotatef(90, 0, 0, 1);
	glColor3f(0.929, 0.568, 0.129);
	recursive_render(carrot.scene, carrot.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	//leek
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//scale_center_model(fish, 1.0, 1.0, 1.0);
	glTranslatef(-0.8, 0.15, 2.0);
	glScalef(0.01, 0.012, 0.015);
	glRotatef(-90, 0, 0, 1);
	glColor3f(0, 0.564, 0.352);
	recursive_render(leekGreen.scene, leekGreen.scene->mRootNode);	//render Model
	glColor3f(0.901, 0.917, 0.905);
	recursive_render(leekWhite.scene, leekWhite.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.
}

void DrawSoupDone(void)
{
	// Draw stove
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 1.0);
	recursive_render(stove.scene, stove.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glColor3f(0.0, 0.0, 0.0);
	recursive_render(stoveBlack.scene, stoveBlack.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	//glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glColor3f(1.0, 1.0, 0.0);
	recursive_render(stoveDoor.scene, stoveDoor.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

	//Draw pot with water on stove
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(pot, 1.0, 1.0, 1.0);
	glTranslatef(45, -10.0, 372.0);
	glScalef(0.65, 0.65, 0.65);
	glColor3f(0.662, 0.662, 0.662);
	recursive_render(pot.scene, pot.scene->mRootNode);	//render Model

	glColor3f(0.964, 0.705, 0.003);
	recursive_render(potWater.scene, potWater.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.

}

void DrawFoodInPot(void)
{
	// leek position
	GLfloat start[] = { 0.6, 0.05, 4 };
	GLfloat endZ = 1.5;
	GLfloat angle[] = { 90,0,90 };
	GLfloat static oldValue = 0;
	int direction = 1;
	int static turning = 1;
	GLfloat static rotated[3] = { 0,0,0 };
	GLfloat stepWidth = 0.05;

	DrawStove();

	//Draw pot with water on stove
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	scale_center_model(pot, 1.0, 1.0, 1.0);
	glTranslatef(45, -10.0, 372.0);
	glScalef(0.65, 0.65, 0.65);
	glColor3f(0.662, 0.662, 0.662);
	recursive_render(pot.scene, pot.scene->mRootNode);	//render Model

	glColor3f(0.447, 0.807, 0.952);
	recursive_render(potWater.scene, potWater.scene->mRootNode);	//render Model
	glPopMatrix();  					// Restore world coordinate system.


	if (turning)
	{
		glTranslatef(start[X], start[Y], start[Z]); // translate over the stove

		if (rotated[X] < angle[X])
		{
			rotated[X]++;
		}
		if (rotated[Y] < angle[Y])
		{
			rotated[Y]++;
		}
		if (rotated[Z] < angle[Z])
		{
			rotated[Z]++;
		}
		glRotatef(rotated[X], 1, 0, 0);
		glRotatef(rotated[Y], 0, 1, 0);
		glRotatef(rotated[Z], 0, 0, 1);

		if (rotated[X] >= angle[X] && rotated[Y] >= angle[Y] && rotated[Z] >= angle[Z])
		{
			turning = 0;
			rotated[X] = 0;
			rotated[Y] = 0;
			rotated[Z] = 0;
		}
	}
	else
	{
		glTranslatef(start[X], start[Y], transMovment(endZ, start[Z], stepWidth, &direction, &oldValue)); // translate over the stove
		glRotatef(angle[X], 1, 0, 0);
		glRotatef(angle[Y], 1, 1, 0);
		glRotatef(angle[Z], 0, 0, 1);

		if (oldValue >= start[Z] - endZ - stepWidth)
		{
			turning = 1;
		}
	}

	//glPushMatrix();
	//Draw Leek
	draw(&leekGreenCut, 0.2, 0.2, 0.2, 0.2, 0.3, 0.1, 0, 0.564, 0.352);
	draw(&leekWhiteCut, 0.3, 0.3, 0.3, 1.5, 0.2, 0.1, 0.901, 0.917, 0.905);

	//Draw carrot
	draw(&carrotCut, 0.3, 0.3, 0.3, 0.3, 0.3, 0.4, 0.929, 0.568, 0.129);

	// Draw meat
	draw(&meatCut, 0.6, 0.6, 0.6, 0.3, 0.15, 0.3, 0.988, 0.337, 0.337);

	// Draw fish
	draw(&fishCut, 0.8, 0.8, 0.8, -0.5, -1, -0.8, 0.349, 0.529, 0.486);
	//glPopMatrix();
}

void DrawServeFood(void)
{
	glScalef(0.8, 0.8, 0.8);
	//draw table
	draw(&table, 5.0, 5.0, 5.0, 0, 0, 0, 1, 1, 1);

	//draw Bowl
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord
	glTranslatef(0.0, -1.8, 3.75);
	glScalef(0.01, 0.01, 0.01);
	//bowlUpper
	glColor3f(1.0, 1.0, 1.0);
	recursive_render(bowlUpper.scene, bowlUpper.scene->mRootNode);	//render Model

	//bownLower
	glColor3f(0.909, 0.745, 0.674);
	recursive_render(bowlLower.scene, bowlLower.scene->mRootNode);	//render Model

	//downInner
	glColor3f(0.964, 0.705, 0.003);
	recursive_render(bowlInner.scene, bowlInner.scene->mRootNode);	//render Model

	//spoon
	glColor3f(0.752, 0.752, 0.752);
	recursive_render(spoon.scene, spoon.scene->mRootNode);	//render Model
	glPopMatrix();					// Restore world coordinate system.
}

//Default model
void neuesModell()
{
	glLoadIdentity;
	glPushMatrix();					//Nullpunkt Weltkoord

	glTranslatef(0.0, 0.1, 0.0);	//translate y +0.1
	glRotatef(90.0, 0.0, 0.0, 1.0); //rotate 90 to left
	glScalef(0.1, 0.1, 0.1);		//scale 0,1

	glLineWidth(1.0);				//liniendicke
	glColor3f(0.0, 0.0, 0.5);		//color

	//***
	//Modell
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 1, 0);
	glEnd();
	//***

	glPopMatrix();					// Restore world coordinate system.
}

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
		if (xmove_2 > 0.5)					// an dieser Stelle ist es �berfl�ssig 2 mal move zu generieren, nur Abtrennungshalber existieren zwei
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
		xmove_1 = xmove_1 + 0.5f * timeDelta;
	if (state == 2)
		xmove_2 = xmove_2 + 0.5f * timeDelta;
}

void CrossMotion(float timeDelta)			// updating the crossmotion bei idle()
{
	crossmotion += timeDelta * 45.0f;		// Rotate Kreuz at 45 degrees per second.
	if (crossmotion > 360.0f)
		crossmotion -= 360.0f;
}

void drawText(const char* text, int length, int x, int y) // funtion to draw text
{

	glMatrixMode(GL_PROJECTION);
	double* matrix = new double[16];
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


static void releaseModels()
{
	// free space to avoid ressource leakage (assimp)
	aiReleaseImport(knife.scene);
	aiReleaseImport(fish.scene);
	aiReleaseImport(pot.scene);
	aiReleaseImport(lid.scene);
	aiReleaseImport(carrot.scene);
	aiReleaseImport(carrotCut.scene);
	aiReleaseImport(meatCut.scene);
	aiReleaseImport(meat.scene);
	aiReleaseImport(board.scene);
	aiReleaseImport(potWater.scene);
	aiReleaseImport(leekGreen.scene);
	aiReleaseImport(leekWhite.scene);
	aiReleaseImport(leekGreenCut.scene);
	aiReleaseImport(leekWhiteCut.scene);

	aiReleaseImport(sink.scene);
	aiReleaseImport(sinkDoor.scene);
	aiReleaseImport(sinkFaucet.scene);

	aiReleaseImport(stove.scene);
	aiReleaseImport(stoveBlack.scene);
	aiReleaseImport(stoveDoor.scene);

	aiReleaseImport(table.scene);
	aiReleaseImport(bowlLower.scene);
	aiReleaseImport(bowlUpper.scene);
	aiReleaseImport(bowlInner.scene);
	aiReleaseImport(spoon.scene);
}
#endif
