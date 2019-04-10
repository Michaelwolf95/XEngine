#pragma once
#include "Component.h"
#include "AudioManager.h"

class DLLExport AudioComponent : public Component
{
public:
	static Registrar<AudioComponent> registrar;
	void Start() override;
	void Update() override;
	//float range, distance;
	bool is3D = false;
	bool repeat = false;

	std::string soundPath;
	AudioComponent();
	~AudioComponent();
	void Load3D(string, bool, bool, bool);
	void Play(string, glm::vec3, float);
	void Play();
	void Pause();
	void UnPause();
	void DrawInspector() override;

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>

	//when adding something new to here, when using a scene already made, add it to the save first
	//then run the scene and save it, then add it to the load
	void save(Archive & ar, const unsigned int version) const
	{
		//// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(soundPath);
		ar & BOOST_SERIALIZATION_NVP(is3D);
		ar & BOOST_SERIALIZATION_NVP(repeat);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(soundPath);
		ar & BOOST_SERIALIZATION_NVP(is3D);
		ar & BOOST_SERIALIZATION_NVP(repeat);
	}
};