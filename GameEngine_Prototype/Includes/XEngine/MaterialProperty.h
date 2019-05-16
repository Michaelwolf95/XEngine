#pragma once
#include <glm/glm.hpp>
#include <string>
#include "LibraryExport.h"
#include "Serialization.h"
#include "GLM_Serialize.h"
#include "Texture.h"
#include <string.h>
#include "Shader.h"

template<typename T>
class  MaterialProperty
{
public:
	MaterialProperty() { value = T(); };

	std::string propertyName = "";

	virtual T getValue() { return value; };
	virtual void setValue(T val) {
		value = val;
	};
protected:
	T value;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(propertyName);
	}
};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(MaterialProperty)

class ENGINE_API FloatProperty : public MaterialProperty<float>
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
		ar & BOOST_SERIALIZATION_NVP(value);
	}
};

class ENGINE_API IntProperty : public MaterialProperty<int>
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
		ar & BOOST_SERIALIZATION_NVP(value);
	}
};

class ENGINE_API Vec2Property : public MaterialProperty<glm::vec2>
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
		ar & BOOST_SERIALIZATION_NVP(value);
	}
};

class ENGINE_API Vec3Property : public MaterialProperty<glm::vec3>
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
		ar & BOOST_SERIALIZATION_NVP(value);
	}
};

class ENGINE_API Vec4Property : public MaterialProperty<glm::vec4>
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
		ar & BOOST_SERIALIZATION_NVP(value);
	}
};

// Uses REFERENCES to textures. They would be stored in...
class ENGINE_API  TextureProperty : public MaterialProperty<Texture*>
{
public:
	TextureProperty() : MaterialProperty<Texture*>() { value = nullptr; };

	void LoadTextureFromPath(std::string filePath);
	void Reload();

	void Bind(Shader* shader, unsigned int texIndex);
	void Unbind(Shader* shader, unsigned int texIndex);

	//std::string textureType = "texture_diffuse";

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive &ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
		ar & boost::serialization::make_nvp<std::string>("filePath", value->path);
		if (version > 1)
		{
			ar & boost::serialization::make_nvp<std::string>("textureType", value->type);
		}
	}
	template<class Archive>
	void load(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
		std::string filePath;
		ar & boost::serialization::make_nvp<std::string>("filePath", filePath);
		LoadTextureFromPath(filePath);

		//value = AssetManager::getInstance().textureLib.GetAsset(filePath);
		if (version > 1)
		{
			std::string textureType = "";
			ar & boost::serialization::make_nvp<std::string>("textureType", value->type);
		}

		

	}
};