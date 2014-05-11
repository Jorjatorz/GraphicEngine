#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_h

#include "Definitions.h"
#include "Singleton.h"
#include "GameResource.h"

#include <string>
#include <map>

class ResourceManager : public Singleton<ResourceManager>
{
public:
	ResourceManager(void);
	~ResourceManager(void);

	//shaders
	//load shader
	Shader* loadShader(std::string shaderName, std::string shaderPath);
	//return the shader
	Shader* getShader(std::string shaderName);

	//Textures
	//load texture
	Texture* loadTexture(std::string textureName, bool mipmap, std::string texturePath);
	//return the texture
	Texture* getTexture(std::string textureName);

	//Meshes
	//load mesh from file
	Mesh* loadMesh(std::string meshName, std::string meshPath);
	//return the shader
	Mesh* getMesh(std::string meshName);

	//Materials
	Material* loadMaterial(std::string materialName, std::string materialPath);
	Material* createMaterial(std::string materialName); //creates a material (the material is not loaded from file)

	Material* getMaterial(std::string materialName);

	//AUX
	//return the modification date of the file
	time_t getModificationTime(std::string filePath);
	//check for modifications of the files
	void checkForModifications();


private:
	typedef std::map<std::string, GameResource<Shader*>*> tShaderMap;
	typedef std::map<std::string, GameResource<Texture*>*> tTextureMap;
	typedef std::map<std::string, GameResource<Mesh*>*> tMeshMap;
	typedef std::map<std::string, GameResource<Material*>*> tMaterialMap;
	
	tShaderMap mShaderMap;
	tTextureMap mTextureMap;
	tMeshMap mMeshMap;
	tMaterialMap mMaterialMap;

};

#endif