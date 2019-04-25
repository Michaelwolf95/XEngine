#pragma once

#include <string>
#include "Component.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Text.h"
#include "serialization.h"

class ENGINE_API FPSText : public Component
{
public:
	static Registrar<FPSText> registrar;
	FPSText();
	~FPSText();
	void Start() override;
	void Update() override;
	void OnEnable() override;
	void ConnectToText();
	Text* fpsText;
	void setCurrentText(Text*);

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	}
};

