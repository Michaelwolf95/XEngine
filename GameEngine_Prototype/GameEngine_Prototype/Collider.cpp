#include "Collider.h"


//REGISTER_COMPONENT(AudioComponent, "AudioComponent")
namespace XEngine
{
	Collider::Collider()
	{
	}


	Collider::~Collider()
	{
	}

	btPolyhedralConvexShape * Collider::GetColShape()
	{
		return colShape;
	}

	void Collider::Start()
	{
		Init();
	}
}