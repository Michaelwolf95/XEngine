#pragma once
#include "Collider.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace XEngine
{
	class ENGINE_API BoxCollider : public Collider
	{
	public:
		static Registrar<BoxCollider> registrar;
		BoxCollider();
		~BoxCollider();
		btPolyhedralConvexShape* CreateCollisionShape() override;
		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void DrawInspector() override;
		void OnDrawGizmosSelected() override;

		//btBoxShape* colShape = nullptr;
		// Convert to glm vec for serialization
		glm::vec3 boxSize = glm::vec3(1, 1, 1);
		btVector3* halfExtents = nullptr;
	private:
		friend class ::boost::serialization::access;
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<class Archive>
		void save(Archive &ar, const unsigned int version) const
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(boxSize);
		}
		template<class Archive>
		void load(Archive &ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(boxSize);
		}

	};
}