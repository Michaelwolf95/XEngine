#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Texture.h"

template<typename T>
class MaterialProperty
{
public:
	//virtual MaterialProperty<T>() {};
	//virtual ~MaterialProperty<T>() {};
	//virtual MaterialProperty<T>(std::string name, T val);
	std::string propertyName;
	virtual void setValue(T val) = 0;
	virtual T getValue() = 0;
protected:
	T value;
};

class FloatProperty : public MaterialProperty<float>
{
public:
	float getValue() override;
	void setValue(float val) override;
};

class IntProperty : public MaterialProperty<int>
{
public:
	int getValue() override;
	void setValue(int val) override;
};

class Vec2Property : public MaterialProperty<glm::vec2>
{
public:
	glm::vec2 getValue() override;
	void setValue(glm::vec2 val) override;
};

class Vec3Property : public MaterialProperty<glm::vec3>
{
public:
	glm::vec3 getValue() override;
	void setValue(glm::vec3 val) override;
};

class Vec4Property : public MaterialProperty<glm::vec4>
{
public:
	glm::vec4 getValue() override;
	void setValue(glm::vec4 val) override;
};

// Uses REFERENCES to textures. They would be stored in 
class TextureProperty : public MaterialProperty<Texture&>
{
public:
	Texture& getValue() override;
	void setValue(Texture& val) override;
};
