#include "ResourceManager.h"

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"

ResourceManager::ResourceManager(void)
{
}


ResourceManager::~ResourceManager(void)
{
	//delete maps
	//shader map
	tShaderMap::iterator shaderIterator;

	for(shaderIterator = mShaderMap.begin(); shaderIterator != mShaderMap.end(); ++shaderIterator)
	{
		delete shaderIterator->second;
	}
	mShaderMap.clear();

	//Texture map
	tTextureMap::iterator textureIterator;

	for(textureIterator = mTextureMap.begin(); textureIterator != mTextureMap.end(); ++textureIterator)
	{
		delete textureIterator->second;
	}
	mTextureMap.clear();

	//Mesh map
	tMeshMap::iterator meshIterator;

	for(meshIterator = mMeshMap.begin(); meshIterator != mMeshMap.end(); ++meshIterator)
	{
		delete meshIterator->second;
	}
	mMeshMap.clear();

	//Delete VBO and VAO
	glDeleteVertexArrays(1, &screenSquareVAO);
	glDeleteBuffers(1, &screenSquareVBO);
}

Shader* ResourceManager::loadShader(std::string shaderName, std::string shaderPath)
{
	//THE SHADERPATH MUST BE THE NAME OF THE SHADER WITHOUT EXTENSION
	//search if the resource already exists
	tShaderMap::iterator it = mShaderMap.find(shaderName);
	if(it != mShaderMap.end())
	{
		return it->second->getResource();
	}

	//if the resource doesnt exists
	std::string completePath = "Data\\Shaders\\" + shaderPath;
	//create Shader (will be deleted by the resource)
	Shader* mShader = new Shader(shaderName);
	//create a new resource(will be deleted by the resourceManager)
	GameResource<Shader*>* mResource = new GameResource<Shader*>(completePath, mShader);

	//load shader and information
	mShader->loadShader(completePath);
	mResource->lastModificationTime[0] = getModificationTime(completePath + ".vert");
	mResource->lastModificationTime[1] = getModificationTime(completePath + ".frag");

	//insert it into the map
	mShaderMap.insert(std::pair<std::string, GameResource<Shader*>*>(shaderName, mResource));

	return mShader;

}

Shader* ResourceManager::getShader(std::string shaderName)
{
	tShaderMap::iterator it = mShaderMap.find(shaderName);
	if(it != mShaderMap.end())
	{
		return it->second->getResource();
	}

	return NULL;
}

Texture* ResourceManager::loadTexture(std::string texturePath, bool mipmap, GLint format)
{

	//search if the resource already exists
	tTextureMap::iterator it = mTextureMap.find(texturePath);
	//if it exist return it
	if(it != mTextureMap.end())
	{
		return it->second->getResource();
	}


	std::string completePath = "Data\\Textures\\" + texturePath;
	
	//create Texture (will be deleted by the resource)
	Texture* mTexture = new Texture();
	//create new resource(will be deleted by the resourceManager)
	GameResource<Texture*>* mResource = new GameResource<Texture*>(completePath, mTexture);

	//load texture and information
	mTexture->loadTexture(completePath, mipmap, format);
	mResource->lastModificationTime[0] = getModificationTime(completePath);

	//insert it into the map
	mTextureMap.insert(std::pair<std::string, GameResource<Texture*>*>(texturePath, mResource));

	return mTexture;

}

Texture* ResourceManager::createTexture(std::string textureName, int width, int height, bool mipmap, GLuint format)
{
	//search if the resource already exists
	tTextureMap::iterator it = mTextureMap.find(textureName);
	//if it exist return it
	if (it != mTextureMap.end())
	{
		return it->second->getResource();
	}

	//create Texture (will be deleted by the resource)
	Texture* mTexture = new Texture();
	mTexture->generateTexture(width, height, format, mipmap, NULL);
	//create new resource(will be deleted by the resourceManager)
	GameResource<Texture*>* mResource = new GameResource<Texture*>("NULL", mTexture);

	//insert it into the map
	mTextureMap.insert(std::pair<std::string, GameResource<Texture*>*>(textureName, mResource));

	return mTexture;
}

Texture* ResourceManager::getTexture(std::string textureName)
{
	//search if the resource already exists
	tTextureMap::iterator it = mTextureMap.find(textureName);
	//if it exist return it
	if(it != mTextureMap.end())
	{
		return it->second->getResource();
	}

	return NULL;
}

//load entity
Mesh* ResourceManager::loadMesh(std::string meshName, std::string meshPath, SceneManager* sceneManager)
{
	//search if an mesh with that name already exists
	tMeshMap::iterator it = mMeshMap.find(meshName);
	if(it != mMeshMap.end())
	{
		return it->second->getResource();
	}

	//else
	std::string completePath = "Data\\Models\\" + meshPath;
	//create and load the mesh (will be deleted by the resource)
	Mesh* mMesh = new Mesh(completePath, sceneManager);
	//create new resource(will be deleted by the resourceManager)
	GameResource<Mesh*>* mResource = new GameResource<Mesh*>( completePath, mMesh);

	//insert it to the map
	mMeshMap.insert(std::pair<std::string, GameResource<Mesh*>*>(meshName, mResource));

	//return it
	return mMesh;
}

//return the shader
Mesh* ResourceManager::getMesh(std::string meshName)
{
	//search if the resource already exists
	tMeshMap::iterator it = mMeshMap.find(meshName);
	if(it != mMeshMap.end())
	{
		return it->second->getResource();
	}

	//if not found return null
	return NULL;
}

Material* ResourceManager::loadMaterial(std::string materialName, std::string materialPath, SceneManager* manager)
{
	//search if an mesh with that name already exists
	tMaterialMap::iterator it = mMaterialMap.find(materialName);
	if(it != mMaterialMap.end())
	{
		return it->second->getResource();
	}

	//else
	std::string completePath = "Data\\Materials\\" + materialPath;

	//create and load the mesh (will be deleted by the resource)
	Material* mMat = new Material(materialName);
	mMat->setSceneManager(manager);
	mMat->readMaterialFromFile(completePath);

	//create new resource(will be deleted by the resourceManager)
	GameResource<Material*>* mResource = new GameResource<Material*>( completePath, mMat);

	//insert it to the map
	mMaterialMap.insert(std::pair<std::string, GameResource<Material*>*>(materialName, mResource));

	//return it
	return mMat;
}

Material* ResourceManager::createMaterial(std::string materialName, SceneManager* manager)
{
	//search if an mesh with that name already exists
	tMaterialMap::iterator it = mMaterialMap.find(materialName);
	if(it != mMaterialMap.end())
	{
		return it->second->getResource();
	}

	//else
	//create and load the mesh (will be deleted by the resource)
	Material* mMat = new Material(materialName);
	mMat->setSceneManager(manager);
	//create new resource(will be deleted by the resourceManager)
	GameResource<Material*>* mResource = new GameResource<Material*>( "NULL", mMat);

	//insert it to the map
	mMaterialMap.insert(std::pair<std::string, GameResource<Material*>*>(materialName, mResource));

	//return it
	return mMat;
}

Material* ResourceManager::getMaterial(std::string materialName)
{
	tMaterialMap::iterator it = mMaterialMap.find(materialName);
	if(it != mMaterialMap.end())
	{
		return it->second->getResource();
	}

	//if we ddint find the mat return NULL
	return NULL;
}

time_t ResourceManager::getModificationTime(std::string filePath)
{
	//will contain the information of the file
	struct stat fileInfo;

	//get the information of the file
	stat(filePath.c_str(), &fileInfo);

	//save the modification time
	return fileInfo.st_mtime;
}

void ResourceManager::checkForModifications()
{
	//for all components in the map
	tShaderMap::iterator shaderIterator;
	for(shaderIterator = mShaderMap.begin(); shaderIterator != mShaderMap.end(); ++shaderIterator)
	{
		//check if the time of modifications isnt the same
		if(shaderIterator->second->lastModificationTime[0] != getModificationTime(shaderIterator->second->getFilePath() + ".vert"))
		{
			//reload the file
			shaderIterator->second->getResource()->loadShader(shaderIterator->second->getFilePath());
			shaderIterator->second->lastModificationTime[0] = getModificationTime(shaderIterator->second->getFilePath() + ".vert");
		}

		if(shaderIterator->second->lastModificationTime[1] != getModificationTime(shaderIterator->second->getFilePath() + ".frag"))
		{
			//reload the file
			shaderIterator->second->getResource()->loadShader(shaderIterator->second->getFilePath());
			shaderIterator->second->lastModificationTime[1] = getModificationTime(shaderIterator->second->getFilePath() + ".frag");
		}
	}

	
	tTextureMap::iterator textureIterator;
	for(textureIterator = mTextureMap.begin(); textureIterator != mTextureMap.end(); ++textureIterator)
	{
		//If the texture is laoded from disk
		if (textureIterator->second->getFilePath() != "NULL")
		{
			if (textureIterator->second->lastModificationTime[0] != getModificationTime(textureIterator->second->getFilePath()))
			{
				//reload the file
				textureIterator->second->getResource()->loadTexture(textureIterator->second->getFilePath(), textureIterator->second->getResource()->isMipmap, textureIterator->second->getResource()->mFormat);
				textureIterator->second->lastModificationTime[0] = getModificationTime(textureIterator->second->getFilePath());
			}
		}
	}

	tMaterialMap::iterator materialIterator;
	for(materialIterator = mMaterialMap.begin(); materialIterator != mMaterialMap.end(); ++materialIterator)
	{
		if(materialIterator->second->lastModificationTime[0] != getModificationTime(materialIterator->second->getFilePath()))
		{
			materialIterator->second->getResource()->readMaterialFromFile(materialIterator->second->getFilePath());
			materialIterator->second->lastModificationTime[0] = getModificationTime(materialIterator->second->getFilePath());
		}
	}
}

void ResourceManager::createScreenQuad()
{
	//vertices
	const GLfloat vertex_positions[] =
	{
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &screenSquareVAO);
	glBindVertexArray(screenSquareVAO);

	glGenBuffers(1, &screenSquareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenSquareVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

	glVertexAttribPointer(Shader::VERTEXPOSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glVertexAttribPointer(Shader::VERTEXTEXCOORD, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}