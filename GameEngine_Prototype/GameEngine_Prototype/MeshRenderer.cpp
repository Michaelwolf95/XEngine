#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GameObject.h"
#include "RenderManager.h"
#include "RenderableObject.h"
#include "Component.h"
#include "Mesh.h"
#include "Shader.h"
#include "ApplicationManager.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "RenderManager.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "MeshRenderer.h"
#include "AssetManager.h"

REGISTER_COMPONENT(MeshRenderer, "MeshRenderer")

MeshRenderer::MeshRenderer()
{
}
// Constructor
MeshRenderer::MeshRenderer(string const &path, Material* m , bool gamma): 
	gammaCorrection(gamma), 
	RenderableObject(m)
{
	// filepath for .obj file.
	this->pathToObjModel = ASSET_FILE_PATH + std::string(path);

	Setup();
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Setup()
{
	if (isSetup)
	{
		return;
	}
	//std::cout << "Setting up SimpleModelComponent." << std::endl;
	render_enabled = true;
	RenderManager::getInstance().AddRenderable((RenderableObject*)this);

	if (material == nullptr)
	{
		material = RenderManager::defaultMaterial;
	}
	else
	{

	}

	isSetup = true;

	// read file using ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pathToObjModel, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = pathToObjModel.substr(0, pathToObjModel.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);


}

void MeshRenderer::Draw()
{

	
	//if (gameObject == nullptr) return;

	
	// This was the error
	glm::mat4 view = RenderManager::getInstance().getView();
	glm::mat4 projection = RenderManager::getInstance().getProjection();
	material->shader->setMat4("view", view);
	material->shader->setMat4("projection", projection);

	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	material->shader->setMat4("model", this->gameObject->transform->getMatrix4x4());
	
	
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		// texture variables
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		// binding textures
		for (unsigned int j = 0; j < meshes[i].textures.size(); j++)
		{
			// get texture before binding
			glActiveTexture(GL_TEXTURE0 + j);

			string number;
			string name = meshes[i].textures[j].type;

			// transfer unsigned to stream
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			// set texture unit
			glUniform1i(glGetUniformLocation(material->shader->ID, (name + number).c_str()), j);

			// bind texture
			glBindTexture(GL_TEXTURE_2D, meshes[i].textures[j].id);
		}

		// draw mesh
		glBindVertexArray(meshes[i].VAO);
		glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// default once configured
		glActiveTexture(GL_TEXTURE0);

	}
}

void MeshRenderer::Start()
{
}

void MeshRenderer::Update()
{
}


// process each node and its children node recursively
void MeshRenderer::processNode(aiNode *node, const aiScene *scene)
{

	// process each mesh at current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// recursively call the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

Mesh MeshRenderer::processMesh(aiMesh *mesh, const aiScene *scene)
{
	// data of the meshes
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// placeholder vector because assimp's vector class does not directly translate to glm's vector class
		glm::vec3 vector;

		// position of vector
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// normal of vector
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		

		// texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;

			// vertex can hold up to 8 texture coordinates
			// under assumption, we won't use models where the vertex can have multiple texture coordinates
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);


		// tangent of vector
		if (mesh->HasTangentsAndBitangents())
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// bitangent of vector
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		

		vertices.push_back(vertex);
	}
		
	// walk through mesh' face(mesh's trangles) and get corresponding vertex indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		// get all indices of face and store in indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process materials
	aiMaterial* aMaterial = scene->mMaterials[mesh->mMaterialIndex];

	// assume sampler names in shaders 
	// such as 'texture_diffuseN' where N is ranging from 1 to MAX_SAMPLER_NUMBER

	// 1.diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(aMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
	// 2.specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(aMaterial, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3.normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(aMaterial, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// 4.height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(aMaterial, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return mesh object from extracted mesh data
	return Mesh(vertices, indices, textures);// :RenderableObject();
}

// checl material textures of a given type and loads texture if not loaded yet
vector<Texture> MeshRenderer::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{

	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		
		mat->GetTexture(type, i, &str);

		// check if texture was loaded, if so skip
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // texture with same filepath already loaded, continue to next
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);

			// store as texture loaded for entire model
			// so we dont load duplicate textures
			textures_loaded.push_back(texture);
		}
		return textures;
	}

}

unsigned int MeshRenderer::TextureFromFile(const char * path, const string &directory, bool gamma)
{

	string filename = string(path);
	filename = directory + '/' + filename;


	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;

		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
void MeshRenderer::OnDrawGizmos()
{
	//gameObject->transform->DrawGizmo();
}

void MeshRenderer::DrawInspector()
{
	char modelPath[48];
	strcpy(modelPath, this->pathToObjModel.c_str());// , );
	ImGui::InputText("ModelPath", modelPath, 48);
	if (modelPath != this->pathToObjModel.c_str())
	{
		this->pathToObjModel = modelPath;
	}
	this->material->DrawInspector();
}