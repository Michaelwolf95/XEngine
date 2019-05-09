#include "SkyboxRenderer.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;
#include "stb/stb_image.h"
#include <vector>
#include "imgui_inspector_extensions.h"

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

// Register to be created and serialized.
REGISTER_COMPONENT(XEngine::SkyboxRenderer, "SkyboxRenderer")
namespace XEngine {
	SkyboxRenderer::SkyboxRenderer() 
	{
		for (size_t i = 0; i < 6; i++)
		{
			cubemapFaces.push_back("");
		}
		//std::string faces[6];

		//cubemapFaces = std::vector<std::string>(faces, 6);
	}
	SkyboxRenderer::~SkyboxRenderer() {
		OnDisable();
	}

	// Start is called on the objects first update.
	void SkyboxRenderer::Start()
	{

	}

	// Update is called once per frame.
	void SkyboxRenderer::Update()
	{

	}

	// Draw the inspector for your custom component.
	void SkyboxRenderer::DrawInspector()
	{
		std::vector<std::string> textureExtensions;
		textureExtensions.push_back(".png");
		textureExtensions.push_back(".PNG");
		textureExtensions.push_back(".jpg");
		textureExtensions.push_back(".JPEG");
		textureExtensions.push_back(".tga");
		GUI::FileReference(cubemapFaces[0], textureExtensions, "Right");
		GUI::FileReference(cubemapFaces[1], textureExtensions, "Left");
		GUI::FileReference(cubemapFaces[2], textureExtensions, "Bottom");
		GUI::FileReference(cubemapFaces[3], textureExtensions, "Top");
		GUI::FileReference(cubemapFaces[4], textureExtensions, "Front");
		GUI::FileReference(cubemapFaces[5], textureExtensions, "Back");
		if (ImGui::Button("Update"))
		{
			cubemapTexture = loadCubemap(cubemapFaces);
		}
	}


	void SkyboxRenderer::OnEnable()
	{
		this->render_enabled = true;
		RenderManager::getInstance().AddRenderable((RenderableObject*)this);
	}

	void SkyboxRenderer::OnDisable()
	{
		this->render_enabled = false;
		RenderManager::getInstance().RemoveRenderable((RenderableObject*)this);
	}

	void SkyboxRenderer::Setup()
	{
		if (isSetup) return;
		skyboxShader = AssetManager::getInstance().shaderLib.GetAsset("../Shaders/skybox.vs", "../Shaders/skybox.fs");
		//skyboxShader = new Shader("../Shaders/skybox.vs", "../Shaders/skybox.fs");

		// skybox VAO

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// load textures
		// -------------
		/*vector<std::string> faces
		{
			ASSET_FILE_PATH + "Textures/skybox/right.jpg",
			ASSET_FILE_PATH + "Textures/skybox/left.jpg",
			ASSET_FILE_PATH + "Textures/skybox/top.jpg",
			ASSET_FILE_PATH + "Textures/skybox/bottom.jpg",
			ASSET_FILE_PATH + "Textures/skybox/front.jpg",
			ASSET_FILE_PATH + "Textures/skybox/back.jpg"
		};*/
		//cubemapTexture = loadCubemap(std::vector<std::string>(cubemapFaces));
		cubemapTexture = loadCubemap(cubemapFaces);
		// shader configuration
		// --------------------

		skyboxShader->use();
		skyboxShader->setInt("skybox", 0);

		isSetup = true;
	}

	void SkyboxRenderer::Draw()
	{
		if (!isSetup) return;
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader->use();
		glm::mat4 view = RenderManager::getInstance().getView();
		glm::mat4 projection = RenderManager::getInstance().getProjection();
		view = glm::mat4(-glm::mat3(view)); // remove translation from the view matrix
		skyboxShader->setMat4("view", view);
		skyboxShader->setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
	}

	// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
	unsigned int SkyboxRenderer::loadCubemap(vector<std::string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}
}