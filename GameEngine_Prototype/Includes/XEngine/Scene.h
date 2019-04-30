#pragma once
#include <vector>
#include "GameObject.h"
#include "Serialization.h"
class ENGINE_API Scene
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
	void AddExistingGameObject(GameObject_ptr go);

	void ScheduleDelete(GameObject_ptr go);
	void Cleanup();

private:
	std::vector<GameObject_ptr> allGameObjects;

	std::vector<GameObject_ptr> gameObjectsToDelete;

	void PrintGameObject(GameObject* go, std::string prefix);

	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		//ar & BOOST_SERIALIZATION_NVP(rootGameObjects);
		ar & BOOST_SERIALIZATION_NVP(allGameObjects);
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
	}
};
typedef std::shared_ptr<Scene> Scene_ptr;
