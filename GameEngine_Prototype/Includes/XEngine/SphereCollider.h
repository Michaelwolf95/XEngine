#pragma once
#include "Collider.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace XEngine
{
	class ENGINE_API SphereCollider : public Collider
	{
	public:
		static Registrar<SphereCollider> registrar;
		SphereCollider();
		~SphereCollider();
		btConvexInternalShape* CreateCollisionShape() override;
		void Start() override;
		void Update() override;
		void DrawInspector() override;
		void OnDrawGizmosSelected() override;

		float radius = 0.5f;

	private:
		friend class ::boost::serialization::access;
		BOOST_SERIALIZATION_SPLIT_MEMBER()
			template<class Archive>
		void save(Archive &ar, const unsigned int version) const
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(radius);
		}
		template<class Archive>
		void load(Archive &ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(radius);
		}
	};

}