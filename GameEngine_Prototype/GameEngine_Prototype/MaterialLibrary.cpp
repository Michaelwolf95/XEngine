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

//void MaterialLibrary::SaveAllMaterials()
//{
//
//}

// Overloading method: pass material name(extension will be added)
Material *& MaterialLibrary::GetAsset(std::string fileName)
{
	//std::string filePath = "../Assets/Materials/";	// material filepath
	//filePath += fileName + ".material";				// material file
	auto search = this->library.find(fileName);

	// find in library
	if (search == this->library.end())
	{
		// Not found in library.
		//std::cout << "Material not found in Library" << std::endl;
		return LoadAsset(fileName);
	}
	else
	{
		// Found
		//std::cout << "Material loaded from Library" << std::endl;
		return this->library[fileName];
	}
}

// Overriden LoadAsset method
Material *& MaterialLibrary::LoadAsset(std::string filePath)
{
	std::cout << "\tLoading Material: " << filePath << std::endl;
	// get material name from filepath
	std::string fileName = filePath.substr(filePath.find_last_of("/") + 1);
	size_t lastindex = fileName.find_last_of(".");
	std::string materialName = fileName.substr( 0, lastindex );

	//Material* loadedMaterial =  new Material(materialName, "multilights.vs", "multilights.fs");
	Material* loadedMaterial =  new Material(materialName, "multilights.shader", "");
	loadedMaterial->filePath = filePath;

	// load from file in directory
	if (LoadMaterialFromFile(*loadedMaterial, filePath.c_str()) )
	{ 
		std::cout << "\t\tMaterial loaded from file." << std::endl;
	}
	else // cant load, then create new one
	{
		//std::cout << "Material not loaded from file in Assets directory ================================" << std::endl;
		std::cout << "\t\tMaterial created and saved to file." << std::endl;


		// create file and save into directory
		SaveMaterialToFile(*loadedMaterial, filePath.c_str());
	}

	// save into library
	library.insert({ filePath, loadedMaterial });

	return library[filePath];
}

// Saving material to file
void MaterialLibrary::SaveMaterialToFile(const Material &m, const char * filePath)
{
	std::cout << "Saving Material: " << filePath << std::endl;
	// make an archive
	std::ofstream ofs(filePath);
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

// Loading material from file
bool MaterialLibrary::LoadMaterialFromFile(Material &m, const char * filePath)
{
	// open the archive 
	std::ifstream ifs(filePath);
	if (!ifs.good()) //Doesn't exist 
	{
		return false;
	}

	// restore from the archive
	boost::archive::xml_iarchive ia(ifs);
	ia >> BOOST_SERIALIZATION_NVP(m);
	m.filePath = filePath;
	return true;
}
