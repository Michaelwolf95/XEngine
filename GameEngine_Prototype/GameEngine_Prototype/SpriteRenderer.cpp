#include "SpriteRenderer.h"
#include "AssetManager.h"
#include "RenderManager.h"

REGISTER_COMPONENT(XEngine::SpriteRenderer, "SpriteRenderer")

namespace XEngine {

	SpriteRenderer::SpriteRenderer()
	{
		//Setup();
	}

	SpriteRenderer::~SpriteRenderer()
	{
		OnDisable();
	}
	void SpriteRenderer::Start()
	{
		
	}
	void SpriteRenderer::Update()
	{
	}

	void SpriteRenderer::OnEnable()
	{
		this->render_enabled = true;
		RenderManager::getInstance().AddRenderable((RenderableObject*)this);
	}

	void SpriteRenderer::OnDisable()
	{
		this->render_enabled = false;
		RenderManager::getInstance().RemoveRenderable((RenderableObject*)this);
	}

	// THIS IS THE PROBLEM ===========================================================
	// Loading Texture with textureLib??
	void SpriteRenderer::Setup()
	{
		if (isSetup)
		{
			return;
		}
		isSetup = true;
		std::cout << "Setting up SpriteShader.\n";
		shader = AssetManager::getInstance().shaderLib.GetAsset("../Shaders/sprite.vs", "../Shaders/sprite.fs");
		*texture = AssetManager::getInstance().textureLib.GetAsset(textureFilePath);
		//AssetManager::LoadTextureAsset(textureFilePath.c_str(), &textureID);
		// Configure VAO/VBO
		GLuint VBO;
		GLfloat vertices[] = {
			// Pos      // Tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &this->quadVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(this->quadVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void SpriteRenderer::Draw()
	{
		if (!isSetup) return;
		// Prepare transformations
		this->shader->use();

		glm::mat4 model;
		model = glm::translate(model, this->gameObject->transform->getPosition());  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
		glm::vec2 size(1, 1);
		float rotate = 45.0f;
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
		model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

		model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

		this->shader->setMat4("model", model);



		this->shader->setMat4("model", this->gameObject->transform->getMatrix4x4());

		// Render textured quad
		this->shader->setVec3("spriteColor", glm::vec3(1,0,0));

		glActiveTexture(GL_TEXTURE0);

		if (texture->id > 0)
		{
			// bind Texture
			glBindTexture(GL_TEXTURE_2D, texture->id);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBindVertexArray(this->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}


	void SpriteRenderer::DrawInspector()
	{
		
		ImGui::InputText("ModelPath", &this->textureFilePath[0], 128);

		if (ImGui::Button("Change Texture"))
		{
			std::cout << "CHANGING TEXTURE\n";
			*texture = AssetManager::getInstance().textureLib.GetAsset(textureFilePath);
			//isSetup = false;
			//Setup();
			
		}

		const ImGuiPayload* payload = ImGui::GetDragDropPayload();
		if (payload != nullptr && payload->IsDataType("FILE_DRAG"))
		{
			ImGui::Text("<----- CHANGE MODEL ----->");
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
				{
					IM_ASSERT(payload->DataSize == 128);
					const char* payload_n = (const char*)payload->Data;

					std::string fileName(payload_n);
					if (fileName.substr(fileName.find_last_of(".")) == ".png")
					{
						std::cout << "Dropping MODEL!" << std::endl;

						// temporary fix, replace any backward slash with a forward slash
						std::replace(fileName.begin(), fileName.end(), '\\', '/');

						textureFilePath = fileName;
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
	}
}