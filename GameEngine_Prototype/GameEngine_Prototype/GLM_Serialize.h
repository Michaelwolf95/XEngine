#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Serialization.h"
//<3, float, 0>

// THIS IS PROBABLY WHERE THE PROBLEM IS ==========
//BOOST_CLASS_EXPORT_GUID(glm::vec3, "vec3")
BOOST_SERIALIZATION_SPLIT_FREE(glm::vec3)
BOOST_SERIALIZATION_SPLIT_FREE(glm::vec4)
BOOST_SERIALIZATION_SPLIT_FREE(glm::quat)
namespace boost
{
	namespace serialization
	{
		// Vec2
		template<class Archive> void save(Archive& ar, const glm::vec2& vec, unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(vec.x);
			ar & BOOST_SERIALIZATION_NVP(vec.y);
		}
		template<class Archive> void load(Archive& ar, glm::vec2& vec, unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(vec.x);
			ar & BOOST_SERIALIZATION_NVP(vec.y);
		}

		// Vec3
		template<class Archive> void save(Archive& ar, const glm::vec3& vec, unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(vec.x);
			ar & BOOST_SERIALIZATION_NVP(vec.y);
			ar & BOOST_SERIALIZATION_NVP(vec.z);
		}
		template<class Archive> void load(Archive& ar, glm::vec3& vec, unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(vec.x);
			ar & BOOST_SERIALIZATION_NVP(vec.y);
			ar & BOOST_SERIALIZATION_NVP(vec.z);
		}

		// Vec4
		template<class Archive> void save(Archive& ar, const glm::vec4& vec, unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(vec.x);
			ar & BOOST_SERIALIZATION_NVP(vec.y);
			ar & BOOST_SERIALIZATION_NVP(vec.z);
			ar & BOOST_SERIALIZATION_NVP(vec.w);
		}
		template<class Archive> void load(Archive& ar, glm::vec4& vec, unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(vec.x);
			ar & BOOST_SERIALIZATION_NVP(vec.y);
			ar & BOOST_SERIALIZATION_NVP(vec.z);
			ar & BOOST_SERIALIZATION_NVP(vec.w);
		}

		// Quat
		template<class Archive> void save(Archive& ar, const glm::quat& q, unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(q.x);
			ar & BOOST_SERIALIZATION_NVP(q.y);
			ar & BOOST_SERIALIZATION_NVP(q.z);
			ar & BOOST_SERIALIZATION_NVP(q.w);
		}
		template<class Archive> void load(Archive& ar, glm::quat& q, unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(q.x);
			ar & BOOST_SERIALIZATION_NVP(q.y);
			ar & BOOST_SERIALIZATION_NVP(q.z);
			ar & BOOST_SERIALIZATION_NVP(q.w);
		}
	}
}