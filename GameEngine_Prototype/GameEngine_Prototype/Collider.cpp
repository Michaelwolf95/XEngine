#include "Collider.h"
#include "PhysicsManager.h"

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

	void Collider::Init()
	{
		if (!isInitialized)
		{
			std::cout << "INITIALIZING COLLIDER\n";
			//TODO: Figure out adding RBs dynamically at runtime?
			if (this->attachedRigidbody == nullptr)
			{
				gameObject->FindComponent(typeid(Rigidbody), (void**)&attachedRigidbody);

			}

			colShape = CreateCollisionShape();
			PhysicsManager::getInstance().AddCollisionShape(colShape);

			if (this->attachedRigidbody != nullptr)
			{
				// Init and setup.
				if (attachedRigidbody->isInitialized == false)
				{
					attachedRigidbody->Init();
				}

				std::cout << "Attaching Collider...\n";
				attachedRigidbody->AttachCollider(this);

			}
			else
			{
				// Create ghost object for trigger mode??
			}


			isInitialized = true;
		}
	}
}