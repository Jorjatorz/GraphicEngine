#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Definitions.h"

#include <string>
#include <map>

class SceneManager
{
public:
	SceneManager(void);
	~SceneManager(void);

	//Shader
	//create, bind, unbind
	//Creates a new shader, if it already exists it return a pointer to it
	Shader* createShader(std::string shaderName, std::string shaderPath);
	//delete and delete from disc
	//delete the shader by name (from the scene)
	void deleteShader(std::string shaderName);
	//set the new currentSahder
	void setCurrentShader(Shader* newShader);
	//bind shader and set it to currentSahder
	void bindShader(Shader* mNewShader);
	void bindCurrentShader();
	//unbind shader and set currentoShader to null
	void unbindCurrentShader();
	//current binded Shader
	Shader* mCurrentShader;

	//Texture
	//create, etc
	//Creates a new texture, if it already exists it return a pointer to it
	Texture* createTexture(std::string textureName, bool mipmap, std::string texturePath);
	//delete texture by name (from the scene)
	void deleteTexture(std::string textueName);

	//Entity
	//creates a new entity
	Entity* createEntity(std::string entityName, std::string meshPath);
	//delete an entity by name (from scene)
	void deleteEntity(std::string entityName);


	//Light
	//etc


	//maps
	typedef std::map<std::string, Shader*> tShaderMap;
	typedef std::map<std::string, Texture*> tTextureMap;
	typedef std::map<std::string, Entity*> tEntityMap;

	//define maps
	tShaderMap mShaderMap;
	tTextureMap mTextureMap;
	tEntityMap mEntityMap;
};

#endif