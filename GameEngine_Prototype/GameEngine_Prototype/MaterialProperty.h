#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Serialization.h"
#include "GLM_Serialize.h"
#include "Texture.h"
//#include "AssetManager.h"

template<typename T>
class MaterialProperty
{
public:
	MaterialProperty() {};

	std::string propertyName = "";

	virtual T getValue() = 0;
	virtual void setValue(T val) = 0;
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
public:
	FloatProperty();
	virtual float getValue() override;
	virtual void setValue(float val) override;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
	}
};

class IntProperty : public MaterialProperty<int>
{
public:
	IntProperty();
	virtual int getValue() override;
	virtual void setValue(int val) override;
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
public:
	Vec2Property();
	virtual glm::vec2 getValue() override;
	virtual void setValue(glm::vec2 val) override;
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
public:
	Vec3Property();
	virtual glm::vec3 getValue() override;
	virtual void setValue(glm::vec3 val) override;
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
public:
	Vec4Property();
	virtual glm::vec4 getValue() override;
	virtual void setValue(glm::vec4 val) override;
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
	TextureProperty();

	virtual Texture* getValue() override;
	virtual void setValue(Texture* val) override;
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
//
//template<typename T>
//inline MaterialProperty<T>::MaterialProperty()
//{
//}
