#include "Mesh.h"
#include "Shader.h"

Mesh::Mesh(void)
{
	//the mesh buffers are not filled not loaded
	buffersLoaded = false;
	//generate buffers
	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
}


Mesh::~Mesh(void)
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vertexArrayObject);
}


void Mesh::createCube()
{
	//if the buffers werent filled, fill them
	if(!buffersLoaded)
	{
		//vertices
		const GLfloat vertex_positions[] =
		{
			-0.25f, -0.25f, -0.25f,
			-0.25f, 0.25f, -0.25f,
			0.25f, -0.25f, -0.25f,
			0.25f, 0.25f, -0.25f,
			0.25f, -0.25f, 0.25f,
			0.25f, 0.25f, 0.25f,
			-0.25f, -0.25f, 0.25f,
			-0.25f, 0.25f, 0.25f,
		};
		//indexes
		const GLushort vertex_indices[] =
		{
			0, 1, 2,
			2, 1, 3,
			2, 3, 4,
			4, 3, 5,
			4, 5, 6,
			6, 5, 7,
			6, 7, 0,
			0, 7, 1,
			6, 0, 2,
			2, 4, 6,
			7, 5, 3,
			7, 3, 1
		};

		numberOfVertices = 36;


		glBindVertexArray(vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions),	vertex_positions, GL_STATIC_DRAW);
		glVertexAttribPointer(Shader::vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL); //write vertices position to the shader
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
		glBindVertexArray(0);

		buffersLoaded = true;
	}
}

void Mesh::bindMeshArray()
{
	glBindVertexArray(vertexArrayObject);
}

void Mesh::unbindMeshArray()
{
	glBindVertexArray(0);
}

void Mesh::loadMesh(std::string meshName)
{
	if(!buffersLoaded)
	{
		Assimp::Importer mImporter;
		const aiScene *scene = mImporter.ReadFile(meshName.c_str(), aiProcessPreset_TargetRealtime_Quality);

		if(!scene)
		{
			std::cout << "Error importing " << meshName << " " << mImporter.GetErrorString() << std::endl;
		}

		aiMesh* mesh = scene->mMeshes[0]; //We only take the first mesh

		numberOfVertices = mesh->mNumVertices; //set the number of vertices

		for(int i = 0; i < mesh->mNumVertices; ++i)
		{
			//create vertex array
			const aiVector3D* vertex = &mesh->mVertices[i]; //copy the vertices
			const aiVector3D* normal = &mesh->mNormals[i]; //copy the vertices

			mVertexVector.push_back(vertex->x);
			mVertexVector.push_back(vertex->y);
			mVertexVector.push_back(vertex->z);

			mNormalsVector.push_back(normal->x);
			mNormalsVector.push_back(normal->y);
			mNormalsVector.push_back(normal->z);

		}

		for(int j = 0; j < mesh->mNumFaces; ++j)
		{
			//create index array
			const aiFace* face = &mesh->mFaces[j];

			assert(face->mNumIndices == 3);

			mIndexVector.push_back(face->mIndices[0]);
			mIndexVector.push_back(face->mIndices[1]);
			mIndexVector.push_back(face->mIndices[2]);
		}



		glBindVertexArray(vertexArrayObject);
		//vertices
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, mVertexVector.size() * sizeof(GLfloat),	&mVertexVector[0], GL_STATIC_DRAW);
		glVertexAttribPointer(Shader::vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL); //write vertices position to the shader
		//normals
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, mNormalsVector.size() * sizeof(GLfloat),	&mNormalsVector[0], GL_STATIC_DRAW);
		glVertexAttribPointer(Shader::vertexNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL); //write vertices position to the shader

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexVector.size() * sizeof(GLushort), &mIndexVector[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindVertexArray(0);

		buffersLoaded = true;

		aiMatrix4x4 m = scene->mRootNode->mTransformation;
		// OpenGL matrices are column major
		m.Transpose();

		// save model matrix and apply node transformation
		for(unsigned int i = 0; i < 4; i++)
		{
			for(unsigned int j = 0; j < 4; j++)
			{
				meshMatrix[i][j] = m[i][j];
			}
	}

	}
}