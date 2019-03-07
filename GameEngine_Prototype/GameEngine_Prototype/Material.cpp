#include "Material.h"
#include "RenderManager.h"
#include "Shader.h"
#include "AssetManager.h"

//BOOST_CLASS_EXPORT_GUID(Material, "Material")

Material::Material(std::string _name, std::string vertPath, std::string fragPath, bool _useLight)
{
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
		shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
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

void Material::LoadTexture(const char * _textureFilePath)
{
	textureFilePath = _textureFilePath;
	AssetManager::LoadTextureAsset(textureFilePath.c_str(), &textureID);
}

void Material::DrawInspector()
{
	if (ImGui::TreeNode("Mat", "Material: %s", this->name.c_str()))
	{
		//ImGui::SameLine();
		//ImGui::Text(this->name.c_str());
		ImGuiInputTextFlags flags = 0;
	//	flags |= ImGuiInputTextFlags_CharsScientific;// | ~ImGuiInputTextFlags_CharsNoBlank;
		ImGui::InputText("VertPath", &vertexShaderPath[0], 32, flags);
		ImGui::InputText("FragPath", &fragmentShaderPath[0], 32, flags);
		ImGui::InputText("TexturePath", &textureFilePath[0], 32, flags);

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