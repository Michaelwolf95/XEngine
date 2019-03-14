#pragma once
#include <string>
#include "Texture.h"

template<typename T>
class MaterialProperty
{
public:
	std::string propertyName;
	virtual void setValue(T val) = 0;
	virtual T getValue() = 0;
protected:
	T value;
};

class FloatProperty : public MaterialProperty<float>
{
	float getValue() override;
	void setValue(float val) override;
};
// Uses REFERENCES to textures. They would be stored in 
class TextureProperty : public MaterialProperty<Texture&>
{
	Texture& getValue() override;
	void setValue(Texture& val) override;
};