#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Serialization.h"
#include "GLM_Serialize.h"
#include "Texture.h"

template<class T>
class MaterialProperty
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

class FloatProperty : public MaterialProperty<float>
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

class IntProperty : public MaterialProperty<int>
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

class Vec2Property : public MaterialProperty<glm::vec2>
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

class Vec3Property : public MaterialProperty<glm::vec3>
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

class Vec4Property : public MaterialProperty<glm::vec4>
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
class TextureProperty : public MaterialProperty<Texture*>
{
public:
	TextureProperty() : MaterialProperty<Texture*>() { value = nullptr; };
private:
	void LoadTextureFromPath(std::string filePath);
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive &ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
		ar & boost::serialization::make_nvp<std::string>("filePath", value->path);
	}
	template<class Archive>
	void load(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
		std::string filePath;
		ar & boost::serialization::make_nvp<std::string>("filePath", filePath);
		//value = AssetManager::getInstance().textureLib.GetAsset(filePath);
		LoadTextureFromPath(filePath);
	}
};
