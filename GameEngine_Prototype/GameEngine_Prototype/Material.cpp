#include "Material.h"
#include "RenderManager.h"
#include "Shader.h"
#include "AssetManager.h"

//#include "SceneEditor.h"
#include "imgui_inspector_extensions.h"
#include "imgui_stdlib.h"

//BOOST_CLASS_EXPORT_GUID(Material, "Material")

Material::Material(std::string _name, std::string vertPath, std::string fragPath, bool _useLight)
{
	colorProperty.propertyName = "color";
	colorProperty.setValue(glm::vec3(Color.x, Color.y, Color.z));
	vec3Properties.push_back(colorProperty);
	ambientProperty.propertyName = VAR_NAME(ambient);
	ambientProperty.setValue(ambient);
	vec3Properties.push_back(ambientProperty);
	diffuseProperty.propertyName = VAR_NAME(diffuse);
	diffuseProperty.setValue(diffuse);
	vec3Properties.push_back(diffuseProperty);
	specularProperty.propertyName = VAR_NAME(specular);
	specularProperty.setValue(specular);
	vec3Properties.push_back(specularProperty);
	shinyProperty.propertyName = VAR_NAME(shininess);
	shinyProperty.setValue(shininess);
	floatProperties.push_back(shinyProperty);
	name = _name;
	vertexShaderPath = vertPath;
	fragmentShaderPath = fragPath;

	useLight = _useLight;
	Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Init();
}

Material::Material()
{
	if (shader == nullptr)
	{
		shader = RenderManager::defaultShader;
	}
	Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Material::~Material()
{
	//shader = RenderManager::defaultShader;
	//useLight = false;
	//Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Material::Init()
{
	if (isInitialized)
	{
		return;
	}
	if ((vertexShaderPath.empty() || fragmentShaderPath.empty()) == false)
	{
		std::cout << "Loading Shaders for Material.." << vertexShaderPath << ", " <<fragmentShaderPath << std::endl;
		//shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
		shader = AssetManager::getInstance().shaderLib.GetAsset(vertexShaderPath, fragmentShaderPath);
	}
	else
	{
		shader = RenderManager::defaultShader;
	}

	if (!textureFilePath.empty())
	{
		LoadTexture(textureFilePath.c_str());
	}

	isInitialized = true;
}

void Material::Load()
{
	shader->setColor("MainColor", Color.x, Color.y, Color.z, Color.w);

	if (textureID > 0)
	{
		// bind Texture
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Material::Draw(std::vector<Light*> lights)
{
	if (useLight) {
		//std::cout << "Rendering lights in Draw material\n";
		int *counter = nullptr;
		std::string uniformString;

		int pointLightCount = 0;
		int globalLightCount = 0;
		int spotLightCount = 0;

		shader->setInt("numLights", RenderManager::getInstance().lights.size());

		for (Light* light : lights) {
			
			if (light->getTypeID() == Light::LightType::PointLight) {
				counter = &pointLightCount;
			}
			else if (light->getTypeID() == Light::LightType::GlobalLight) {
				counter = &globalLightCount;
			}
			
			uniformString = *light->getUniformName() + '[' + std::to_string(*counter) + "].";


			// Add light properties to shader.
			light->draw(shader, *counter);

			counter ? (*counter)++ : printf("ERROR: counter is NULL! (Materials->Draw)\n");
		}

		shader->setInt("numPointLights", pointLightCount);
		shader->setInt("numGlobalLights", globalLightCount);


		for (auto fp : floatProperties) {
			shader->setFloat(uniformString + fp.propertyName, fp.getValue());
		}

		for (auto ip : intProperties) {
			shader->setInt(uniformString + ip.propertyName, ip.getValue());
		}

		for (auto v2p : vec2Properties) {
			shader->setVec2(uniformString + v2p.propertyName, v2p.getValue());
		}

		for (auto v3p : vec3Properties) {
			shader->setVec3(uniformString + v3p.propertyName, v3p.getValue());
		}

		for (auto v4p : vec4Properties) {
			shader->setVec3(uniformString + v4p.propertyName, v4p.getValue());
		}
	}
}

void Material::LoadTexture(const char * _textureFilePath)
{
	//textureID = AssetManager::getInstance().textureLib.GetAsset(textureFilePath);
	textureFilePath = _textureFilePath;
	AssetManager::LoadTextureAsset(textureFilePath.c_str(), &textureID);
}

void Material::DrawInspector()
{
	if (ImGui::TreeNode(this, "Material: %s", this->name.c_str()))
	{
		ImGui::InputText("VertPath", &vertexShaderPath);

		ImGui::InputText("FragPath", &fragmentShaderPath);
		ImGui::InputText("TexturePath", &textureFilePath);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
			{
				IM_ASSERT(payload->DataSize == 32);
				const char* payload_n = (const char*)payload->Data;

				textureFilePath = payload_n;
			}
			ImGui::EndDragDropTarget();
		}

		//ImGuiInputTextFlags flags = 0;
		//	flags |= ImGuiInputTextFlags_CharsScientific;// | ~ImGuiInputTextFlags_CharsNoBlank;
		//ImGui::InputText("VertPath", &vertexShaderPath[0], 32, flags);
		//ImGui::InputText("FragPath", &fragmentShaderPath[0], 32, flags);
		//ImGui::InputText("TexturePath", &textureFilePath[0], 32, flags);

		ImGui::Checkbox("Use Light", &useLight);
		ImGui::ColorEdit4("Color", (float*)&Color);

		if (ImGui::Button("Update"))
		{
			isInitialized = false;
			Init();
		}

		ImGui::TreePop();
	}
}

//
//std::ostream & operator<<(std::ostream &os, const Material &material)
//{
//	return os << BOOST_SERIALIZATION_NVP(material);
//	//return os << material.vertexShaderPath;// << ;
//}