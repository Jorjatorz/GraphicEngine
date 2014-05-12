#include "Material.h"

#include "ResourceManager.h"
#include "SceneManager.h"
#include "Shader.h"
#include "Texture.h"

Material::Material(void)
{
	mMaterialName = "NULL";
	mMaterialShader = ResourceManager::getSingletonPtr()->loadShader("basic", "basic"); //Default shader

	//Init mat properties
	mBaseColorS.baseColorTextured = false;
	mBaseColorS.mBaseColorT = NULL;
	mBaseColorS.mBaseColorV = glm::vec3(0.17, 0.17, 0.17);
	mRoughNessS.roughnessTextured = false;
	mRoughNessS.mRoughnessT = NULL;
	mRoughNessS.mRoughnessR = 0.0;
	mEmissiveColorS.emissiveTextured = false;
	mEmissiveColorS.mEmissiveColorT = NULL;
	mEmissiveColorS.mEmissiveColorV = glm::vec3(1.0);
	mOpacityS.opacityTextured = false;
	mOpacityS.mOpacityT = NULL;
	mOpacityS.mOpacityR = 0.0;
	mMetallicR = 1.0; //FUll metal
	mSpecularR = 0.5;
	mNormalS.normalTextured = false;
	mNormalS.mNormalT = NULL;
	mRefractionR = 1;
	mAmbientColorV = glm::vec3(1.0);
}

Material::Material(std::string materialName, std::string shaderName)
{
	mMaterialName = materialName;
	mMaterialShader = ResourceManager::getSingletonPtr()->getShader(shaderName);

	//Init mat properties
	mBaseColorS.baseColorTextured = false;
	mBaseColorS.mBaseColorT = NULL;
	mBaseColorS.mBaseColorV = glm::vec3(0.17, 0.17, 0.17);
	mRoughNessS.roughnessTextured = false;
	mRoughNessS.mRoughnessT = NULL;
	mRoughNessS.mRoughnessR = 0.0;
	mEmissiveColorS.emissiveTextured = false;
	mEmissiveColorS.mEmissiveColorT = NULL;
	mEmissiveColorS.mEmissiveColorV = glm::vec3(1.0);
	mOpacityS.opacityTextured = false;
	mOpacityS.mOpacityT = NULL;
	mOpacityS.mOpacityR = 0.0;
	mMetallicR = 1.0; //FUll metal
	mSpecularR = 0.5;
	mNormalS.normalTextured = false;
	mNormalS.mNormalT = NULL;
	mRefractionR = 1;
	mAmbientColorV = glm::vec3(1.0);
}



Material::~Material(void)
{
}

void Material::applyMaterial()
{
	//send uniforms
	if(mBaseColorS.baseColorTextured)
	{
		mMaterialShader->UniformTexture("baseTexture", mBaseColorS.mBaseColorT->mTextureID);
	}
	else
	{
		mMaterialShader->Uniform("baseColorVector", mBaseColorS.mBaseColorV);
	}

	if(mRoughNessS.roughnessTextured)
	{
		mMaterialShader->UniformTexture("roughnessTexture", mRoughNessS.mRoughnessT->mTextureID);
	}
	else
	{
		mMaterialShader->Uniform("roughnessValue", mRoughNessS.mRoughnessR);
	}

	if(mEmissiveColorS.emissiveTextured)
	{
		mMaterialShader->UniformTexture("emissiveTexture", mEmissiveColorS.mEmissiveColorT->mTextureID);
	}
	else
	{
		mMaterialShader->Uniform("emissiveVector", mEmissiveColorS.mEmissiveColorV);
	}

	if(mOpacityS.opacityTextured)
	{
		mMaterialShader->UniformTexture("opacityTexture", mOpacityS.mOpacityT->mTextureID);
	}
	else
	{
		mMaterialShader->Uniform("opacityValue", mOpacityS.mOpacityR);
	}
	
	mMaterialShader->Uniform("metallicValue", mMetallicR);

	mMaterialShader->Uniform("specularValue", mSpecularR);

	if(mNormalS.normalTextured)
	{
		mMaterialShader->UniformTexture("normalsTexture", mNormalS.mNormalT->mTextureID);
	}

	mMaterialShader->Uniform("refractionValue", mRefractionR);
	
	mMaterialShader->Uniform("ambientColorVector", mAmbientColorV);
}

void Material::readMaterialFromFile(std::string matFile)
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
}

void Material::setParameterFromFile(std::string prop, std::ifstream& inputFile)
{
	char equalSign;

	if(prop == "Shader")
	{
		inputFile >> equalSign;
		std::string shaderPath;
		inputFile >> shaderPath;
		mMaterialShader = ResourceManager::getSingletonPtr()->loadShader(shaderPath, shaderPath);
	}
	else if(prop == "baseColorT")
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

void Material::setBaseColorV(glm::vec3 newBaseColor)
{
	mBaseColorS.mBaseColorV = newBaseColor;
}

void Material::setBaseColorRGB(glm::vec3 newBaseColor)
{
	mBaseColorS.mBaseColorV = newBaseColor / 255.0f;
}