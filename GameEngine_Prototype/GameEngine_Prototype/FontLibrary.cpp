#include "FontLibrary.h"
#include "Text.h"


FontLibrary::FontLibrary()
{
}


FontLibrary::~FontLibrary()
{
}

void FontLibrary::LoadAssetIntoLibrary(Text t)
{
	Text* loadedText = &t;

	library.insert({ t.fontPath ,  loadedText });

	std::cout << "Hello World" << library.size() << "Bye World" << std::endl;
}

Text *& FontLibrary::LoadAsset(std::string filePath)
{
	Text* loadedText = new Text();

	loadedText->fontPath = filePath;

	library.insert({ filePath ,  loadedText});

	std::cout << "Hello World" << library.size() << "Bye World" << std::endl;

	return library[filePath];
}
