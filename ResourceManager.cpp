#include "ResourceManager.h"

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

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
}

Shader* ResourceManager::loadShader(std::string shaderName, std::string shaderPath)
{
	//search if the resource already exists
	tShaderMap::iterator shaderIterator;
	for(shaderIterator = mShaderMap.begin(); shaderIterator != mShaderMap.end(); ++shaderIterator)
	{
		//if already exists, excists true
		if(shaderIterator->second->getFilePath() == shaderPath)
		{
			return shaderIterator->second->getResource();
		}
	}

	//if the resource doesnt exists
	//create Shader (will be deleted by the resource)
	Shader* mShader = new Shader();
	//create a new resource(will be deleted by the resourceManager)
	GameResource<Shader*>* mResource = new GameResource<Shader*>(shaderPath, mShader);

	//load shader and information
	mShader->loadShader(shaderPath);
	mResource->lastModificationTime[0] = getModificationTime(shaderPath + ".vert");
	mResource->lastModificationTime[1] = getModificationTime(shaderPath + ".frag");

	//insert it into the map
	mShaderMap.insert(std::pair<std::string, GameResource<Shader*>*>(shaderName, mResource));

	return mShader;

}

Shader* ResourceManager::getShader(std::string shaderName)
{
	//search if the resource exists and return it
	tShaderMap::iterator shaderIterator;
	for(shaderIterator = mShaderMap.begin(); shaderIterator != mShaderMap.end(); ++shaderIterator)
	{
		//if exists return it
		if(shaderIterator->first == shaderName)
		{
			return shaderIterator->second->getResource();
		}
	}

	return NULL;
}

Texture* ResourceManager::loadTexture(std::string textureName, bool mipmap, std::string texturePath)
{

	//search if the resource already exists
	tTextureMap::iterator textureIterator;
	for(textureIterator = mTextureMap.begin(); textureIterator != mTextureMap.end(); ++textureIterator)
	{
		//if already exists, excists true
		if(textureIterator->second->getFilePath() == texturePath)
		{
			return textureIterator->second->getResource();
		}
	}

	//create Texture (will be deleted by the resource)
	Texture* mTexture = new Texture();
	//create new resource(will be deleted by the resourceManager)
	GameResource<Texture*>* mResource = new GameResource<Texture*>(texturePath, mTexture);

	//load texture and information
	mTexture->loadTexture(texturePath, mipmap);
	mResource->lastModificationTime[0] = getModificationTime(texturePath);

	//insert it into the map
	mTextureMap.insert(std::pair<std::string, GameResource<Texture*>*>(textureName, mResource));

	return mTexture;

}

Texture* ResourceManager::getTexture(std::string textureName)
{
	//search if the resource already exists
	tTextureMap::iterator textureIterator;
	for(textureIterator = mTextureMap.begin(); textureIterator != mTextureMap.end(); ++textureIterator)
	{
		//if exist return it
		if(textureIterator->first == textureName)
		{
			return textureIterator->second->getResource();
		}
	}

	return NULL;
}

//load entity
Mesh* ResourceManager::loadMeshFromFile(std::string meshName, std::string meshPath)
{
	//search if an mesh with that name already exists
	tMeshMap::iterator meshIterator;
	for(meshIterator = mMeshMap.begin(); meshIterator != mMeshMap.end(); ++meshIterator)
	{
		if(meshIterator->first == meshName)
		{
			return meshIterator->second->getResource();
		}
	}
	//else
	//create the mesh (will be deleted by the resource)
	Mesh* mMesh = new Mesh();
	//create new resource(will be deleted by the resourceManager)
	GameResource<Mesh*>* mResource = new GameResource<Mesh*>(meshPath, mMesh);

	//load mesh

	//insert it to the map
	mMeshMap.insert(std::pair<std::string, GameResource<Mesh*>*>(meshName, mResource));

	//return it
	return mMesh;
}

Mesh* ResourceManager::createMesh(std::string meshName, std::string meshPath)
{
	//search if an mesh with that name already exists
	tMeshMap::iterator meshIterator;
	for(meshIterator = mMeshMap.begin(); meshIterator != mMeshMap.end(); ++meshIterator)
	{
		if(meshIterator->first == meshName)
		{
			return meshIterator->second->getResource();
		}
	}
	//else
	//create the mesh (will be deleted by the resource)
	Mesh* mMesh = new Mesh();
	//create new resource(will be deleted by the resourceManager)
	GameResource<Mesh*>* mResource = new GameResource<Mesh*>(meshPath, mMesh);


	//insert it to the map
	mMeshMap.insert(std::pair<std::string, GameResource<Mesh*>*>(meshName, mResource));

	//return it
	return mMesh;
}

//return the shader
Mesh* ResourceManager::getMesh(std::string meshName)
{
	//search if the resource already exists
	tMeshMap::iterator meshIterator;
	for(meshIterator = mMeshMap.begin(); meshIterator != mMeshMap.end(); ++meshIterator)
	{
		//if exuist return it
		if(meshIterator->first == meshName)
		{
			return meshIterator->second->getResource();
		}
	}

	//if not found return null
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

	//check if the time of modifications isnt the same
	tTextureMap::iterator textureIterator;
	for(textureIterator = mTextureMap.begin(); textureIterator != mTextureMap.end(); ++textureIterator)
	{
		if(textureIterator->second->lastModificationTime[0] != getModificationTime(textureIterator->second->getFilePath()))
		{
			//reload the file
			textureIterator->second->getResource()->loadTexture(textureIterator->second->getFilePath(), textureIterator->second->getResource()->isMipmap);
			textureIterator->second->lastModificationTime[0] = getModificationTime(textureIterator->second->getFilePath());
		}
	}
}

