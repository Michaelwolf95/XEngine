#pragma once
#include "Component.h"
#include "AudioManager.h"
#include <vector>

class DLLExport AudioComponent : public Component
{
public:
	static Registrar<AudioComponent> registrar;
	void Start() override;
	void Update() override;
	//float range, distance;
	bool is3D = false;
	int mChannel;

	std::string soundPath;
	AudioComponent();
	~AudioComponent();
	void Load2D(string, bool, bool);
	void Load3D(string, bool, bool, bool);
	void Play(string, glm::vec3, float);
	void Play();
	//vector<string> soundList;

	glm::vec3 getListener();

	void DrawInspector() override;

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		//// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(soundPath);
		ar & BOOST_SERIALIZATION_NVP(is3D);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(soundPath);
		ar & BOOST_SERIALIZATION_NVP(is3D);
	}
};