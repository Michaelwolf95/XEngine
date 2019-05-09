#pragma once
#include "Component.h"
#include "XEngine.h"
#include "Shader.h"
#include <vector>
namespace XEngine {
	class ENGINE_API SkyboxRenderer : public Component, public RenderableObject
	{
	public:
		static Registrar<SkyboxRenderer> registrar;
		SkyboxRenderer();
		~SkyboxRenderer();
		void Start() override;
		void Update() override;
		void DrawInspector() override;

		void OnEnable() override;
		void OnDisable() override;

		void Setup() override;
		void Draw() override;

		unsigned int skyboxVAO, skyboxVBO;
		unsigned int cubemapTexture;
		Shader* skyboxShader = nullptr;
		unsigned int loadCubemap(vector<std::string> faces);

		//std::string cubemapFaces[6];
		//std::vector<std::string>
		std::vector<std::string> cubemapFaces;

	private:
		bool isSetup = false;
		friend class boost::serialization::access;
		BOOST_SERIALIZATION_SPLIT_MEMBER()
			template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			// Add custom parameters here.
			ar & BOOST_SERIALIZATION_NVP(cubemapFaces);
		}
		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			// Add custom parameters here.
			ar & BOOST_SERIALIZATION_NVP(cubemapFaces);
			Setup();
		}

	};
}