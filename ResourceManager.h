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

	//meshes
	//load mesh from file
	Mesh* loadMeshFromFile(std::string meshName, std::string meshPath);
	//allocate new mesh from a pointer
	Mesh* createMesh(std::string meshName, std::string meshPath);
	//return the shader
	Mesh* getMesh(std::string meshName);

	//AUX
	//return the modification date of the file
	time_t getModificationTime(std::string filePath);
	//check for modifications of the files
	void checkForModifications();


private:
	typedef std::map<std::string, GameResource<Shader*>*> tShaderMap;
	typedef std::map<std::string, GameResource<Texture*>*> tTextureMap;
	typedef std::map<std::string, GameResource<Mesh*>*> tMeshMap;
	
	tShaderMap mShaderMap;
	tTextureMap mTextureMap;
	tMeshMap mMeshMap;

};

#endif