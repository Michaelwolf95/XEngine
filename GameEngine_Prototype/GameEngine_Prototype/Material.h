#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Serialization.h"

/* TODO: The information we want to store about each material
should be stored in "material modules" that can be attached to it.
For now, we can just use some flags to check if its lit or not.
*/
class Material
{
public:
	Shader* shader;
	std::string textureFilePath;
	unsigned int textureID = 0;
	bool useLight = false;
	float ambientStrength = 1.0f;
	float specularStrength = 0.5f;
	glm::vec4 Color;
	Material(Shader* _shader = nullptr, bool _useLight = false);
	~Material();
	void Load(); // Load for openGL to draw.

	void LoadTexture(const char* _textureFilePath);

private:
	friend class boost::serialization::access;
	//friend std::ostream & operator<<(std::ostream &os, const Material &comp);
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_NVP(textureFilePath);
		ar & BOOST_SERIALIZATION_NVP(useLight);
		ar & BOOST_SERIALIZATION_NVP(ambientStrength);
		ar & BOOST_SERIALIZATION_NVP(specularStrength);
		ar & BOOST_SERIALIZATION_NVP(Color);

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version) // file_version
	{
		ar & BOOST_SERIALIZATION_NVP(textureFilePath);
		ar & BOOST_SERIALIZATION_NVP(useLight);
		ar & BOOST_SERIALIZATION_NVP(ambientStrength);
		ar & BOOST_SERIALIZATION_NVP(specularStrength);
		ar & BOOST_SERIALIZATION_NVP(Color);

		shader = RenderManager::defaultShader;
		if (!textureFilePath.empty())
		{
			LoadTexture(textureFilePath.c_str());
		}
	}

};

