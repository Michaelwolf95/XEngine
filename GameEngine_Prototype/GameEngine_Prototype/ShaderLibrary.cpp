#include "ShaderLibrary.h"
#include "Shader.h"

ShaderLibrary::ShaderLibrary() {}
ShaderLibrary::~ShaderLibrary() {}

// Overloading method: pass filepaths create a query to load asset
Shader * ShaderLibrary::GetAsset(std::string vs, std::string fs)
{
	//std::cout << "ShaderLibrary::GetAsset called with arguments\n";
	//std::cout << "\tvs: " << vs << std::endl;
	//std::cout << "\tfs: " << fs << std::endl;

	// create query based on arguments
	ShaderQuery shaderQ{ vs, fs };

	// Find in library
	auto search = this->library.find(shaderQ);
	if (search == this->library.end())
	{
		// Not found in library.
		//std::cout << "Loading Shader: " << vs << std::endl;
		return LoadAsset(shaderQ);
	}
	else
	{
		// Found
		return this->library[shaderQ];
	}
}

// Overriden LoadAsset method
Shader *& ShaderLibrary::LoadAsset(ShaderQuery query)
{
	// make new shader
	Shader* loadedShader;
	loadedShader = query.fs.empty() ? 
		new Shader( query.vs.c_str() ) :
		new Shader( query.vs.c_str(), query.fs.c_str() );

	// save shader into shader library
	library.insert({ query, loadedShader });
	return library[query];
}
