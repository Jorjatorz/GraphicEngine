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

	//predifined mesh
	//square
	//triangle
	//cube
	void createCube();

	//load mesh
	void loadMesh(std::string meshPath); //load the mesh from a file

	//bind the mesh vertexArray
	void bindMeshArray();
	void unbindMeshArray();

	//number of vertices and indices the mesh have
	GLuint numberOfVertices;
	GLuint numberOfIndices;

	glm::mat4 meshMatrix;


private:
	//uv, etc.
	std::vector<GLfloat> mVertexVector;
	std::vector<GLfloat> mNormalsVector;
	std::vector<GLushort> mIndexVector;



	//controls if the vertex and index buffers where already created (thus (if wanted to be loaded) it was succesfully loaded from file)
	bool buffersLoaded;

	//openGL buffers
	GLuint vertexBuffer, normalBuffer, uvBuffer, indexBuffer, vertexArrayObject;
};

#endif