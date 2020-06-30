#ifndef AE_TERRAINSHADOWRENDERER_H
#define AE_TERRAINSHADOWRENDERER_H

#include "../System.h"
#include "Renderer.h"

namespace Atlas {

	namespace Renderer {

		class TerrainShadowRenderer : public Renderer {

		public:
			TerrainShadowRenderer();

			virtual void Render(Viewport* viewport, RenderTarget* target, Camera* camera, Scene::Scene* scene);

		private:
			void GetUniforms();

			Shader::Shader shader;

			Framebuffer framebuffer;

			Shader::Uniform* heightScale;
			Shader::Uniform* tileScale;
			Shader::Uniform* patchSize;

			Shader::Uniform* nodeLocation;
			Shader::Uniform* nodeSideLength;

			Shader::Uniform* leftLoD;
			Shader::Uniform* topLoD;
			Shader::Uniform* rightLoD;
			Shader::Uniform* bottomLoD;

			Shader::Uniform* lightSpaceMatrix;


		};

	}

}

#endif