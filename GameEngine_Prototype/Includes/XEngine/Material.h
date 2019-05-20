#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "LibraryExport.h"
#include "Shader.h"
#include "Light.h"
#include "Serialization.h"
#include <vector>
#include "Texture.h"
#include "MaterialProperty.h"

//namespace XEngine {

	class ENGINE_API Material
	{
	public:
		// Shader attributes
		std::string name; // name of mesh, used to mapped to material in MeshRenderer
		std::string vertexShaderPath;
		std::string fragmentShaderPath;
		std::string filePath; // the filepath of obj for boost serialization
		Shader* shader;

		std::vector<FloatProperty> floatProperties;
		std::vector<IntProperty> intProperties;
		std::vector<Vec2Property> vec2Properties;
		std::vector<Vec3Property> vec3Properties;
		std::vector<Vec4Property> vec4Properties;
		std::vector<TextureProperty> textureProperties;

		bool useLight = false;
		//std::vector<Texture> textures;
		//std::string textureFilePath;
		//unsigned int textureID = 0;

		Material(std::string _name, std::string vertPath = "", std::string fragPath = "", bool _useLight = true);
		Material();
		~Material();
		void Init();
		//void Load(); 
		void Load(); // Load for openGL to draw.
		void Unload();
		//void LoadTexture(const char* _textureFilePath);
		void getDefaultProperties();
		void DrawInspector();
		std::string to_string();

	private:
		void parseShaderFileForProperties(std::string path);
		bool isInitialized = false;

		friend class boost::serialization::access;
		//friend std::ostream & operator<<(std::ostream &os, const Material &material);
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			std::cout << "Saving Material: " << name <<", Version= " << version << std::endl;
			//std::cout << "save " << textureFilePath << std::endl;
			if (version <= 0)
			{
				ar & BOOST_SERIALIZATION_NVP(textureProperties);
			}
			ar & BOOST_SERIALIZATION_NVP(name);
			ar & BOOST_SERIALIZATION_NVP(vertexShaderPath);
			ar & BOOST_SERIALIZATION_NVP(fragmentShaderPath);
			ar & BOOST_SERIALIZATION_NVP(filePath);
			ar & BOOST_SERIALIZATION_NVP(useLight);
			if (version < 2)
			{
				std::string textureFilePath = "";
				ar & BOOST_SERIALIZATION_NVP(textureFilePath);
			}
			ar & BOOST_SERIALIZATION_NVP(floatProperties);
			ar & BOOST_SERIALIZATION_NVP(intProperties);
			ar & BOOST_SERIALIZATION_NVP(vec2Properties);
			ar & BOOST_SERIALIZATION_NVP(vec3Properties);
			ar & BOOST_SERIALIZATION_NVP(vec4Properties);
			if (version > 0)
			{
				ar & BOOST_SERIALIZATION_NVP(textureProperties);
			}
			//ar & BOOST_SERIALIZATION_NVP(shader); //serialize is not a member of Shader
		}
		template<class Archive>
		void load(Archive & ar, const unsigned int version) // file_version
		{
			std::cout << "Loading Material. Version= " << version << std::endl;
			if (version <= 0)
			{
				ar & BOOST_SERIALIZATION_NVP(textureProperties);
			}
			ar & BOOST_SERIALIZATION_NVP(name);
			ar & BOOST_SERIALIZATION_NVP(vertexShaderPath);
			ar & BOOST_SERIALIZATION_NVP(fragmentShaderPath);
			ar & BOOST_SERIALIZATION_NVP(filePath);
			ar & BOOST_SERIALIZATION_NVP(useLight);
			if (version < 2)
			{
				std::string textureFilePath;
				//ar & BOOST_SERIALIZATION_NVP(textureFilePath);
				ar & boost::serialization::make_nvp("textureFilePath", textureFilePath);
			}
			ar & BOOST_SERIALIZATION_NVP(floatProperties);
			ar & BOOST_SERIALIZATION_NVP(intProperties);
			ar & BOOST_SERIALIZATION_NVP(vec2Properties);
			ar & BOOST_SERIALIZATION_NVP(vec3Properties);
			ar & BOOST_SERIALIZATION_NVP(vec4Properties);
			if (version > 0)
			{
				ar & BOOST_SERIALIZATION_NVP(textureProperties);
			}
			//isInitialized = true;
			//std::cout << "load " << textureFilePath << std::endl;
			Init();
			//shader = AssetManager::getInstance().shaderLib.GetAsset(vertexShaderPath, fragmentShaderPath);

		}
	};
	//BOOST_CLASS_IMPLEMENTATION(Material, boost::serialization::track_always)
	BOOST_CLASS_VERSION(Material, 2);
//}
//BOOST_CLASS_EXPORT

//std::ostream & operator<<(std::ostream &os, const Material &material);
//BOOST_SERIALIZATION_
#endif // !1