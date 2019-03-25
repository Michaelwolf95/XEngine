#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Light.h"
#include "Serialization.h"
#include <vector>
#include "Texture.h"
#include "MaterialProperty.h"

#define VAR_NAME(Var) (#Var)


/* TODO: The information we want to store about each material
could be stored in "material modules" that can be attached to it.
For now, we can just use some flags to check if its lit or not.
*/


class Material
{
public:
	// Shader attributes
	glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec3Property colorProperty;
	glm::vec3 ambient = glm::vec3(0.05f);
	Vec3Property ambientProperty;
	glm::vec3 diffuse = glm::vec3(0.8f);
	Vec3Property diffuseProperty;
	glm::vec3 specular = glm::vec3(1.0f);
	Vec3Property specularProperty;
	float shininess = 32.0f;
	FloatProperty shinyProperty;

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

	//std::vector<MaterialPropertyBase*>properties;


	bool useLight = false;
	std::vector<Texture> textures;

	std::string textureFilePath;
	unsigned int textureID = 0;
	
	//float ambientStrength = 1.0f;
	//float specularStrength = 0.5f;
	Material(std::string _name, std::string vertPath, std::string fragPath, bool _useLight = true);
	Material();
	~Material();
	void Init();
	void Load(); // Load for openGL to draw.
	void Draw(std::vector<Light*> lights);

	void LoadTexture(const char* _textureFilePath);

	void DrawInspector();

	std::string to_string();

private:
	bool isInitialized = false;

	friend class boost::serialization::access;
	//friend std::ostream & operator<<(std::ostream &os, const Material &material);
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(vertexShaderPath);
		ar & BOOST_SERIALIZATION_NVP(fragmentShaderPath);
		ar & BOOST_SERIALIZATION_NVP(Color);
		ar & BOOST_SERIALIZATION_NVP(useLight);
		ar & BOOST_SERIALIZATION_NVP(textureFilePath);
		//ar & BOOST_SERIALIZATION_NVP(specularStrength);
		//ar & BOOST_SERIALIZATION_NVP(ambientStrength);
		ar & BOOST_SERIALIZATION_NVP(ambient);
		ar & BOOST_SERIALIZATION_NVP(diffuse);
		ar & BOOST_SERIALIZATION_NVP(specular);
		ar & BOOST_SERIALIZATION_NVP(shininess);
		ar & BOOST_SERIALIZATION_NVP(floatProperties);
		ar & BOOST_SERIALIZATION_NVP(intProperties);
		ar & BOOST_SERIALIZATION_NVP(vec2Properties);
		ar & BOOST_SERIALIZATION_NVP(vec3Properties);
		ar & BOOST_SERIALIZATION_NVP(vec4Properties);
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(filePath);
		ar & BOOST_SERIALIZATION_NVP(textureProperties);
		//ar & BOOST_SERIALIZATION_NVP(shader); //serialize is not a member of Shader
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version) // file_version
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(vertexShaderPath);
		ar & BOOST_SERIALIZATION_NVP(fragmentShaderPath);
		ar & BOOST_SERIALIZATION_NVP(Color);
		ar & BOOST_SERIALIZATION_NVP(useLight);
		ar & BOOST_SERIALIZATION_NVP(textureFilePath);
		//ar & BOOST_SERIALIZATION_NVP(specularStrength);
		//ar & BOOST_SERIALIZATION_NVP(ambientStrength);
		ar & BOOST_SERIALIZATION_NVP(ambient);
		ar & BOOST_SERIALIZATION_NVP(diffuse);
		ar & BOOST_SERIALIZATION_NVP(specular);
		ar & BOOST_SERIALIZATION_NVP(shininess);
		ar & BOOST_SERIALIZATION_NVP(floatProperties);
		ar & BOOST_SERIALIZATION_NVP(intProperties);
		ar & BOOST_SERIALIZATION_NVP(vec2Properties);
		ar & BOOST_SERIALIZATION_NVP(vec3Properties);
		ar & BOOST_SERIALIZATION_NVP(vec4Properties);
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(filePath);
		ar & BOOST_SERIALIZATION_NVP(textureProperties);
		//ar & BOOST_SERIALIZATION_NVP(shader); //serialize is not a member of Shader

		isInitialized = false;
		Init();
		//shader = RenderManager::defaultShader;
		
	}

};
//BOOST_CLASS_EXPORT

//std::ostream & operator<<(std::ostream &os, const Material &material);
//BOOST_SERIALIZATION_
#endif // !1