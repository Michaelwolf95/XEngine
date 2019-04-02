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
	std::cout << "Material constructor called with arguments\n";
	std::cout << "\t_name: " << _name			<< std::endl;
	std::cout << "\tvertPath: "	<< vertPath		<< std::endl;
	std::cout << "\tfragPath " << fragPath 		<< std::endl;
	std::cout << "\t_useLight: " << _useLight	<< std::endl;

	name = _name;
	vertexShaderPath = vertPath;
	fragmentShaderPath = fragPath;

	useLight = _useLight;

	//filePath = "../Assets/Materials/MultiLightModel.material";
	filePath = "../Assets/Materials/" + this->name + ".material";


	// Temporary until system put in place to add values to individual properties
	float shininess = 32.0f;
	FloatProperty shinyProperty;
	glm::vec3 ambient = glm::vec3(0.05f);
	Vec3Property ambientProperty;
	glm::vec3 diffuse = glm::vec3(0.8f);
	Vec3Property diffuseProperty;
	glm::vec3 specular = glm::vec3(1.0f);
	Vec3Property specularProperty;
	glm::vec4 color = glm::vec4(1.0f);
	Vec4Property colorProperty;

	colorProperty.propertyName = VAR_NAME(color);
	colorProperty.setValue(color);
	ambientProperty.propertyName = VAR_NAME(ambient);
	ambientProperty.setValue(ambient);
	diffuseProperty.propertyName = VAR_NAME(diffuse);
	diffuseProperty.setValue(diffuse);
	specularProperty.propertyName = VAR_NAME(specular);
	specularProperty.setValue(specular);
	shinyProperty.propertyName = VAR_NAME(shininess);
	shinyProperty.setValue(shininess);
	
	floatProperties.push_back(shinyProperty);
	vec3Properties.push_back(ambientProperty);
	vec3Properties.push_back(diffuseProperty);
	vec3Properties.push_back(specularProperty);
	vec4Properties.push_back(colorProperty);

	Init();

	//std::cout << to_string() << std::endl;

}

Material::Material()
{
	std::cout << "Material default constructor with defaultShader call\n";
	if (shader == nullptr)
	{
		shader = RenderManager::defaultShader;
	}
	//Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
	std::cout << "Initializing Material: " << name << std::endl;
	if ((vertexShaderPath.empty() /*|| fragmentShaderPath.empty()*/) == false)
	{

		std::cout << "Loading Shaders for Material" << std::endl
			<< "\tname: " << this->name << std::endl
			<< "\tvertexShaderPath: " << vertexShaderPath << std::endl
			<< "\tfragmentShaderPath: " << fragmentShaderPath << std::endl;
		//shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
		shader = AssetManager::getInstance().shaderLib.GetAsset(vertexShaderPath, fragmentShaderPath);
	}
	else
	{
		std::cout << "Loading default Shaders..." << std::endl;
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
	//shader->setColor("color", Color.x, Color.y, Color.z, Color.w); // this should be done with vec4 properties
	shader->use();
	RenderManager::getInstance().currentShaderID = shader->ID;


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

void Material::Draw()
{
	shader->use();

	if (useLight) {
		//std::cout << "Rendering lights in Draw material\n";
		int *counter = nullptr;
		std::string uniformString;

		int pointLightCount = 0;
		int globalLightCount = 0;
		int spotLightCount = 0;

		shader->setInt("numLights", RenderManager::getInstance().lights.size());

		//shader->setInt("Texture", textureID); // would this be compatible with meshes?

		//std::cout << to_string() << std::endl;

		for (Light* light : RenderManager::getInstance().lights) {
			
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
	}
	// Non-light uniforms
	for (auto fp : floatProperties) {
		shader->setFloat(fp.propertyName, fp.getValue());
	}
	for (auto ip : intProperties) {
		shader->setInt(ip.propertyName, ip.getValue());
	}
	for (auto v2p : vec2Properties) {
		shader->setVec2(v2p.propertyName, v2p.getValue());
	}
	for (auto v3p : vec3Properties) {
		shader->setVec3(v3p.propertyName, v3p.getValue());
	}
	for (auto v4p : vec4Properties) {
		shader->setVec4(v4p.propertyName, v4p.getValue());
	}
}

void Material::LoadTexture(const char * _textureFilePath)
{
	//textureID = AssetManager::getInstance().textureLib.GetAsset(textureFilePath);
	textureFilePath = _textureFilePath;
	AssetManager::LoadTexture(textureFilePath.c_str(), &textureID);

	AssetManager::LoadTextureAsset(textureFilePath.c_str(), &textureID);
}

void Material::DrawInspector()
{
	if (ImGui::TreeNode(this, "Material: %s", this->name.c_str()))
	{
		ImGui::InputText("Name", &name);

		ImGui::InputText("VertPath", &vertexShaderPath);

		ImGui::InputText("FragPath", &fragmentShaderPath);
		ImGui::InputText("TexturePath", &textureFilePath);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
			{
				IM_ASSERT(payload->DataSize == 128);
				const char* payload_n = (const char*)payload->Data;

				textureFilePath = payload_n;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Checkbox("Use Light", &useLight);
		//ImGui::ColorEdit4("Color", (float*)&Color); // vec4 property

		if (ImGui::TreeNode(this, "Float Properties", "%s_Floats", this->name.c_str()))
		{
			for (size_t i = 0; i < floatProperties.size(); i++)
			{
				float value = floatProperties[i].getValue();

				ImGui::InputFloat(floatProperties[i].propertyName.c_str(), &value);

				if (value != floatProperties[i].getValue())
					floatProperties[i].setValue(value);
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode(this, "Int Properties", "%s_Ints", this->name.c_str()))
		{
			for (size_t i = 0; i < intProperties.size(); i++)
			{
				int value = intProperties[i].getValue();

				ImGui::InputInt(intProperties[i].propertyName.c_str(), &value);

				if (value != intProperties[i].getValue())
					intProperties[i].setValue(value);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode(this, "Vec2 Properties", "%s_Vec2s", this->name.c_str()))
		{
			for (size_t i = 0; i < vec2Properties.size(); i++)
			{
				glm::vec2 value = vec2Properties[i].getValue();

				ImGui::InputFloat2(vec2Properties[i].propertyName.c_str(), (float*)&value);

				if (value != vec2Properties[i].getValue())
					vec2Properties[i].setValue(value);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode(this, "Vec3 Properties", "%s_Vec3s", this->name.c_str()))
		{
			for (size_t i = 0; i < vec3Properties.size(); i++)
			{
				glm::vec3 value = vec3Properties[i].getValue();

				ImGui::ColorEdit3(vec3Properties[i].propertyName.c_str(), (float *)&value);

				if (value != vec3Properties[i].getValue())
					vec3Properties[i].setValue(value);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode(this, "Vec4 Properties", "%s_Vec4", this->name.c_str()))
		{
			for (size_t i = 0; i < vec4Properties.size(); i++)
			{			
				glm::vec4 value = vec4Properties[i].getValue();

				vec4Properties[i].propertyName == "color" ? 
					ImGui::ColorEdit4(vec4Properties[i].propertyName.c_str(), (float *)&value) :
					ImGui::InputFloat3(vec4Properties[i].propertyName.c_str(), (float *)&value);

				if (value != vec4Properties[i].getValue())
					vec4Properties[i].setValue(value);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode(this, "Texture Properties", "%s_Textures", this->name.c_str()))
		{
			for (size_t i = 0; i < textureProperties.size(); i++)
			{
				Texture* value = textureProperties[i]->getValue();
				std::string path = value->path;

				ImGui::InputText(textureProperties[i]->propertyName.c_str(), &path);

				if (path != value->path)
				{
					value->path = path;
				}
			}
			ImGui::TreePop();
		}

		const ImGuiPayload* payload = ImGui::GetDragDropPayload();
		if (payload != nullptr && payload->IsDataType("FILE_DRAG"))
		{
			ImGui::Text("<----- CHANGE MATERIAL ----->");
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
				{
					IM_ASSERT(payload->DataSize == 128);
					const char* payload_n = (const char*)payload->Data;

					std::string fileName(payload_n);
					if (fileName.substr(fileName.find_last_of(".")) == ".material")
					{
						std::cout << "Dropping Material!" << std::endl;
						//fileName = fileName.substr(fileName.find_last_of("\\") + 1); // NOTE: MAKE SURE THIS WORKS ON ALL SYSTEMS!!!
						//size_t lastindex = fileName.find_last_of(".");
						//fileName = fileName.substr(0, lastindex);
						Material* mat = AssetManager::getInstance().materialLib.GetAsset(fileName);
						if (mat != nullptr)
						{
							*this = *mat;
							std::cout << "Dropping Material!" << std::endl;
						}
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
		

		if (ImGui::Button("Update"))
		{
			isInitialized = false;
			Init();

			AssetManager::getInstance().materialLib.SaveMaterialToFile(*this, this->filePath.c_str());
		}

		ImGui::TreePop();
	}
}

std::string Material::to_string()
{
	std::string str = "\nPrint contents of material (" + name + "): ";
	for (auto cp : floatProperties) {
		str += '\n' + cp.propertyName + ": " + std::to_string(cp.getValue());
	}
	for (auto ip : intProperties) {
		str += '\n' + ip.propertyName + ": " + std::to_string(ip.getValue());
	}
	for (auto v2p : vec2Properties) {
		str += '\n' + v2p.propertyName + ": x = " + std::to_string(v2p.getValue().x) + " y = " + std::to_string(v2p.getValue().y);

	}
	for (auto v3p : vec3Properties) {
		str += '\n' + v3p.propertyName + ": x = " + std::to_string(v3p.getValue().x) + " y = " + std::to_string(v3p.getValue().y) + " z = " + std::to_string(v3p.getValue().z);
											  
	}										  
	for (auto v4p : vec4Properties) {		  
		str += '\n' + v4p.propertyName + ": x = " + std::to_string(v4p.getValue().x) + " y = " + std::to_string(v4p.getValue().y) + " z = " + std::to_string(v4p.getValue().z) + " w = " + std::to_string(v4p.getValue().w);
											  
	}
	for (auto tp : textureProperties) {
		str += '\n' + tp->propertyName + ": " + tp->getValue()->type;
	}

	str += "\nfilePath: " + this->filePath;
	str += "\nFragmentShaderPath: " + this->fragmentShaderPath;
	str += "\nvertexShaderPath: " + this->vertexShaderPath;
	str += "\nisInitialized: "; str += this->isInitialized ? "True" : "False";
	str += this->shader->to_string();
	str += "\nEnd of material's contents.";
	return str;
}

//
//std::ostream & operator<<(std::ostream &os, const Material &material)
//{
//	return os << BOOST_SERIALIZATION_NVP(material);
//	//return os << material.vertexShaderPath;// << ;
//}