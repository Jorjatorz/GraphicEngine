#include "Mesh.h"

#include "Shader.h"
#include "Material.h"
#include "ResourceManager.h"
#include "SceneManager.h"

Mesh::Mesh(void)
{
	mCurrentSceneManager = NULL;
	numberOfVertices = numberOfFaces = 0;
	//the mesh buffers are not filled not loaded
	buffersLoaded = false;
	AABBmaxVector = AABBminVector = AABBsize = glm::vec3(0.0);

}

Mesh::Mesh(std::string meshPath, SceneManager* manager)
{
	mCurrentSceneManager = manager;
	numberOfVertices = numberOfFaces = 0;
	//the mesh buffers are not filled not loaded
	buffersLoaded = false;
	AABBmaxVector = AABBminVector = glm::vec3(0.0);

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

	glDeleteBuffers(1, &AABBvbo);
	glDeleteBuffers(1, &AABBibo);
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
			//We obtain the AABB
			calculateAABB(scene);

			aiMesh* mesh = scene->mMeshes[i]; //We take the mesh

			tMeshStruct newMesh; //aux mesh struct
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

			//Fill buffers
			//vertices
			if(mesh->HasPositions())
			{
				glBindBuffer(GL_ARRAY_BUFFER, newMesh.vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, newMesh.mVertexVector.size() * sizeof(GLfloat), &newMesh.mVertexVector[0], GL_STATIC_DRAW);
				glVertexAttribPointer(Shader::VERTEXPOSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL); //write vertices position to the shader
				glEnableVertexAttribArray(Shader::VERTEXPOSITION);
			}
			//normals
			if(mesh->HasNormals())
			{
				glBindBuffer(GL_ARRAY_BUFFER, newMesh.normalBuffer);
				glBufferData(GL_ARRAY_BUFFER, newMesh.mNormalsVector.size() * sizeof(GLfloat),	&newMesh.mNormalsVector[0], GL_STATIC_DRAW);
				glVertexAttribPointer(Shader::VERTEXNORMALS, 3, GL_FLOAT, GL_FALSE, 0, NULL); //write normals position to the shader
				glEnableVertexAttribArray(Shader::VERTEXNORMALS);
			}
			//UV coords
			if(mesh->HasTextureCoords(0))
			{
				glBindBuffer(GL_ARRAY_BUFFER, newMesh.texCoordsBuffer);
				glBufferData(GL_ARRAY_BUFFER, newMesh.mTexCoordsVector.size() * sizeof(GLfloat),	&newMesh.mTexCoordsVector[0], GL_STATIC_DRAW);
				glVertexAttribPointer(Shader::VERTEXTEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL); //write normals position to the shader
				glEnableVertexAttribArray(Shader::VERTEXTEXCOORD);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.mIndexVector.size() * sizeof(GLuint), &newMesh.mIndexVector[0], GL_STATIC_DRAW);


			//Materials
			newMesh.meshMaterial = mCurrentSceneManager->createMaterial(meshPath + "Material" + std::to_string(i), "NULL"); //i the index
			aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];

			//Load texture (if exists)
			aiString texturePath;
			if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
			{
				newMesh.meshMaterial->mBaseColorS.mBaseColorT = mCurrentSceneManager->createTexture(texturePath.data, false, GL_RGB, texturePath.data);
				newMesh.meshMaterial->mBaseColorS.baseColorTextured = true;
			}
			else
			{
				newMesh.meshMaterial->mBaseColorS.baseColorTextured = false;
			}

			//Diffuse Color
			aiColor4D diffuse;
			if (aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS)
			{
				newMesh.meshMaterial->mBaseColorS.mBaseColorV = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
			}

			//Ambien color
			aiColor4D ambient;
			if (aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &ambient) == AI_SUCCESS)
			{
				newMesh.meshMaterial->mAmbientColorV = glm::vec3(ambient.r, ambient.g, ambient.b);
			}

			//Emissive Color
			aiColor4D emissive;
			if (aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_EMISSIVE, &emissive) == AI_SUCCESS)
			{
				newMesh.meshMaterial->mEmissiveColorS.mEmissiveColorV = glm::vec3(emissive.r, emissive.g, emissive.b);
			}

			//Save the new mesh
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

		calculateAABB(scene);
		createAABB();
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

void Mesh::calculateAABB(const aiScene* mScene)
{
	for(int i = 0; i < mScene->mNumMeshes; ++i)
	{
		for(int j = 0; j < mScene->mMeshes[i]->mNumVertices; ++j)
		{
			if (AABBmaxVector.x < mScene->mMeshes[i]->mVertices[j].x)
				AABBmaxVector.x = mScene->mMeshes[i]->mVertices[j].x;
			if (AABBmaxVector.y < mScene->mMeshes[i]->mVertices[j].y)
				AABBmaxVector.y = mScene->mMeshes[i]->mVertices[j].y;
			if (AABBmaxVector.z < mScene->mMeshes[i]->mVertices[j].z)
				AABBmaxVector.z = mScene->mMeshes[i]->mVertices[j].z;

			if (AABBminVector.x > mScene->mMeshes[i]->mVertices[j].x)
				AABBminVector.x = mScene->mMeshes[i]->mVertices[j].x;
			if (AABBminVector.y > mScene->mMeshes[i]->mVertices[j].y)
				AABBminVector.y = mScene->mMeshes[i]->mVertices[j].y;
			if (AABBminVector.z > mScene->mMeshes[i]->mVertices[j].z)
				AABBminVector.z = mScene->mMeshes[i]->mVertices[j].z;
		}
	}
}


void Mesh::createAABB()
{
	// Cube 1x1x1, centered on origin
	GLfloat vertices[] = {
		-0.5, -0.5, -0.5, 1.0,
		0.5, -0.5, -0.5, 1.0,
		0.5, 0.5, -0.5, 1.0,
		-0.5, 0.5, -0.5, 1.0,
		-0.5, -0.5, 0.5, 1.0,
		0.5, -0.5, 0.5, 1.0,
		0.5, 0.5, 0.5, 1.0,
		-0.5, 0.5, 0.5, 1.0,
	};


	glGenBuffers(1, &AABBvbo);
	glBindBuffer(GL_ARRAY_BUFFER, AABBvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort elements[] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	glGenBuffers(1, &AABBibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AABBibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	AABBsize = AABBmaxVector - AABBminVector;
	glm::vec3 AABBcenter = glm::vec3((AABBminVector.x + AABBmaxVector.x) / 2.0, (AABBminVector.y + AABBmaxVector.y) / 2.0, (AABBminVector.z + AABBmaxVector.z) / 2.0);
	AABBmatrix = glm::translate(glm::mat4(1), AABBcenter) * glm::scale(glm::mat4(1), AABBsize);

}

void Mesh::renderAABB(glm::mat4 modelMatrix, glm::mat4 projectionViewM)
{
	/* Apply object's transformation matrix */
	glm::mat4 finalAABBMat = modelMatrix * AABBmatrix;

	Shader* mShader = mCurrentSceneManager->getCurrentShader();

	mShader->UniformMatrix("MVP", projectionViewM * finalAABBMat);

	glBindBuffer(GL_ARRAY_BUFFER, AABBvbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,  // attribute
		4,                  // number of elements per vertex, here (x,y,z,w)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AABBibo);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0); //Front face
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort))); //Back face
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort))); //Joint lines
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}