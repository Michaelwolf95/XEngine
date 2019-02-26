#pragma once
#include <vector>
#include "GameObject.h"
#include "Serialization.h"
class Scene
{
	typedef GameObject * gameObject_pointer; // Needed because we need an OBJECT.
public:
	std::string name;
	std::vector<gameObject_pointer> rootGameObjects;
	bool isStarted = false;
	bool isLoaded = false;
	Scene();
	Scene(const char* _name);
	Scene(std::vector<gameObject_pointer> gameObjects);
	~Scene();
	void PrintScene();
	void Load();
	void Unload();
	void Start();
	void Update();
	GameObject* CreateGameObject(const char* name, Transform* parent = nullptr);
	void DeleteGameObject(GameObject* go);
private:
	friend std::ostream & operator<<(std::ostream &os, const Scene &scene);
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int) // file_version
	{
		//std::cout << "Serializing Scene." << std::endl;
		// in this program, these classes are never serialized directly but rather
		// through a pointer to the base class bus_stop. So we need a way to be
		// sure that the archive contains information about these derived classes.

		//ar.register_type(static_cast<Transform *>(NULL));

		//ar.template register_type<GameObject>();
		////ar.register_type(static_cast<gameObject_pointer*>(NULL));
		//ar.register_type(static_cast<GameObject**>(NULL));

		////ar.template register_type<bus_stop_corner>();
		//ar.register_type(static_cast<bus_stop_corner *>(NULL));
		//ar.template register_type<bus_stop_destination>();
		//ar.register_type(static_cast<bus_stop_destination *>(NULL));
		// serialization of stl collections is already defined
		// in the header
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(rootGameObjects);
	}
};

std::ostream & operator<<(std::ostream &os, const Scene &scene);

//
//template<class Archive>
//void Scene::serialize(Archive & ar, const unsigned int)
//{
//	// in this program, these classes are never serialized directly but rather
//	// through a pointer to the base class bus_stop. So we need a way to be
//	// sure that the archive contains information about these derived classes.
//	//ar.template register_type<bus_stop_corner>();
//
//	//ar.register_type(static_cast<gameObject_pointer*>(NULL));
//	ar.register_type(static_cast<GameObject**>(NULL));
//
//	//ar.template register_type<bus_stop_destination>();
//	//ar.register_type(static_cast<bus_stop_destination *>(NULL));
//	// serialization of stl collections is already defined
//	// in the header
//	ar & rootGameObjects;
//}