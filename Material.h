#ifndef MATERIAL_H
#define MATERIAL_H

#include "Definitions.h"

#include <iostream>
#include <string>
#include <map>
#include <fstream>

class Material
{
public:
	Material(void);
	Material(std::string materialName, std::string shaderName);
	~Material(void);

	//read the materials propertyes from a file
	void readMaterialFromFile(std::string matFile);

	//Sends material information (uniforms) to the shader
	void applyMaterial();

	typedef struct
	{
		glm::vec3 mBaseColorV; //type 0
		Texture* mBaseColorT; //type 1
		bool baseColorTextured;
	} tBaseColor;

	typedef struct
	{
		real mRoughnessR;
		Texture* mRoughnessT;
		bool roughnessTextured;
	} tRoughness;
	
	typedef struct
	{
		glm::vec3 mEmissiveColorV;
		Texture* mEmissiveColorT;
		bool emissiveTextured;
	} tEmissiveColor;
	
	typedef struct
	{
		real mOpacityR;
		Texture* mOpacityT;
		bool opacityTextured;
	} tOpacity;

	typedef struct
	{
		Texture* mNormalT;
		bool normalTextured;
	} tNormal;

	//Material properties
	tBaseColor mBaseColorS;
	tRoughness mRoughNessS;
	tEmissiveColor mEmissiveColorS;
	tOpacity mOpacityS;
	real mMetallicR; //0 - 1
	real mSpecularR; //0 - 1 (Default 0.5) For non-Metallic elements SHOULD BE LEFT AS DEFAULT. Controls microshadows https://docs.unrealengine.com/latest/INT/Engine/Rendering/Materials/PhysicallyBased/index.html
	tNormal mNormalS;
	real mRefractionR; //any value (default 1 (air))
	glm::vec3 mAmbientColorV; //Can be a texture if passed a skyblock?? (maybe compute the mean color)

	Shader* getShader()
	{
		return mMaterialShader;
	}

private:
	Shader* mMaterialShader;
	std::string mMaterialName;

	void setParameterFromFile(std::string prop, std::ifstream& inputFile);

};

#endif