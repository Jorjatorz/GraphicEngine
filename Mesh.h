#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <string>
#include <vector>

#include "Definitions.h"

class Mesh
{
public:
	Mesh(void);
	Mesh(std::string meshPath);
	~Mesh(void);

private:
	//uv, etc.
	typedef struct
	{
		std::vector<GLfloat> mVertexVector;
		std::vector<GLfloat> mNormalsVector;
		std::vector<GLfloat> mTexCoordsVector;
		std::vector<GLuint> mIndexVector;

		//openGL buffers
		GLuint vertexBuffer, normalBuffer, texCoordsBuffer, indexBuffer, vertexArrayObject;
	} tMeshStruct;

	Material* meshMaterial;

public:
	//predifined mesh
	//square
	//triangle
	//cube
	void createCube();

	//load mesh
	void loadMesh(std::string meshPath); //load the mesh from a file

	//Material fro mmesh
	Material* getMaterial()
	{
		return meshMaterial;
	}

	//bind the mesh vertexArray
	void bindMeshArray(const tMeshStruct &mComp);
	void unbindMeshArray();

	//number of vertices and indices the mesh have
	GLuint numberOfVertices;
	GLuint numberOfFaces;

	glm::mat4 meshMatrix;

	std::vector<tMeshStruct> mMeshComponentsVector; //contains all the meshes of the total mesh

private:

	void genBuffers(tMeshStruct &meshToGen); //Generates all the buffers of that mesh

	void calculateAABB(const aiScene* mScene); //Get the AABB vertices and generate buffers
	glm::vec3 AABBmaxVector, AABBminVector; //Holds AABB vertex positions


	//controls if the vertex and index buffers where already created (thus (if wanted to be loaded) it was succesfully loaded from file)
	bool buffersLoaded;
};

#endif