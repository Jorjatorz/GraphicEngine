#include "Material.h"

#include "ResourceManager.h"
#include "SceneManager.h"

Material::Material(void)
{
	mMaterialName = "NULL";
	mMaterialShader = NULL;
}

Material::Material(std::string materialName, std::string shaderName)
{
	mMaterialName = materialName;
	mMaterialShader = ResourceManager::getSingletonPtr()->getShader(shaderName);
}



Material::~Material(void)
{
}

void Material::applyMaterial(SceneManager* currentSceneManager)
{
	//send uniforms
}

void Material::readMaterial(std::string matFile)
{
	std::string nextProp = "null";

	std::ifstream inputFile(matFile.c_str());

	if(!inputFile.is_open())
	{
		std::cout << "Error opening material " << matFile << std::endl;
	}
	else
	{
		//while the string is not  equal the sentinel
		inputFile >> nextProp;
		while(nextProp != "<end>")
		{
			setParameterFromFile(nextProp, inputFile);
			inputFile >> nextProp;
		}
		
		inputFile.close();
	}
	/*
	material name
	base texture?
	base color/texture
	rougness texture?
	rougnes diff/tex
	metallic tex/diff
	specular
	normalT
	refraction
	ambient
	emissive
	opacity
	*/
}

void Material::setParameterFromFile(std::string prop, std::ifstream& inputFile)
{
	char equalSign;
	if(prop == "baseColorT")
	{
		inputFile >> equalSign; //get the =
		std::string texturePath;
		inputFile >> texturePath;
		mBaseColorS.mBaseColorT = ResourceManager::getSingletonPtr()->loadTexture(texturePath, false, texturePath);
		mBaseColorS.baseColorTextured = true;
	}
	else if(prop == "baseColorV")
	{
		inputFile >> equalSign; //get the =
		char comma;
		inputFile >> mBaseColorS.mBaseColorV.x >> comma >> mBaseColorS.mBaseColorV.y >> comma >> mBaseColorS.mBaseColorV.z;
		mBaseColorS.baseColorTextured = false;
	}
	else if(prop == "roughnessT")
	{
		inputFile >> equalSign; //get the =
		std::string texturePath;
		inputFile >> texturePath;
		mRoughNessS.mRoughnessT = ResourceManager::getSingletonPtr()->loadTexture(texturePath, false, texturePath);
		mRoughNessS.roughnessTextured = true;
	}
	else if(prop == "roughnessR")
	{
		inputFile >> equalSign; //get the =
		inputFile >> mRoughNessS.mRoughnessR;
		mRoughNessS.roughnessTextured = false;
	}
	else if(prop == "emissiveT")
	{
		inputFile >> equalSign; //get the =
		std::string texturePath;
		inputFile >> texturePath;
		mEmissiveColorS.mEmissiveColorT = ResourceManager::getSingletonPtr()->loadTexture(texturePath, false, texturePath);
		mEmissiveColorS.emissiveTextured = true;
	}
	else if(prop == "emissiveV")
	{
		inputFile >> equalSign; //get the =
		char comma;
		inputFile >> mEmissiveColorS.mEmissiveColorV.x >> comma >> mEmissiveColorS.mEmissiveColorV.y >> comma >> mEmissiveColorS.mEmissiveColorV.z;
		mEmissiveColorS.emissiveTextured = false;
	}
	else if(prop == "opacityT")
	{
		inputFile >> equalSign; //get the =
		std::string texturePath;
		inputFile >> texturePath;
		mOpacityS.mOpacityT = ResourceManager::getSingletonPtr()->loadTexture(texturePath, false, texturePath);
		mOpacityS.opacityTextured = true;
	}
	else if(prop == "opacityR")
	{
		inputFile >> equalSign; //get the =
		inputFile >> mOpacityS.mOpacityR;
		mOpacityS.opacityTextured = false;
	}
	else if(prop == "metallicR")
	{
		inputFile >> equalSign; //get the =
		inputFile >> mMetallicR;
	}
	else if(prop == "specularR")
	{
		inputFile >> equalSign; //get the =
		inputFile >> mSpecularR;
	}
	else if(prop == "normalT")
	{
		inputFile >> equalSign; //get the =
		std::string texturePath;
		inputFile >> texturePath;
		mNormalS.mNormalT = ResourceManager::getSingletonPtr()->loadTexture(texturePath, false, texturePath);
		mNormalS.normalTextured = true;
	}
	else if(prop == "refractionR")
	{
		inputFile >> equalSign; //get the =
		inputFile >> mRefractionR;
	}
	else if(prop == "ambientColorV")
	{
		inputFile >> equalSign; //get the =
		char comma;
		inputFile >> mAmbientColorV.x >> comma >> mAmbientColorV.y >> comma >> mAmbientColorV.z;
	}
}