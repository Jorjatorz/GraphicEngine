#include "Mesh.h"
#include "Shader.h"

Mesh::Mesh(void)
{
	numberOfVertices = numberOfFaces = 0;
	//the mesh buffers are not filled not loaded
	buffersLoaded = false;
	//generate buffers
}

Mesh::Mesh(std::string meshPath)
{
	numberOfVertices = numberOfFaces = 0;
	//the mesh buffers are not filled not loaded
	buffersLoaded = false;

	loadMesh(meshPath); //load the mesh
}

Mesh::~Mesh(void)
{
	for(int i = 0; i < mMeshComponentsVector.size(); ++i)
	{
		glDeleteBuffers(1, &mMeshComponentsVector[i].vertexBuffer);
		glDeleteBuffers(1, &mMeshComponentsVector[i].normalBuffer);
		glDeleteBuffers(1, &mMeshComponentsVector[i].texCoordsBuffer);
		glDeleteBuffers(1, &mMeshComponentsVector[i].indexBuffer);
		glDeleteVertexArrays(1, &mMeshComponentsVector[i].vertexArrayObject);
	}
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

		tMeshStruct newMesh;

		genBuffers(newMesh); //generete buffers

		glBindVertexArray(newMesh.vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh.vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions),	vertex_positions, GL_STATIC_DRAW);
		glVertexAttribPointer(Shader::vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL); //write vertices position to the shader
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
		glBindVertexArray(0);

		mMeshComponentsVector.push_back(newMesh); //store the components

		buffersLoaded = true;
	}
}

void Mesh::bindMeshArray(const tMeshStruct &mComp)
{
	glBindVertexArray(mComp.vertexArrayObject);
}

void Mesh::unbindMeshArray()
{
	glBindVertexArray(0);
}

void Mesh::loadMesh(std::string meshPath)
{
	if(!buffersLoaded)
	{
		std::cout << "Loading mesh: " << meshPath << std::endl;

		Assimp::Importer mImporter;
		const aiScene *scene = mImporter.ReadFile(meshPath.c_str(), aiProcessPreset_TargetRealtime_Fast);

		std::cout << "Load succesful" << std::endl;
		std::cout << "Number of meshes: " << scene->mNumMeshes << std::endl;

		if(!scene)
		{
			std::cout << "Error importing " << meshPath << " " << mImporter.GetErrorString() << std::endl;
		}

		for(unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i]; //We take the mesh

			tMeshStruct newMesh;
			genBuffers(newMesh); //generate buffers

			numberOfVertices += mesh->mNumVertices; //set the number of vertices
			numberOfFaces += mesh->mNumFaces; //each face has 3 indices

			for(unsigned int j = 0; j < mesh->mNumVertices; ++j)
			{
				//create vertex array
				const aiVector3D* vertex = &mesh->mVertices[j]; //copy the vertices
				const aiVector3D* normal = &mesh->mNormals[j]; //copy the vertices
				const aiVector3D* texCoord = &mesh->mTextureCoords[0][j];

				if(mesh->HasPositions())
				{
					newMesh.mVertexVector.push_back(vertex->x);
					newMesh.mVertexVector.push_back(vertex->y);
					newMesh.mVertexVector.push_back(vertex->z);
				}
			
				if(mesh->HasNormals())
				{
					newMesh.mNormalsVector.push_back(normal->x);
					newMesh.mNormalsVector.push_back(normal->y);
					newMesh.mNormalsVector.push_back(normal->z);
				}

				if(mesh->HasTextureCoords(0))
				{
					newMesh.mTexCoordsVector.push_back(texCoord->x);
					newMesh.mTexCoordsVector.push_back(texCoord->y);
				}

			}

			for(int j = 0; j < mesh->mNumFaces; ++j)
			{
				//create index array
				const aiFace* face = &mesh->mFaces[j];

				assert(face->mNumIndices == 3);

				newMesh.mIndexVector.push_back(face->mIndices[0]);
				newMesh.mIndexVector.push_back(face->mIndices[1]);
				newMesh.mIndexVector.push_back(face->mIndices[2]);
			}

			glBindVertexArray(newMesh.vertexArrayObject);
			//vertices
			if(mesh->HasPositions())
			{
				glBindBuffer(GL_ARRAY_BUFFER, newMesh.vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, newMesh.mVertexVector.size() * sizeof(GLfloat), &newMesh.mVertexVector[0], GL_STATIC_DRAW);
				glVertexAttribPointer(Shader::vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL); //write vertices position to the shader
				glEnableVertexAttribArray(Shader::vertexPosition);
			}
			//normals
			if(mesh->HasNormals())
			{
				glBindBuffer(GL_ARRAY_BUFFER, newMesh.normalBuffer);
				glBufferData(GL_ARRAY_BUFFER, newMesh.mNormalsVector.size() * sizeof(GLfloat),	&newMesh.mNormalsVector[0], GL_STATIC_DRAW);
				glVertexAttribPointer(Shader::vertexNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL); //write normals position to the shader
				glEnableVertexAttribArray(Shader::vertexNormal);
			}
			//UV coords
			if(mesh->HasTextureCoords(0))
			{
				glBindBuffer(GL_ARRAY_BUFFER, newMesh.texCoordsBuffer);
				glBufferData(GL_ARRAY_BUFFER, newMesh.mTexCoordsVector.size() * sizeof(GLfloat),	&newMesh.mTexCoordsVector[0], GL_STATIC_DRAW);
				glVertexAttribPointer(Shader::vertexTexCoord, 2, GL_FLOAT, GL_FALSE, 0, NULL); //write normals position to the shader
				glEnableVertexAttribArray(Shader::vertexTexCoord);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.mIndexVector.size() * sizeof(GLuint), &newMesh.mIndexVector[0], GL_STATIC_DRAW);

			mMeshComponentsVector.push_back(newMesh);
		}

		// unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


		buffersLoaded = true;

		aiMatrix4x4 m = scene->mRootNode->mTransformation;
		// OpenGL matrices are column major
		m.Transpose();

		// save model matrix
		for(unsigned int i = 0; i < 4; i++)
		{
			for(unsigned int j = 0; j < 4; j++)
			{
				meshMatrix[i][j] = m[i][j];
			}
		}

	}
}

void Mesh::genBuffers(tMeshStruct &meshToGen)
{
	glGenVertexArrays(1, &meshToGen.vertexArrayObject);
	glGenBuffers(1, &meshToGen.vertexBuffer);
	glGenBuffers(1, &meshToGen.normalBuffer);
	glGenBuffers(1, &meshToGen.texCoordsBuffer);
	glGenBuffers(1, &meshToGen.indexBuffer);
}