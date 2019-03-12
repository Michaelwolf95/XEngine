#pragma once
#include "AssetLibrary.h"
class TextureLibrary : public AssetLibrary<std::string, unsigned int>
{
public:
	TextureLibrary();
	~TextureLibrary();
	unsigned int& LoadAsset(std::string filePath) override;
};

