
//#pragma once

#ifndef __ModeUtils__
#define __ModeUtils__
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/freeglut.h>


#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)




// set the bounds of the node of the model
void get_bounding_box_for_node(const C_STRUCT aiNode* nd, C_STRUCT aiVector3D* min, C_STRUCT aiVector3D* max, C_STRUCT aiMatrix4x4* trafo, const aiScene* scene);

// set the bounds of the displayed area 
void get_bounding_box(C_STRUCT aiVector3D* min, C_STRUCT aiVector3D* max, const aiScene* C_STRUCT scene);

// convert a color array to a float array
void color4_to_float4(const C_STRUCT aiColor4D* c, float f[4]);

// converte single value to a array of an color
void set_float4(float f[4], float a, float b, float c, float d);

// set the surface of the models
void apply_material(const C_STRUCT aiMaterial* mtl);

// Render the imported models
void recursive_render(const C_STRUCT aiScene* sc, const C_STRUCT aiNode* nd);

// load the model to the scene
int loadasset(aiVector3D* scene_min, aiVector3D* scene_max, const C_STRUCT aiScene* scene, aiVector3D* scene_center);


class MODEL
{
public:
	MODEL(char* path_model);
	~MODEL();

	const C_STRUCT aiScene* scene;
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
	// free space to avoid ressource leakage (assimp)
	aiReleaseImport(this->scene);
}




#endif // !__ModeUtils__
