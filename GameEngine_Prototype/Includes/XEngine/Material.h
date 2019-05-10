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
#include <boost/serialization/vector.hpp>
#include <vector>
#include "Texture.h"
#include "MaterialProperty.h"

#define VAR_NAME(Var) (#Var)


/* TODO: The information we want to store about each material
could be stored in "material modules" that can be attached to it.
For now, we can just use some flags to check if its lit or not.
*/


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
	std::vector<Texture> textures;
	std::string textureFilePath;
	unsigned int textureID = 0;

	Material(std::string _name, std::string vertPath = "", std::string fragPath = "", bool _useLight = true);
	Material();
	~Material();
	void Init();
	void Load(); // Load for openGL to draw.
	void Draw();
	void LoadTexture(const char* _textureFilePath);
	void getDefaultProperties();
	void DrawInspector();
	std::string to_string();

private:
	void parseFileForProperties(std::string path);
	bool isInitialized = false;

	friend class boost::serialization::access;
	//friend std::ostream & operator<<(std::ostream &os, const Material &material);
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		std::cout << "save " << textureFilePath << std::endl;

		ar & BOOST_SERIALIZATION_NVP(textureProperties);
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(vertexShaderPath);
		ar & BOOST_SERIALIZATION_NVP(fragmentShaderPath);
		ar & BOOST_SERIALIZATION_NVP(filePath);
		ar & BOOST_SERIALIZATION_NVP(useLight);
		ar & BOOST_SERIALIZATION_NVP(textureFilePath);
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
		ar & BOOST_SERIALIZATION_NVP(textureProperties);
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(vertexShaderPath);
		ar & BOOST_SERIALIZATION_NVP(fragmentShaderPath);
		ar & BOOST_SERIALIZATION_NVP(filePath);
		ar & BOOST_SERIALIZATION_NVP(useLight);
		ar & BOOST_SERIALIZATION_NVP(textureFilePath);
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
		std::cout << "load " << textureFilePath << std::endl;
		Init();
		//shader = AssetManager::getInstance().shaderLib.GetAsset(vertexShaderPath, fragmentShaderPath);
		
	}
};
//BOOST_CLASS_EXPORT

//std::ostream & operator<<(std::ostream &os, const Material &material);
//BOOST_SERIALIZATION_
#endif // !1