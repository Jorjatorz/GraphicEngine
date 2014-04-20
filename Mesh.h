#ifndef MESH_H
#define MESH_H

#include <string>

#include "Definitions.h"

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	//predifined mesh
	//square
	//triangle
	//cube
	void createCube();

	//load mesh
	void loadMesh(std::string meshName); //load the mesh from a file

	//bind the mesh vertexArray
	void bindMeshArray();
	void unbindMeshArray();

	//number of vertices the mesh have
	GLint numberOfVertices;
private:
	//uv, etc.
<<<<<<< HEAD
=======
	std::vector<GLfloat> mVertexVector;
	std::vector<GLfloat> mNormalsVector;
	std::vector<GLushort> mIndexVector;


>>>>>>> parent of f606805... Monomeshes now loading Ok. Trying to implement LookAt to SceneNodes

	//controls if the vertex and index buffers where already created (thus (if wanted to be loaded) it was succesfully loaded from file)
	bool buffersLoaded;

	//openGL buffers
	GLuint vertexBuffer, indexBuffer, vertexArrayObject;
};

#endif