#pragma once
#include <vector>
#include "GameObject.h"
#include "Serialization.h"
class Scene
{
public:
	std::string name;
	std::string filePath;
	std::vector<GameObject_ptr> rootGameObjects;
	bool isStarted = false;
	bool isLoaded = false;
	Scene();
	Scene(const char* _name);
	Scene(std::vector<GameObject_ptr> gameObjects);
	~Scene();
	void PrintScene();
	void Load();
	void Reset();
	void Unload();
	void Start();
	void StartGameObject(GameObject_ptr go);
	void Update();
	void UpdateGameObject(GameObject_ptr go);
	void FixedUpdate();
	void FixedUpdateGameObject(GameObject_ptr go);
	GameObject_ptr CreateGameObject(const char* name, Transform* parent = nullptr);
	void DeleteGameObject(GameObject_ptr go);
	void OnHierarchyUpdate();
	//GameObject_ptr FindSharedGameObjectPointer(GameObject* go);
private:
	std::vector<GameObject_ptr> allGameObjects;

	void PrintGameObject(GameObject* go, std::string prefix);

	friend std::ostream & operator<<(std::ostream &os, const Scene &scene);
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		//ar & BOOST_SERIALIZATION_NVP(rootGameObjects);
		ar & BOOST_SERIALIZATION_NVP(allGameObjects);

		 
		//std::vector<uintptr_t> parentAddresses;
		//for (size_t i = 0; i < allGameObjects.size(); i++)
		//{
		//	// Note: You should allocate the correct size, here I assumed that you are using 32 bits address
		//	char addr[] = "0x00000000";
		//	std::sprintf(addr, "%p", allGameObjects[i]->transform->parent);
		//	uintptr_t addr = reinterpret_cast<uintptr_t>(&allGameObjects[i]->transform->parent);
		//	parentAddresses.push_back(addr);
		//}
		//ar & BOOST_SERIALIZATION_NVP(parentAddresses);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version) // file_version
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		//ar & BOOST_SERIALIZATION_NVP(rootGameObjects);
		ar & BOOST_SERIALIZATION_NVP(allGameObjects);


		for (size_t i = 0; i < allGameObjects.size(); i++)
		{
			if (allGameObjects[i]->transform->GetParent() == nullptr)
			{
				rootGameObjects.push_back(allGameObjects[i]);
			}
			else
			{
				// Reconstruct Transforms

			}
		}
		//std::vector<uintptr_t> transformAddresses;
		//ar & BOOST_SERIALIZATION_NVP(transformAddresses);
		//for (size_t i = 0; i < allGameObjects.size(); i++)
		//{
		//	// Note: You should allocate the correct size, here I assumed that you are using 32 bits address
		//	char addr[] = "0x00000000";
		//	std::sprintf(addr, "%p", allGameObjects[i]->transform->parent);
		//	uintptr_t addr = reinterpret_cast<uintptr_t>(&allGameObjects[i]->transform->parent);
		//	parentAddresses.push_back(addr);
		//}

		/*for (size_t i = 0; i < rootGameObjects.size(); i++)
		{
			allGameObjects.push_back(rootGameObjects[i]);
			std::vector<Transform*> childs = rootGameObjects[i]->transform->GetChildren();
			for (size_t j = 0; j < childs.size(); j++)
			{
				allGameObjects.push_back(childs[j]->gameObject->GetSelfPtr());
			}
		}*/
	}
	//template<class Archive>
	//void serialize(Archive & ar, const unsigned int) // file_version
	//{
	//	//std::cout << "Serializing Scene." << std::endl;
	//	// in this program, these classes are never serialized directly but rather
	//	// through a pointer to the base class bus_stop. So we need a way to be
	//	// sure that the archive contains information about these derived classes.

	//	//ar.register_type(static_cast<Transform *>(NULL));

	//	//ar.template register_type<GameObject>();
	//	////ar.register_type(static_cast<gameObject_pointer*>(NULL));
	//	//ar.register_type(static_cast<GameObject**>(NULL));

	//	////ar.template register_type<bus_stop_corner>();
	//	//ar.register_type(static_cast<bus_stop_corner *>(NULL));
	//	//ar.template register_type<bus_stop_destination>();
	//	//ar.register_type(static_cast<bus_stop_destination *>(NULL));
	//	// serialization of stl collections is already defined
	//	// in the header
	//	ar & BOOST_SERIALIZATION_NVP(name);
	//	ar & BOOST_SERIALIZATION_NVP(rootGameObjects);
	//}
};
typedef std::shared_ptr<Scene> Scene_ptr;

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