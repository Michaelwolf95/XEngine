#include "ShaderLibrary.h"
#include "Shader.h"

ShaderLibrary::ShaderLibrary()
{
}

ShaderLibrary::~ShaderLibrary()
{
}

// Overloading method: pass filepaths create a query to load asset
Shader * ShaderLibrary::GetAsset(std::string vs, std::string fs)
{
	// create query based on arguments
	ShaderQuery shaderQ{ vs, fs };

	auto search = this->library.find(shaderQ);

	// find in library
	if (search == this->library.end())
	{
		// Not found in library.
		std::cout << "Shader not found in Library" << std::endl;
		return LoadAsset(shaderQ);
	}
	else
	{
		// Found
		std::cout << "Shader loaded from Library" << std::endl;
		return this->library[shaderQ];
	}
}

// Overriden LoadAsset method
Shader *& ShaderLibrary::LoadAsset(ShaderQuery query)
{
	// make new shader
	Shader* loadedShader = new Shader( query.vs.c_str(), query.fs.c_str() );

	// save shader into shader library
	library.insert({ query, loadedShader });
	std::cout << "Shader saved into Library" << std::endl;

	return library[query];
}
