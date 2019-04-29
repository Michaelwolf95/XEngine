#pragma once
#include "Component.h"
//#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <boost/serialization/unordered_map.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "RenderableObject.h"
#include "Component.h"
#include "GameObject.h"
#include "Model.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

namespace XEngine {

	class ENGINE_API SpriteRenderer : public RenderableObject, public Component
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();
		static Registrar<SpriteRenderer> registrar;
		void Start() override;
		void Update() override;
		void OnEnable() override;
		void OnDisable() override;
		void DrawInspector() override;

		// Renderable Object functions
		void Setup() override;
		void Draw() override;
		Shader* shader = nullptr;
		GLuint quadVAO;
		std::string textureFilePath = "../../DemoProject/Assets/awesomeface.png";
		//unsigned int textureID = 0;
		Texture* texture;
		//std::string vertexShaderPath;
		//std::string fragmentShaderPath;

	private:
		bool isSetup = false;
		friend class boost::serialization::access;
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			//// invoke serialization of the base class 
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(textureFilePath);

		}
		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			// invoke serialization of the base class 
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(textureFilePath);

			Setup();
		}
	};
}