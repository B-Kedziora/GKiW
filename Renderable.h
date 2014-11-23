#pragma once
#include <string>
#include "AssimpIncludes\Importer.hpp"
#include "AssimpIncludes\scene.h"
#include "AssimpIncludes\postprocess.h"
#include "Texture.h"
#include "SingleMesh.h"
#include <vector>

/*
*	Uses assimp import library. It represents renderable object. One object can contains one or more meshes.
*	There must be one texture per mesh (no more, no less). In other case mesh will not be displayed or textures
*	will be messed up. You can set whether texture should be repeated or clamped for meshes in object
*	Usage:
*	-Create object using constructor
*	-invoke loadObject()
*	-invoke render()
*/

class Renderable
{
public:
	/*Loads object into GPU memory. Paths are relative. filename is the name of .obj with path (or other format file). texDir is the directory of textures*/
	void loadObject(std::string filename, std::string texDir);

	/*Calls render() for all meshes*/
	void render();

	/*true for repeated texture, false for clamped*/
	void repeatTexture(bool repeat);
	Renderable(void); //Temp tutaj

private:
	/*Loads single mesh*/
	void initMesh(GLuint index, const aiMesh* paiMesh);

	/*Loads textures*/
	void initMaterials(const aiScene* pScene, const std::string& Filename);

	/*Stores number of meshes in the object*/
	GLint numberOfObjects;
	
	~Renderable(void);

	std::vector<SingleMesh> meshVector; //Meshes
    std::vector<Texture*> texVector;	//Textures
};

