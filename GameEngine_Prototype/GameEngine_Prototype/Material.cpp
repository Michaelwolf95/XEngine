#include "Material.h"
#include "RenderManager.h"
#include "Shader.h"
#include "AssetManager.h"

//#include "SceneEditor.h"
#include "imgui_inspector_extensions.h"
#include "imgui_stdlib.h"
#include "Material.h"
#include "AssetManager.h"
using namespace XEngine;
#include "MaterialProperty.h"
#include "Serialization.h"

//BOOST_CLASS_EXPORT_IMPLEMENT(Material)
//BOOST_CLASS_EXPORT_GUID(Material, "Material")
//BOOST_CLASS_VERSION(Material, 2);

//namespace XEngine {
	Material::Material(std::string _name, std::string vertPath, std::string fragPath, bool _useLight)
	{
		//std::cout << "Material constructor called with arguments\n";
		//std::cout << "\t_name: " << _name			<< std::endl;
		//std::cout << "\tvertPath: "	<< vertPath		<< std::endl;
		//std::cout << "\tfragPath " << fragPath 		<< std::endl;
		//std::cout << "\t_useLight: " << _useLight	<< std::endl;

		name = _name;
		vertexShaderPath = vertPath;
		fragmentShaderPath = fragPath;

		useLight = _useLight;

		//filePath = "../Assets/Materials/MultiLightModel.material";
		filePath = ASSET_FILE_PATH + std::string("Materials/") + this->name + ".material";


		Init();

		if (vertexShaderPath == "multilights.shader") // TODO: Remove when all shaders are re-formatted
		{
			shader->vFilePath = "../Shaders/" + vertexShaderPath;
			parseShaderFileForProperties(shader->vFilePath);
		}

		//std::cout << to_string() << std::endl;

	}

	Material::Material()
	{
		//std::cout << "Material default constructor with defaultShader call\n";
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
		if (shader == nullptr)
		{
			if (vertexShaderPath.empty() == false)
			{
				//std::cout << "Loading Shaders for Material" << std::endl
				//	<< "\tname: " << this->name << std::endl
				//	<< "\tvertexShaderPath: " << vertexShaderPath << std::endl
				//	<< "\tfragmentShaderPath: " << fragmentShaderPath << std::endl;

				//shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
				shader = AssetManager::getInstance().shaderLib.GetAsset(vertexShaderPath, fragmentShaderPath);
				if (vertexShaderPath == "multilights.shader") // TODO: Remove when all shaders are re-formatted
				{
					shader->vFilePath = "../Shaders/" + vertexShaderPath;
					//parseFileForProperties(shader->vFilePath);
				}
			}
			else
			{
				std::cout << "ERROR LOADING SHADER - Loading default shader instead." << std::endl;
				shader = RenderManager::defaultShader;
			}
		}

		for (size_t i = 0; i < textureProperties.size(); i++)
		{
			textureProperties[i].Reload();
		}

		isInitialized = true;
	}

	// Load material to draw.
	void Material::Load()
	{
		shader->use();
		RenderManager::getInstance().currentShaderID = shader->ID;

		if (useLight) {

			shader->setVec4("sceneAmbience", RenderManager::getInstance().getCurrentCamera()->clearColor);
			shader->setFloat("sceneAmbienceStrength", 0.6f); // hard coded for now

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
				else if (light->getTypeID() == Light::LightType::SpotLight) {
					counter = &spotLightCount;
				}

				uniformString = *light->getUniformName() + '[' + std::to_string(*counter) + "].";

				// Add light properties to shader.
				light->Draw(shader, *counter);

				counter ? (*counter)++ : printf("ERROR: counter is NULL! (Materials->Draw)\n");
			}
			shader->setInt("numPointLights", pointLightCount);
			shader->setInt("numGlobalLights", globalLightCount);
			shader->setInt("numSpotLights", spotLightCount);
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

		for (size_t i = 0; i < textureProperties.size(); i++)
		{
			textureProperties[i].Bind(shader, i);
		}

		
	}
	// Unload material after drawing. (Unbind textures, etc.)
	void Material::Unload()
	{
		for (size_t i = 0; i < textureProperties.size(); i++)
		{
			textureProperties[i].Unbind(shader, i);
		}
		
	}


	void Material::parseShaderFileForProperties(std::string path)
	{
		//std::cout << "Material::parseFileForProperties with arguments\n";
		//std::cout << "\tpath: " << path << std::endl;

		// clear properties
		floatProperties.clear();
		intProperties.clear();
		vec2Properties.clear();
		vec3Properties.clear();
		vec4Properties.clear();
		//textureProperties.clear();

		try
		{
			std::string line;
			std::ifstream file;
			bool readToProps = false;
			file.open(path, std::ifstream::out);

			while (!file.eof())
			{
				getline(file, line, '\n');

				if (line == "#MATERIAL_PROPERTIES")
				{
					readToProps = true;
					getline(file, line, '\n');
				}
				if (line == "#!MATERIAL_PROPERTIES")
				{
					file.close();
					break;
				}

				std::vector<std::string> prop;
				std::stringstream ss(line);
				std::string token;
				while (getline(ss, token, ' '))
					prop.push_back(token);

				if (prop.at(0) == "float")
				{
					FloatProperty p;
					p.setValue(std::stof(prop.at(2)));
					p.propertyName = prop.at(1);
					floatProperties.push_back(p);
				}
				if (prop.at(0) == "int")
				{
					IntProperty p;
					p.setValue(std::stoi(prop.at(2)));
					p.propertyName = prop.at(1);
					intProperties.push_back(p);
				}
				if (prop.at(0) == "vec4")
				{
					Vec4Property p;
					p.setValue(glm::vec4(std::stof(prop.at(2))));
					p.propertyName = prop.at(1);
					vec4Properties.push_back(p);
				}
				if (prop.at(0) == "vec3")
				{
					Vec3Property p;
					p.setValue(glm::vec3(std::stof(prop.at(2))));
					p.propertyName = prop.at(1);
					vec3Properties.push_back(p);
				}
				if (prop.at(0) == "vec2")
				{
					Vec2Property p;
					p.setValue(glm::vec2(std::stof(prop.at(2))));
					p.propertyName = prop.at(1);
					vec2Properties.push_back(p);
				}
				//if (prop.at(0) == "sampler2D")
				//{
				//	TextureProperty p;
				//	//p.setValue(glm::vec3(std::stof(prop.at(2))));
				//	p.propertyName = prop.at(1);
				//	textureProperties.push_back(p);
				//}
				// TODO: more property types
			}
			file.close();
		}
		catch (std::exception e)
		{
			std::cout << "ERROR::MATERIAL::PARSE" << std::endl;
		}
	}

	void Material::getDefaultProperties()
	{
		std::cout << "Retrieving default properties for shaders stored at "
			<< shader->vFilePath << std::endl;
		parseShaderFileForProperties(shader->vFilePath);
	}

	void Material::DrawInspector()
	{
		ImGui::PushID(this);
		if (ImGui::TreeNode(this, "Material: %s", this->name.c_str()))
		{
			ImGui::InputText("Name", &name);

			ImGui::InputText("VertPath", &vertexShaderPath);

			ImGui::InputText("FragPath", &fragmentShaderPath);
			//ImGui::InputText("TexturePath", &textureFilePath);
			//if (ImGui::BeginDragDropTarget())
			//{
			//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
			//	{
			//		IM_ASSERT(payload->DataSize == 128);
			//		const char* payload_n = (const char*)payload->Data;

			//		textureFilePath = payload_n;
			//	}
			//	ImGui::EndDragDropTarget();
			//}

			ImGui::Checkbox("Use Light", &useLight);
			//ImGui::ColorEdit4("Color", (float*)&Color); // vec4 property
			std::string propName = this->name + "_Floats";
			if (ImGui::TreeNode(propName.c_str(), "Float Properties", "%s_Floats", this->name.c_str()))
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
			propName = this->name + "_Ints";
			if (ImGui::TreeNode(propName.c_str(), "Int Properties", "%s_Ints", this->name.c_str()))
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
			propName = this->name + "_Vec2";
			if (ImGui::TreeNode(propName.c_str(), "Vec2 Properties", "%s_Vec2s", this->name.c_str()))
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
			propName = this->name + "_Vec3";
			if (ImGui::TreeNode(propName.c_str(), "Vec3 Properties", "%s_Vec3s", this->name.c_str()))
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
			propName = this->name + "_Vec4";
			if (ImGui::TreeNode(propName.c_str(), "Vec4 Properties", "%s_Vec4", this->name.c_str()))
			{
				for (size_t i = 0; i < vec4Properties.size(); i++)
				{
					glm::vec4 value = vec4Properties[i].getValue();

					ImGui::ColorEdit4(vec4Properties[i].propertyName.c_str(), (float *)&value);

					if (value != vec4Properties[i].getValue())
						vec4Properties[i].setValue(value);
				}
				ImGui::TreePop();
			}
			propName = this->name + "_Textures";
			if (ImGui::TreeNode(propName.c_str(), "Texture Properties", "%s_Textures", this->name.c_str()))
			{
				for (size_t i = 0; i < textureProperties.size(); i++)
				{
					std::string texPropName = propName + "_" + std::to_string(i);
					Texture* value = textureProperties[i].getValue();
					std::string path = value->path;

					//GUI::FileReference(value->path,)
					ImGui::InputText((texPropName).c_str(), &textureProperties[i].getValue()->path);
					const ImGuiPayload* payload = ImGui::GetDragDropPayload();
					{
						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
							{
								IM_ASSERT(payload->DataSize == 128);
								const char* payload_n = (const char*)payload->Data;

								std::string filePath(payload_n);
								std::string ext = filePath.substr(filePath.find_last_of("."));
								if (ext == ".png" || ext == ".jpg" || ext == ".tga")
								{
									std::replace(filePath.begin(), filePath.end(), '\\', '/');
									//path.assign(filePath);
									textureProperties[i].getValue()->path.assign(filePath);
								}
							}
							ImGui::EndDragDropTarget();
						}
					}
					ImGui::Indent();
					ImGui::InputText((texPropName + "_Type").c_str(), &textureProperties[i].getValue()->type);
					int mode = textureProperties[i].getValue()->loadMode;
					ImGui::InputInt((texPropName + "_Mode").c_str(), &mode);
					textureProperties[i].getValue()->loadMode = mode;
					ImGui::Unindent();
				}
				std::vector<std::string> textureFileExts;
				textureFileExts.push_back(".png"); textureFileExts.push_back(".jpg"); textureFileExts.push_back(".tga");
				static std::string tempPath = "";
				GUI::FileReference(tempPath, textureFileExts, "New");
				//ImGui::InputText("New", &tempPath);
				if (ImGui::Button("Add"))
				{
					if (tempPath.length() > 0)
					{
						Texture* texture = (AssetManager::getInstance().textureLib.GetAsset(tempPath));

						// turn into textureProperty
						TextureProperty textureProp;
						textureProp.setValue(texture);

						textureProperties.push_back(textureProp);
					}
				}
				if (textureProperties.size() > 0)
				{
					ImGui::SameLine();
					if (ImGui::Button("Delete"))
					{
						textureProperties.pop_back();
					}
				}

				ImGui::TreePop();
			}

			//const ImGuiPayload* payload = ImGui::GetDragDropPayload();
			//if (payload != nullptr && payload->IsDataType("FILE_DRAG"))
			//{
			//	ImGui::Text("<----- CHANGE MATERIAL ----->");
			//	if (ImGui::BeginDragDropTarget())
			//	{
			//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
			//		{
			//			IM_ASSERT(payload->DataSize == 128);
			//			const char* payload_n = (const char*)payload->Data;

			//			std::string filePath(payload_n);
			//			if (filePath.substr(filePath.find_last_of(".")) == ".material")
			//			{
			//				std::cout << "Dropping Material!" << std::endl;
			//				//fileName = fileName.substr(fileName.find_last_of("\\") + 1); // NOTE: MAKE SURE THIS WORKS ON ALL SYSTEMS!!!
			//				//size_t lastindex = fileName.find_last_of(".");
			//				//fileName = fileName.substr(0, lastindex);
			//				Material* mat = AssetManager::getInstance().materialLib.GetAsset(filePath);
			//				if (mat != nullptr)
			//				{

			//					
			//					*this = *mat;
			//					std::cout << "Dropping Material!" << std::endl;
			//				}
			//			}
			//		}
			//		ImGui::EndDragDropTarget();
			//	}
			//}


			if (ImGui::Button("Save Material"))
			{
				AssetManager::getInstance().materialLib.SaveMaterialToFile(*this, this->filePath.c_str());
				isInitialized = false;
				Init(); // TODO: How does this update?
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
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
			str += '\n' + tp.propertyName + ": " + tp.getValue()->type;
		}

		str += "\nfilePath: " + this->filePath;
		str += "\nFragmentShaderPath: " + this->fragmentShaderPath;
		str += "\nvertexShaderPath: " + this->vertexShaderPath;
		str += "\nisInitialized: "; str += this->isInitialized ? "True" : "False";
		str += this->shader->to_string();
		str += "\nEnd of material's contents.";
		return str;
	}


//}