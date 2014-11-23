#include "Renderable.h"


Renderable::Renderable(void)
{
}

void Renderable::loadObject(std::string filename, std::string texDir)
{
	//Assimp interface
    Assimp::Importer Importer;

	//aiScene contains all the meshes in scene. It is devided into aiMesh objects. 
    const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	
	numberOfObjects = pScene->mNumMeshes;

    if (pScene)
	{
		meshVector.resize(pScene->mNumMeshes);

		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0 ; i < meshVector.size() ; i++)
		{
			//aiMesh corresponds to one of the meshes in scene
			const aiMesh* paiMesh = pScene->mMeshes[i];
			initMesh(i, paiMesh);
		}

		initMaterials(pScene, texDir);
    }
    else {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());
    }
}

void Renderable::initMesh(GLuint Index, const aiMesh* paiMesh)
{
    meshVector.at(Index).MaterialIndex = Index;
    
    std::vector<Vertex> Vertices;
    std::vector<GLuint> Indices;

	//In the case there is no texture
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	//Loads all vertices in mesh
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++)
	{
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		//Converts from aiVector3D to glm
        Vertex v;
		v.position = glm::vec3(pPos->x, pPos->y, pPos->z);
        v.normal = glm::vec3(pNormal->x, pNormal->y, pNormal->z);
		v.texture = glm::vec2(pTexCoord->x, pTexCoord->y);

        Vertices.push_back(v);
    }

	/*Creates indices for drawing. This works only for triangulated mesh, but triangulation should be done by Assimp. But pretriangulated mesh
	* should loads faster than with runtime triangulation.
	*/
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++)
	{
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);		
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    meshVector.at(Index).Init(Vertices, Indices);
}

void Renderable::initMaterials(const aiScene* pScene, const std::string& dir)
{
	for(int i=0; i < numberOfObjects; i++)
	{
		texVector.push_back(Texture::createTexture(dir+"/"+std::to_string(i)+".png"));	//Ci¹gnik samolotowy, hehe. 
	}
}

void Renderable::render()
{
	for(int i = 0; i < numberOfObjects; i++)
	{
		if(texVector.at(i) != nullptr)	// Objects without texture should not be shown. Something went really wrong. 
		{
			glBindTexture(GL_TEXTURE_2D, texVector[i]->getTexture());
			meshVector.at(i).render();
		}
	}
}

void Renderable::repeatTexture(bool repeat)
{
	for(int i = 0; i < numberOfObjects; i++)
	{
		if(texVector.at(i) != nullptr)
		{
			texVector[i]->repeatTexture(repeat);
		}
	}
}

Renderable::~Renderable(void)
{
}
