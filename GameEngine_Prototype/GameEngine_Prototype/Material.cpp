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
	if (ImGui::TreeNode("Material"))
	{
		ImGui::Text(this->name.c_str());

		//bool updated = false;
		char vertPath[32];
		strcpy(vertPath, this->vertexShaderPath.c_str());// , );
		ImGui::InputText("VertPath", vertPath, 32);
		if (vertPath != this->vertexShaderPath.c_str())
		{
			this->vertexShaderPath = vertPath;
			//updated = true;
		}
		char fragPath[48];
		strcpy(fragPath, this->fragmentShaderPath.c_str());// , );
		ImGui::InputText("FragPath", fragPath, 32);
		if (fragPath != this->fragmentShaderPath.c_str())
		{
			this->fragmentShaderPath = fragPath;
		}
		char texturePath[32];
		strcpy(texturePath, this->textureFilePath.c_str());
		ImGui::InputText("TexturePath", texturePath, 32);
		if (texturePath != this->textureFilePath.c_str())
		{
			this->textureFilePath = texturePath;
		}

		ImGui::Checkbox("Use Light", &useLight);

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