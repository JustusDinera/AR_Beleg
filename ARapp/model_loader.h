#pragma once
#ifndef _MODEL_LOADER_
#define _MODEL_LOADER_


//--------- assimp libs-------------
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//--------- OpenGL libs-------------
#include <GL/glut.h>


/* the global Assimp scene object */
const C_STRUCT aiScene* scene = NULL;
GLuint scene_list = 0;
C_STRUCT aiVector3D scene_min, scene_max, scene_center;

void get_bounding_box_for_node(const C_STRUCT aiNode* nd,
	C_STRUCT aiVector3D* min,
	C_STRUCT aiVector3D* max,
	C_STRUCT aiMatrix4x4* trafo
);

void get_bounding_box(C_STRUCT aiVector3D* min, C_STRUCT aiVector3D* max);

void color4_to_float4(const C_STRUCT aiColor4D* c, float f[4]);

void set_float4(float f[4], float a, float b, float c, float d);

void apply_material(const C_STRUCT aiMaterial* mtl);

void recursive_render(const C_STRUCT aiScene* sc, const C_STRUCT aiNode* nd);

int loadasset(const char* path);

#endif // !_MODEL_LOADER_
