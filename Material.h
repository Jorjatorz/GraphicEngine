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
	void readMaterial(std::string matFile);

	//Sends material information (uniforms) to the shader
	void applyMaterial(SceneManager* currentSceneManager);

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
	real mSpecularR; //0 - 1 (Default 0.5) For non-Metallic elements
	tNormal mNormalS;
	real mRefractionR; //any value (default 1 (air))
	glm::vec3 mAmbientColorV; //Can be a texture if passed a skyblock?? (maybe compute the mean color)

	std::string mMaterialName;
	Shader* mMaterialShader;

private:
	void setParameterFromFile(std::string prop, std::ifstream& inputFile);

};

#endif