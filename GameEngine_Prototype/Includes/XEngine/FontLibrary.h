#pragma once

#include "AssetLibrary.h"
#include "Text.h"
#include <string>

class FontLibrary : public AssetLibrary<std::string, Text*>
{
public:
	FontLibrary();
	~FontLibrary();
	void LoadAssetIntoLibrary(Text t);

protected:
	Text *&LoadAsset(std::string filePath);
};