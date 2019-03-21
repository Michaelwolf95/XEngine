#include "MaterialLibrary.h"
#include "AssetManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Serialization.h"

MaterialLibrary::MaterialLibrary()
{
}


MaterialLibrary::~MaterialLibrary()
{
}

void MaterialLibrary::SaveAllMaterials()
{

}

// Overloading method: pass filepaths to create a query to load asset
Material *& MaterialLibrary::GetAsset(std::string name, std::string vertPath, std::string fragPath)
{
	// create query based on arguments
	MaterialQuery materialQ{ name, vertPath, fragPath };

	auto search = this->library.find(materialQ);

	// find in library
	if (search == this->library.end())
	{
		// Not found in library.
		std::cout << "Material not found in Library" << std::endl;
		return LoadAsset(materialQ);
	}
	else
	{
		// Found
		std::cout << "Material loaded from Library" << std::endl;
		return this->library[materialQ];
	}
}

// Overriden LoadAsset method
Material *& MaterialLibrary::LoadAsset(MaterialQuery materialQ)
{
	Material* loadedMaterial =  new Material(materialQ.name, materialQ.vertPath, materialQ.fragPath);
	
	// load from file in directory
	if (LoadMaterialFromFileByName(*loadedMaterial, materialQ.name.c_str()))
	{ 
		std::cout << "Material loaded from file in Assets directory" << std::endl;
	}
	else // cant load, then create new one
	{
		std::cout << "Material not loaded from file in Assets directory" << std::endl;
	}

	// save into library
	library.insert({ materialQ, loadedMaterial });
	std::cout << "Material saved into Library" << std::endl;

	// create file and save into directory
	SaveMaterialToFile(*loadedMaterial);

	return library[materialQ];
}

// Saving material to file
void MaterialLibrary::SaveMaterialToFile(const Material &m) {
	std::string filename = "../Assets/Materials/";	// material filepath
	filename += m.name + ".material";				// material file
	SaveMaterialToFile(m, filename.c_str());
}

// Saving material to file
void MaterialLibrary::SaveMaterialToFile(const Material &m, const char * fileName)
{
	std::cout << "Saving Material: " << fileName << std::endl;
	// make an archive
	std::ofstream ofs(fileName);
	if (!ofs)
	{
		std::cout << "Cannot open outfile" << std::endl;
		return;
	}
	if (ofs.bad())
	{
		std::cout << "Out File Stream BAD" << std::endl;
		return;
	}

	boost::archive::xml_oarchive oa(ofs);
	oa << BOOST_SERIALIZATION_NVP(m);
}

// Loading material from file by name
bool MaterialLibrary::LoadMaterialFromFileByName(Material &m, const char * materialName)
{
	std::string filename("../Assets/Materials/");
	filename += std::string(materialName) + ".material";
	return LoadMaterialFromFile(m, filename.c_str());
}

// Loading material from file
bool MaterialLibrary::LoadMaterialFromFile(Material &m, const char * fileName)
{
	// open the archive 
	std::ifstream ifs(fileName);
	if (!ifs.good()) //Doesn't exist 
	{
		return false;
	}

	//boost::archive::text_iarchive ia(ifs);
	boost::archive::xml_iarchive ia(ifs);

	// restore from the archive
	ia >> BOOST_SERIALIZATION_NVP(m);

	m.filePath = fileName;
	return true;
}
